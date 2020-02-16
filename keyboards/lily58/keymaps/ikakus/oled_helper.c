#include QMK_KEYBOARD_H
#include "oled_helper.h"
#include <stdio.h>
#ifdef PROTOCOL_LUFA
  #include "lufa.h"
  #include "split_util.h"
#endif

extern uint8_t is_master;

static uint32_t timer = 0;
char term_symbol_visible [ ] = ">_ ";
char term_symbol_invisible [ ] = ">  ";
char term_symbol [] = "   ";
bool underscore_visible = true;
bool keylogger_enabled = false; 
bool linux_layout_enabled = false; 


void matrix_init_user(void) {
    strcpy(term_symbol, term_symbol_visible);
    timer = timer_read32();
}

//SSD1306 OLED update loop, make sure to add #define SSD1306OLED in config.h

// When add source files to SRC in rules.mk, you can use functions.
const char *read_layer_state(void);
const char *read_logo(void);
void set_keylog(uint16_t keycode, keyrecord_t *record);
const char *read_keylog(void);
const char *read_keylogs(void);

void oled_task_user(void) {
   render_layout_state();
   render_layer_state();
   render_mod_state(get_mods()|get_oneshot_mods());
}
void render_layer_state(void) {
  oled_write_P(PSTR("\nLayer:"), false);
  bool lower = layer_state_is(_LOWER);
  bool raise = layer_state_is(_RAISE);

  if(lower){ 
    oled_write_P(PSTR(" Lower "), true); 
  } else if(raise){ 
    oled_write_P(PSTR(" Raise "), true); 
  } else { 
    oled_write_P(PSTR(" Default"), false); 
  }
}

void render_mod_state(uint8_t modifiers) {
  oled_write_P(PSTR("\nMods: "), false);
  oled_write_P(PSTR("SHF"), (modifiers & MOD_MASK_SHIFT));
  oled_write_P(PSTR(" "), false);
  oled_write_P(PSTR("CTL"), (modifiers & MOD_MASK_CTRL));
  oled_write_P(PSTR(" "), false);
  oled_write_P(PSTR("ALT"), (modifiers & MOD_MASK_ALT));
  oled_write_P(PSTR(" "), false);
  oled_write_P(PSTR("GUI"), (modifiers & MOD_MASK_GUI));
}

void render_layout_state(void) {
  oled_write_P(PSTR("Layout: "), false);
  switch (biton32(default_layer_state)) {
      case _QWERTY_MAC:
        oled_write_P(PSTR("Mac"), false);
        break;
      case _QWERTY_LINUX:
        oled_write_P(PSTR("Linux"), false);
        break;
      default:
        oled_write_ln_P(PSTR("Undefined"), false);
  }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  if (record->event.pressed) {
    set_keylog(keycode, record);
   // set_timelog();
  }

  switch (keycode) {
  	case KC_F13:
  	if (record->event.pressed) {
      if (linux_layout_enabled == true) {
        linux_layout_enabled = false;
        set_single_persistent_default_layer(_QWERTY_MAC);
      }else{
        linux_layout_enabled = true;
        set_single_persistent_default_layer(_QWERTY_LINUX);
       }
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
