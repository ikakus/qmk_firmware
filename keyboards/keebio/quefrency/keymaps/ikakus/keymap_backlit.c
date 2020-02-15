#include QMK_KEYBOARD_H

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
	[0] = LAYOUT(KC_GESC, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0, KC_MINS, KC_EQL, KC_DEL, KC_BSPC, KC_TAB, KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P, KC_LBRC, KC_RBRC, KC_BSLS, KC_CAPS, KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, KC_SCLN, KC_QUOT, KC_ENT, KC_LSFT, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH, KC_RSFT, KC_RCTL, KC_LCTL, KC_LGUI, KC_LALT, MO(1), KC_SPC, LT(1,KC_SPC), KC_NO, KC_RALT, KC_LEFT, KC_DOWN, KC_UP, KC_RGHT),
	[1] = LAYOUT(KC_GRV, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_F12, KC_TRNS, KC_DEL, KC_TRNS, RGB_TOG, RGB_MOD, RGB_RMOD, KC_NO, KC_BSPC, KC_NO, KC_PGUP, KC_PGDN, KC_HOME, KC_END, KC_NO, KC_NO, KC_TRNS, KC_TRNS, RGB_HUI, RGB_HUD, RGB_SAI, RGB_SAD, KC_DEL, KC_LEFT, KC_DOWN, KC_UP, KC_RGHT, KC_BSPC, KC_DEL, KC_TRNS, KC_TRNS, RGB_VAI, RGB_VAD, RGB_SPI, RGB_SPD, KC_ENT, KC_ENT, KC_NO, KC_NO, KC_NO, KC_NO, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_MUTE, KC_VOLD, KC_VOLU, KC_TRNS)
};
// My layer names
#define _MAIN 0
#define _FUNC 1

uint32_t layer_state_set_user(uint32_t state) {
  switch (biton32(state)) {
    case _MAIN:
      rgblight_disable();
      rgblight_set(); // The number depends on your configuration's BACKLIGHT_LEVELS definition at config.h
      break;
    case _FUNC:
      rgblight_enable();
      rgblight_set();
      break;
  }
  return state;
}
