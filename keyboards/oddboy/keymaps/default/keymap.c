#include QMK_KEYBOARD_H
#include "os_detection.h"

enum custom_keycodes {
    CPI_UP = SAFE_RANGE,
    CPI_DN,
    SCR_MOD,
    MAC_BACK,
};

static bool scroll_mode = false;
static int16_t scroll_accu_x = 0;
static int16_t scroll_accu_y = 0;
static int8_t scroll_axis = 0;  // 0=undecided, 1=horizontal, -1=vertical
#define SCROLL_DIVISOR 100
#define SCROLL_LOCK_THRESHOLD 50
#define SCROLL_UNLOCK_THRESHOLD 150

// ── Ball-to-key layer mappings ─────────────────────────────────────────────
// Add one entry per layer where trackball movement should send keycodes.
// left/right map to X axis, up/down map to Y axis.
// one_shot_x / one_shot_y: fire once per gesture; won't fire again until ball
// stops for BALL_KEY_STOP_MS and restarts. Use false for keys that should
// repeat (volume) and true for keys that should fire once (prev/next track).
typedef struct {
    uint8_t  layer;
    uint16_t left;
    uint16_t right;
    uint16_t up;
    uint16_t down;
    bool     one_shot_x;
    bool     one_shot_y;
} ball_key_layer_t;

#define BALL_KEY_THRESHOLD         80   // accumulator threshold for debounce axes
#define BALL_KEY_ONESHOT_THRESHOLD 200  // minimum distance before one-shot fires
#define BALL_KEY_DEBOUNCE          200  // ms between repeats (non-one-shot axes)
#define BALL_KEY_STOP_MS           80   // ms of no movement = gesture ended / axis unlock
#define BALL_KEY_AXIS_LOCK         40   // accumulator to commit to an axis

static const ball_key_layer_t ball_key_layers[] = {
    { 2, KC_MPRV, KC_MNXT, KC_VOLU, KC_VOLD, true, false },
};

static int16_t  ball_key_accu_x       = 0;
static int16_t  ball_key_accu_y       = 0;
static uint16_t ball_key_timer_x      = 0;
static uint16_t ball_key_timer_y      = 0;
static uint16_t ball_key_last_move_x  = 0;
static uint16_t ball_key_last_move_y  = 0;
static bool     ball_key_fired_x      = false;
static bool     ball_key_fired_y      = false;
static int8_t   ball_key_axis         = 0;  // 0=undecided, 1=X locked, -1=Y locked

void keyboard_post_init_user(void) {
    keymap_config.swap_lctl_lgui = true;
    keymap_config.swap_rctl_rgui = true;
    debug_enable = true;
}

void housekeeping_task_user(void) {
    static os_variant_t last_os = OS_UNSURE;
    os_variant_t os = detected_host_os();
    if (os != OS_UNSURE && os != last_os) {
        last_os = os;
        bool is_mac = (os == OS_MACOS || os == OS_IOS);
        keymap_config.swap_lctl_lgui = is_mac;
        keymap_config.swap_rctl_rgui = is_mac;
        eeconfig_update_keymap(keymap_config.raw);
        uprintf("os poll: %d swap=%d\n", os, is_mac);
    }
}

void pointing_device_init_user(void) {
    set_auto_mouse_layer(3);
    set_auto_mouse_enable(true);
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        switch (keycode) {
            case CPI_UP:
                pointing_device_set_cpi(pointing_device_get_cpi() + 200);
                return false;
            case CPI_DN:
                pointing_device_set_cpi(pointing_device_get_cpi() - 200);
                return false;
            case SCR_MOD:
                scroll_mode = true;
                return false;
            case MAC_BACK:
                if (detected_host_os() == OS_MACOS || detected_host_os() == OS_IOS) {
                    tap_code16(LGUI(KC_LBRC));
                } else {
                    tap_code16(KC_WBAK);
                }
                return false;
        }
    } else {
        if (layer_state_is(3) && keycode == CTL_T(KC_A)) {
            layer_off(3);
        }
        switch (keycode) {
            case SCR_MOD:
                scroll_mode = false;
                scroll_axis = 0;
                scroll_accu_x = 0;
                scroll_accu_y = 0;
                return false;
        }
    }
    return true;
}

