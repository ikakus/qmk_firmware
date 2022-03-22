/*
 * Copyright 2021 Quentin LEBASTARD <qlebastard@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include QMK_KEYBOARD_H

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

  [0] = LAYOUT_split_3x5_3(
      //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                                         KC_Y,    KC_U,    KC_I,    KC_O,   KC_P,
      //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      CTL_T(KC_A), KC_S,    KC_D,    KC_F,    KC_G,                                     KC_H,    KC_J,    KC_K,    KC_L, CTL_T(KC_SCLN),
      //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      SFT_T(KC_Z),    KC_X,    KC_C,    KC_V,    KC_B,                                  KC_N,    KC_M, KC_COMM,  KC_DOT, SFT_T(KC_SLSH),
      //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                        KC_LCTRL, LT(2, KC_SPC),KC_LALT ,         KC_LGUI, MO(1), KC_ENT
                                          //`--------------------------'  `--------------------------'
  ),

  [1] = LAYOUT_split_3x5_3(
      //,-----------------------------------------------------.                    ,-----------------------------------------------------.
           XXXXXXX, KC_1, KC_2, KC_3, XXXXXXX,                                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
      //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
           KC_GESC, KC_4, KC_5, KC_6, KC_0,                                         KC_LEFT, KC_DOWN, KC_UP, KC_RIGHT, KC_BSPACE,
      //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
           KC_LSHIFT, KC_7, KC_8, KC_9, XXXXXXX,                                     CG_TOGG , XXXXXXX, XXXXXXX, XXXXXXX, KC_RSHIFT,
      //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                        KC_LCTRL, MO(4),KC_LALT ,         KC_LGUI, KC_NO, KC_ENT
                                       //`--------------------------'  `--------------------------'
  ),

  [2] = LAYOUT_split_3x5_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      KC_GESC, XXXXXXX, XXXXXXX, XXXXXXX , XXXXXXX,                              KC_GRAVE, KC_MINS, KC_EQL, XXXXXXX , XXXXXXX ,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LCTRL, KC_TAB, XXXXXXX, XXXXXXX, XXXXXXX,                               XXXXXXX, KC_LBRC, KC_RBRC, XXXXXXX , KC_QUOT,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LSHIFT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                             XXXXXXX, KC_LPRN, KC_RPRN, XXXXXXX, KC_BSLS,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                        KC_LCTRL, KC_NO ,KC_LALT ,         KC_LGUI, MO(3), KC_ENT
                                      //`--------------------------'  `--------------------------'
  ),

  [3] = LAYOUT_split_3x5_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                              XXXXXXX, KC_PGDOWN, KC_PGUP, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      XXXXXXX, XXXXXXX, KC_MS_BTN2, KC_MS_BTN1, XXXXXXX,                        KC_MS_L, KC_MS_D, KC_MS_U, KC_MS_R, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                              XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                        KC_LCTRL, KC_NO ,KC_LALT ,         KC_LGUI, KC_NO, KC_ENT
                                      //`--------------------------'  `--------------------------'
  ),

  [4] = LAYOUT_split_3x5_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      XXXXXXX, KC_F1, KC_F2, KC_F3, KC_F10,                                     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LCTRL, KC_F4, KC_F5 , KC_F6, KC_F11,                                     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LSHIFT, KC_F7,KC_F8 ,KC_F9 , KC_F12,                                     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                        KC_LCTRL, KC_NO ,KC_LALT ,         KC_LGUI, KC_NO, KC_ENT
                                      //`--------------------------'  `--------------------------'
  )
};
