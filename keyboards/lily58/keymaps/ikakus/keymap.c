#include QMK_KEYBOARD_H
#include <stdio.h>

#ifdef PROTOCOL_LUFA
  #include "lufa.h"
  #include "split_util.h"
#endif
#ifdef SSD1306OLED
  #include "ssd1306.h"
#endif


extern keymap_config_t keymap_config;

extern uint8_t is_master;
static uint32_t timer = 0;
char term_symbol_visible [ ] = ">_ ";
char term_symbol_invisible [ ] = ">  ";
char term_symbol [] = "   ";
bool underscore_visible = true;
bool keylogger_enabled = false;

#define _QWERTY 0
#define _LOWER 1
#define _RAISE 2


enum custom_keycodes {
  QWERTY = SAFE_RANGE,
  LOWER,
  RAISE
};


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

[_QWERTY] = LAYOUT(KC_GESC, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0, KC_BSPC, KC_TAB, KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P, KC_BSLS, KC_CAPS, KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, KC_SCLN, KC_QUOT, KC_LSFT, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_LBRC, KC_LGUI, KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH, KC_RSFT, KC_LCTL, KC_LALT, MO(2), KC_SPC, KC_ENT, MO(1), KC_RALT, KC_RCTL),

[_LOWER] = LAYOUT(KC_TRNS, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_TRNS, KC_F13, KC_NO, KC_NO, KC_NO, KC_NO, KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_F12, KC_TRNS, KC_NO, KC_NO, KC_MRWD, KC_MFFD, KC_MPLY, KC_LEFT, KC_DOWN, KC_UP, KC_RGHT, KC_BSPC, KC_DEL, KC_TRNS, KC_NO, KC_NO, KC_VOLD, KC_VOLU, KC_MUTE, KC_NO, KC_NO, KC_NO, KC_PGUP, KC_PGDOWN, KC_NO, KC_NO, KC_TRNS, KC_TRNS, KC_TRNS, KC_NO, KC_TRNS, KC_NO, KC_NO, KC_TRNS, KC_TRNS),


[_RAISE] = LAYOUT(KC_TRNS, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_NO, KC_MINS, KC_EQL, KC_NO, KC_NO, KC_NO, KC_TRNS, KC_NO, KC_NO, KC_MRWD, KC_MFFD, KC_MPLY, KC_NO, KC_LCBR, KC_RCBR, KC_NO, KC_NO, KC_NO, KC_TRNS, KC_NO, KC_NO, KC_VOLD, KC_VOLU, KC_MUTE, KC_NO, KC_NO, KC_NO, KC_LBRC, KC_RBRC, KC_NO, KC_NO, KC_TRNS, KC_NO, KC_NO, KC_NO, KC_NO, KC_TRNS, KC_NO, KC_TRNS, KC_TRNS)

};


void matrix_init_user(void) {
    //SSD1306 OLED init, make sure to add #define SSD1306OLED in config.h
    #ifdef SSD1306OLED
     //iota_gfx_init(!has_usb());   // turns on the display
    #endif

    strcpy(term_symbol, term_symbol_visible);
    timer = timer_read32();
}

//SSD1306 OLED update loop, make sure to add #define SSD1306OLED in config.h
#ifdef SSD1306OLED

// When add source files to SRC in rules.mk, you can use functions.
const char *read_layer_state(void);
const char *read_logo(void);
void set_keylog(uint16_t keycode, keyrecord_t *record);
const char *read_keylog(void);
const char *read_keylogs(void);

void matrix_scan_user(void) {
   iota_gfx_task();
}

void matrix_render_user(struct CharacterMatrix *matrix) {
  if (is_master) {
    // If you want to change the display of OLED, you need to change here

    //matrix_write_ln(matrix, read_layer_state());
    //matrix_write_ln(matrix, "");
    matrix_write(matrix, term_symbol);

    if(keylogger_enabled == true){
    //	matrix_write_ln(matrix, read_keylogs());
    }else{
    //	matrix_write_ln(matrix, read_keylog());
    }

  } else {
    //matrix_write_ln(matrix, read_layer_state());
    //matrix_write_ln(matrix, "");
    matrix_write(matrix, term_symbol);
    //matrix_write_ln(matrix, read_keylog());

  }
}

void matrix_update(struct CharacterMatrix *dest, const struct CharacterMatrix *source) {
  if (memcmp(dest->display, source->display, sizeof(dest->display))) {
    memcpy(dest->display, source->display, sizeof(dest->display));
    dest->dirty = true;
  }
}

void iota_gfx_task_user(void) {
  struct CharacterMatrix matrix;
  matrix_clear(&matrix);
  matrix_render_user(&matrix);
  matrix_update(&display, &matrix);


  if (timer_elapsed32(timer) > 500) {
  	    timer = timer_read32();
  		if(underscore_visible == true){
  			strcpy(term_symbol, term_symbol_invisible);
  			underscore_visible = false;
  		}else{
  			strcpy(term_symbol, term_symbol_visible);
  			underscore_visible = true;
  		}

  }
}
#endif//SSD1306OLED

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  if (record->event.pressed) {
#ifdef SSD1306OLED
    set_keylog(keycode, record);
#endif
   // set_timelog();
  }

  switch (keycode) {
  	case KC_F13:
  	if (record->event.pressed) {
      if (keylogger_enabled == true) {
        keylogger_enabled = false;
      }else{
        keylogger_enabled = true;
       }
      }
      return false;
      break;
    case QWERTY:
      if (record->event.pressed) {
        set_single_persistent_default_layer(_QWERTY);
      }
      return false;
      break;
    case LOWER:
      if (record->event.pressed) {
        layer_on(_LOWER);

      } else {
        layer_off(_LOWER);

      }
      return false;
      break;
    case RAISE:
      if (record->event.pressed) {
        layer_on(_RAISE);

      } else {
        layer_off(_RAISE);

      }
      return false;
      break;

  }
  return true;
}
