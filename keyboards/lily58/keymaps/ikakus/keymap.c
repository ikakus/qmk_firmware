#include QMK_KEYBOARD_H
#include <stdio.h>


#include "oled_helper.h"

extern keymap_config_t keymap_config;

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

[_QWERTY_MAC] = LAYOUT(
KC_GESC, KC_1, KC_2, KC_3, KC_4, KC_5,			 				KC_6, KC_7, KC_8, KC_9, KC_0, KC_BSPC,
KC_TAB, KC_Q, KC_W, KC_E, KC_R, KC_T, 			 				KC_Y, KC_U, KC_I, KC_O, KC_P, KC_BSLS,
KC_LGUI, KC_A, KC_S, KC_D, KC_F, KC_G, 			 				KC_H, KC_J, KC_K, KC_L, KC_SCLN, KC_QUOT,
KC_LSFT, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_LBRC,  		KC_LGUI, KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH, KC_RSFT,
KC_LCTL, KC_LALT, MO(3), KC_SPC, 								KC_ENT, MO(2), KC_RALT, KC_RCTL
),

[_QWERTY_LINUX] = LAYOUT(
KC_GESC, KC_1, KC_2, KC_3, KC_4, KC_5,			 				KC_6, KC_7, KC_8, KC_9, KC_0, KC_BSPC,
KC_TAB, KC_Q, KC_W, KC_E, KC_R, KC_T, 			 				KC_Y, KC_U, KC_I, KC_O, KC_P, KC_BSLS,
KC_LCTL, KC_A, KC_S, KC_D, KC_F, KC_G, 			 				KC_H, KC_J, KC_K, KC_L, KC_SCLN, KC_QUOT,
KC_LSFT, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_LBRC,  		KC_NO, KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH, KC_RSFT,
KC_LGUI, KC_LALT, MO(3), KC_SPC, 								KC_ENT, MO(2), KC_RALT, KC_RCTL
),
	
[_LOWER] = LAYOUT(
KC_TRNS, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,		 				KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, 
KC_TRNS, KC_F13, KC_NO, KC_NO, KC_NO, KC_NO, 					KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_F12,
KC_TRNS, KC_NO, KC_NO, KC_MRWD, KC_MFFD, KC_MPLY,   			KC_LEFT, KC_DOWN, KC_UP, KC_RGHT, KC_BSPC, KC_DEL,
KC_TRNS, KC_NO, KC_NO, KC_VOLD, KC_VOLU, KC_MUTE,KC_NO, 	KC_NO, KC_NO, KC_PGUP, KC_PGDOWN, KC_NO, KC_NO, KC_TRNS, 
KC_TRNS, KC_TRNS, KC_NO, KC_TRNS, 								KC_NO, KC_NO, KC_TRNS, KC_TRNS
),


[_RAISE] = LAYOUT(
KC_TRNS, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5,						KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, 
KC_NO,KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, 						KC_NO, KC_MINS, KC_EQL, KC_NO, KC_NO, KC_NO,
KC_TRNS, KC_NO, KC_NO, KC_MRWD, KC_MFFD, KC_MPLY,   			KC_NO, KC_LCBR, KC_RCBR, KC_NO, KC_NO, KC_NO,
KC_TRNS, KC_NO, KC_NO, KC_VOLD, KC_VOLU, KC_MUTE,KC_NO, 	KC_NO, KC_NO, KC_LBRC, KC_RBRC, KC_NO, KC_NO, KC_TRNS,
KC_NO, KC_NO, KC_NO, KC_NO, 									KC_TRNS, KC_NO, KC_TRNS, KC_TRNS
)

};

void encoder_update_user(uint8_t index, bool clockwise) {
   if (clockwise) {
            tap_code(KC_VOLD);
        } else {
            tap_code(KC_VOLU);
        }
}

