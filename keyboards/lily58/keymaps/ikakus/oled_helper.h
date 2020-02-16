#pragma once
#define _QWERTY_MAC 0
#define _QWERTY_LINUX 1
#define _LOWER 2
#define _RAISE 3


enum custom_keycodes {
  QWERTY_MAC,
  QWERTY_LINUX,
  LOWER,
  RAISE
};

void render_mod_state(uint8_t modifiers);

void render_layout_state(void);

void render_layer_state(void);

void render_status(void);

void render_logo(void);
