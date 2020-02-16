#include QMK_KEYBOARD_H
#include "oled_helper.h"
#include <stdio.h>
#ifdef PROTOCOL_LUFA
  #include "lufa.h"
  #include "split_util.h"
#endif
#ifdef SSD1306OLED
  #include "ssd1306.h"
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
    //SSD1306 OLED init, make sure to add #define SSD1306OLED in config.h
    #ifdef SSD1306OLED
        iota_gfx_init(!has_usb());   // turns on the display
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

    matrix_write_ln(matrix, read_layer_state());
     if(linux_layout_enabled == true){
    	matrix_write_ln(matrix, "Layout: LINUX");
    }else{
    	matrix_write_ln(matrix, "Layout: MAC");
    }
    //matrix_write_ln(matrix, "");
    matrix_write(matrix, term_symbol);
    matrix_write_ln(matrix, read_keylog());
   
  } else {
    matrix_write_ln(matrix, read_layer_state());
    matrix_write_ln(matrix, "");
    matrix_write(matrix, term_symbol);
    matrix_write_ln(matrix, read_keylog());
    
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
