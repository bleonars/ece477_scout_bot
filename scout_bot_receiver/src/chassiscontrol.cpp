#include "chassiscontrol.h"
#include "Arduino.h"

std::uint32_t ChassisControl::set_left_motors(std::uint32_t voltage) {
    return INT_MAX;
}

std::uint32_t ChassisControl::set_left_motors(std::uint32_t voltage) {
    return INT_MAX;
}

std::uint32_t ChassisControl::get_range() {
    return INT_MAX;
}

std::uint32_t ChassisControl::get_jstick(jstick_select_e_t jstick) {
    switch (jstick) {
        case JOYSTICK_RIGHT_X:
        case JOYSTICK_RIGHT_Y:
        case JOYSTICK_LEFT_Y:
        default: return INT_MAX;
    }
    return INT_MAX;
}

std::uint32_t ChassisControl::toggle_drive_mode() {
    arcade_enable = !arcade_enable;
}
