#include "quantum.h"
#include "pointing_device.h"
#include "drivers/sensors/pmw3360.h"
#include "drivers/sensors/pmw33xx_common.h"

bool pmw3360_init_impl(void) {
    return pmw33xx_init(0);
}

report_mouse_t pmw3360_get_report_impl(report_mouse_t report) {
    pmw33xx_report_t data = pmw33xx_read_burst(0);
    if (data.motion.b.is_motion) {
        report.x = (mouse_xy_report_t)data.delta_x;
        report.y = (mouse_xy_report_t)data.delta_y;
    }
    return report;
}

uint16_t pmw3360_get_cpi_impl(void) {
    return pmw33xx_get_cpi(0);
}

void pmw3360_set_cpi_impl(uint16_t cpi) {
    pmw33xx_set_cpi(0, cpi);
}
