#include "quantum.h"
#include "pointing_device.h"
#include "analog.h"

extern bool           pmw3360_init_impl(void);
extern report_mouse_t pmw3360_get_report_impl(report_mouse_t report);
extern uint16_t       pmw3360_get_cpi_impl(void);
extern void           pmw3360_set_cpi_impl(uint16_t cpi);

static int16_t joy_center_x = JOYSTICK_CENTER;
static int16_t joy_center_y = JOYSTICK_CENTER;

void pointing_device_driver_init(void) {
    if (is_keyboard_left()) {
        setPinInputHigh(JOYSTICK_SW_PIN);
        // sample resting position to calibrate per-axis center
        int32_t sum_x = 0, sum_y = 0;
        for (uint8_t i = 0; i < 16; i++) {
            sum_x += analogReadPin(JOYSTICK_X_PIN);
            sum_y += analogReadPin(JOYSTICK_Y_PIN);
        }
        joy_center_x = (int16_t)(sum_x / 16);
        joy_center_y = (int16_t)(sum_y / 16);
    } else {
        pmw3360_init_impl();
    }
}

report_mouse_t pointing_device_driver_get_report(report_mouse_t report) {
    if (is_keyboard_left()) {
        int16_t raw_x = analogReadPin(JOYSTICK_X_PIN);
        int16_t raw_y = analogReadPin(JOYSTICK_Y_PIN);
        int16_t dx    = raw_x - joy_center_x;
        int16_t dy    = raw_y - joy_center_y;
        if (abs(dx) < JOYSTICK_DEADZONE) dx = 0;
        if (abs(dy) < JOYSTICK_DEADZONE) dy = 0;
        // quadratic: gentle push = slow scroll, full push = fast scroll
        int16_t my = dy == 0 ? 0 : abs(dy) - JOYSTICK_DEADZONE;
        report.x   = 0;
        report.y   = (mouse_xy_report_t)(dy > 0 ? (int32_t)my * my / 3072 : -(int32_t)my * my / 3072);
    } else {
        report = pmw3360_get_report_impl(report);
    }
    return report;
}

uint16_t pointing_device_driver_get_cpi(void) {
    if (is_keyboard_left()) return 0;
    return pmw3360_get_cpi_impl();
}

void pointing_device_driver_set_cpi(uint16_t cpi) {
    if (!is_keyboard_left()) pmw3360_set_cpi_impl(cpi);
}