report_mouse_t pointing_device_task_combined_user(report_mouse_t left_report, report_mouse_t mouse_report) {
    // joystick (left half) → scroll
    static int16_t joy_accu_x = 0, joy_accu_y = 0;
    joy_accu_x     += left_report.x;
    joy_accu_y     += left_report.y;
    mouse_report.h += joy_accu_x / JOYSTICK_SCROLL_DIVISOR;
    joy_accu_x     %= JOYSTICK_SCROLL_DIVISOR;
    mouse_report.v -= joy_accu_y / JOYSTICK_SCROLL_DIVISOR;
    joy_accu_y     %= JOYSTICK_SCROLL_DIVISOR;
    mouse_report.buttons |= left_report.buttons;
    // joystick x/y consumed as scroll — clear so combine doesn't add them to cursor
    left_report.x = 0;
    left_report.y = 0;

    // trackball (right half) — existing logic follows
    // Ball-to-key: check if the highest active layer has a mapping
    uint8_t cur_layer = get_highest_layer(layer_state);
    for (uint8_t i = 0; i < sizeof(ball_key_layers) / sizeof(ball_key_layers[0]); i++) {
        if (ball_key_layers[i].layer == cur_layer) {
            // track last movement time
            if (mouse_report.x != 0) ball_key_last_move_x = timer_read();
            if (mouse_report.y != 0) ball_key_last_move_y = timer_read();
            uint16_t idle_x = timer_elapsed(ball_key_last_move_x);
            uint16_t idle_y = timer_elapsed(ball_key_last_move_y);

            // reset one-shot lock and axis lock when ball has been still long enough
            if (idle_x > BALL_KEY_STOP_MS && idle_y > BALL_KEY_STOP_MS) {
                ball_key_axis = 0;
                ball_key_accu_x = 0;
                ball_key_accu_y = 0;
            }
            if (ball_key_fired_x && idle_x > BALL_KEY_STOP_MS) ball_key_fired_x = false;
            if (ball_key_fired_y && idle_y > BALL_KEY_STOP_MS) ball_key_fired_y = false;

            ball_key_accu_x += mouse_report.x;
            ball_key_accu_y += mouse_report.y;

            // axis lock: commit to dominant axis once it pulls ahead
            if (ball_key_axis == 0) {
                if (abs(ball_key_accu_x) > BALL_KEY_AXIS_LOCK && abs(ball_key_accu_x) > abs(ball_key_accu_y))
                    ball_key_axis = 1;
                else if (abs(ball_key_accu_y) > BALL_KEY_AXIS_LOCK && abs(ball_key_accu_y) > abs(ball_key_accu_x))
                    ball_key_axis = -1;
            }
            // suppress the non-dominant axis
            if (ball_key_axis == 1)  ball_key_accu_y = 0;
            if (ball_key_axis == -1) ball_key_accu_x = 0;

            bool can_x = ball_key_layers[i].one_shot_x
                ? !ball_key_fired_x
                : timer_elapsed(ball_key_timer_x) > BALL_KEY_DEBOUNCE;
            int16_t thr_x = ball_key_layers[i].one_shot_x ? BALL_KEY_ONESHOT_THRESHOLD : BALL_KEY_THRESHOLD;
            int16_t thr_y = ball_key_layers[i].one_shot_y ? BALL_KEY_ONESHOT_THRESHOLD : BALL_KEY_THRESHOLD;

            if (can_x) {
                if (ball_key_accu_x <= -thr_x) {
                    tap_code16(ball_key_layers[i].left);
                    ball_key_accu_x = 0;
                    if (ball_key_layers[i].one_shot_x) ball_key_fired_x = true;
                    else ball_key_timer_x = timer_read();
                } else if (ball_key_accu_x >= thr_x) {
                    tap_code16(ball_key_layers[i].right);
                    ball_key_accu_x = 0;
                    if (ball_key_layers[i].one_shot_x) ball_key_fired_x = true;
                    else ball_key_timer_x = timer_read();
                }
            }

            bool can_y = ball_key_layers[i].one_shot_y
                ? !ball_key_fired_y
                : timer_elapsed(ball_key_timer_y) > BALL_KEY_DEBOUNCE;
            if (can_y) {
                if (ball_key_accu_y <= -thr_y) {
                    tap_code16(ball_key_layers[i].up);
                    ball_key_accu_y = 0;
                    if (ball_key_layers[i].one_shot_y) ball_key_fired_y = true;
                    else ball_key_timer_y = timer_read();
                } else if (ball_key_accu_y >= thr_y) {
                    tap_code16(ball_key_layers[i].down);
                    ball_key_accu_y = 0;
                    if (ball_key_layers[i].one_shot_y) ball_key_fired_y = true;
                    else ball_key_timer_y = timer_read();
                }
            }
            mouse_report.x = 0;
            mouse_report.y = 0;
            return pointing_device_combine_reports(left_report, mouse_report);
        }
    }

    if (scroll_mode) {
        scroll_accu_x += mouse_report.x;
        scroll_accu_y += mouse_report.y;

        if (scroll_axis == 0) {
            if (abs(scroll_accu_x) > SCROLL_LOCK_THRESHOLD)
                scroll_axis = 1;
            else if (abs(scroll_accu_y) > SCROLL_LOCK_THRESHOLD)
                scroll_axis = -1;
        }

        if (scroll_axis == 1) {
            if (abs(scroll_accu_y) > SCROLL_UNLOCK_THRESHOLD) {
                scroll_axis = 0;
                scroll_accu_x = 0;
                scroll_accu_y = 0;
            } else {
                mouse_report.h = scroll_accu_x / SCROLL_DIVISOR;
                scroll_accu_x %= SCROLL_DIVISOR;
                // do not zero scroll_accu_y — let it accumulate for unlock detection
            }
        } else if (scroll_axis == -1) {
            if (abs(scroll_accu_x) > SCROLL_UNLOCK_THRESHOLD) {
                scroll_axis = 0;
                scroll_accu_x = 0;
                scroll_accu_y = 0;
            } else {
                mouse_report.v = -(scroll_accu_y / SCROLL_DIVISOR);
                scroll_accu_y %= SCROLL_DIVISOR;
                // do not zero scroll_accu_x — let it accumulate for unlock detection
            }
        }

        mouse_report.x = 0;
        mouse_report.y = 0;
    }
    return pointing_device_combine_reports(left_report, mouse_report);
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    [0] = LAYOUT(
        KC_NO        , KC_Q       , KC_W, KC_E, KC_R, KC_T,    KC_Y  , KC_U , KC_I   , KC_O  , KC_P          , KC_NO,
        KC_LGUI      , CTL_T(KC_A), KC_S, KC_D, KC_F, KC_G,    KC_H  , KC_J , KC_K   , KC_L  , CTL_T(KC_SCLN), KC_NO,
        KC_NO        , SFT_T(KC_Z), KC_X, KC_C, KC_V, KC_B,    KC_N  , KC_M , KC_COMM, KC_DOT, SFT_T(KC_SLSH), KC_NO,
        LT(2, KC_SPC), KC_LALT,    KC_ENT, MO(1)
    ),

    [1] = LAYOUT(
        KC_NO  , KC_NO  , KC_1, KC_2, KC_3, KC_NO,    KC_NO  , KC_NO  , KC_NO, KC_NO   , KC_NO  , KC_NO,
        KC_TRNS, QK_GESC, KC_4, KC_5, KC_6, KC_0,    KC_LEFT, KC_DOWN, KC_UP, KC_RIGHT, KC_BSPC, KC_NO,
        KC_TRNS, KC_LSFT, KC_7, KC_8, KC_9, KC_NO,    KC_NO  , KC_NO  , KC_NO, KC_NO   , KC_RSFT, KC_NO,
        MO(4)  , KC_TRNS,    KC_TRNS, KC_NO
    ),

    [2] = LAYOUT(
        KC_NO  , QK_GESC, KC_NO   , KC_NO  , KC_NO  , KC_NO,    KC_GRAVE, KC_MINS, KC_EQL , KC_NO, KC_NO  , KC_NO,
        KC_NO  , KC_LCTL, KC_TAB  , KC_MPRV, KC_MPLY, KC_MNXT,    KC_NO   , KC_LBRC, KC_RBRC, KC_NO, KC_QUOT, KC_NO,
        KC_TRNS, KC_LSFT, MAC_BACK, KC_VOLD, KC_VOLU, KC_NO,    KC_NO   , KC_LPRN, KC_RPRN, KC_NO, KC_BSLS, KC_NO,
        KC_NO  , KC_TRNS,    KC_TRNS , MO(3)
    ),

    [3] = LAYOUT(
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,    KC_NO  , CPI_UP    , CPI_DN    , KC_NO     , KC_NO  , KC_NO,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,    KC_NO  , KC_NO     , KC_NO     , KC_NO     , KC_NO  , KC_NO,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,    KC_NO  , KC_MS_BTN1, KC_MS_BTN2, KC_MS_BTN3, SCR_MOD, KC_NO,
        KC_TRNS, KC_TRNS,    KC_TRNS, KC_NO
    ),

    [4] = LAYOUT(
        KC_NO, KC_NO  , KC_F1, KC_F2, KC_F3, KC_F10,    KC_NO  , KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_LCTL, KC_F4, KC_F5, KC_F6, KC_F11,    KC_NO  , KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_LSFT, KC_F7, KC_F8, KC_F9, KC_F12,    KC_NO  , KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_TRNS,    KC_TRNS, KC_NO
    )

};
