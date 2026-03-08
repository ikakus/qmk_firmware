#include QMK_KEYBOARD_H
#include "pointing_device.h"

void keyboard_post_init_user(void) {
    debug_enable = true;
    debug_mouse = true;
}

report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {
    if (mouse_report.x != 0 || mouse_report.y != 0) {
        dprintf("x: %d, y: %d\n", mouse_report.x, mouse_report.y);
    }
    return mouse_report;
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT_ortho_1x1(KC_NO)
};
