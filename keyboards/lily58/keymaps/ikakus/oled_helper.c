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

const char *read_layer_state(void);
const char *read_logo(void);
void set_keylog(uint16_t keycode, keyrecord_t *record);
const char *read_keylog(void);
const char *read_keylogs(void);

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
  if (is_master) {
    return OLED_ROTATION_0;
  }
  return OLED_ROTATION_180;
}

void oled_task_user(void) {
 	//strcpy(term_symbol, term_symbol_visible);
	timer = timer_read32();
	
	if (is_master) {
        render_status();
    } else {
        render_logo();
    }
	
}

void render_status(void){
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

void render_logo(void) {
  static const char PROGMEM qmk_logo[] = {
      0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f, 0x90, 0x91, 0x92, 0x93, 0x94,
      0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf, 0xb0, 0xb1, 0xb2, 0xb3, 0xb4,
      0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf, 0xd0, 0xd1, 0xd2, 0xd3, 0xd4,
      0};

  oled_write_P(qmk_logo, false);
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
