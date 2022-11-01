#include "chassiscontrol.h"
#include "Arduino.h"
#include "bdc_motor.h"

std::uint32_t ChassisControl::set_left_motors(std::uint32_t voltage) {
    bdc_motor_config_t motor_1_cfg; // left front
    bdc_motor_config_t motor_2_cfg; // left back

    motor_1_cfg.m_mcpwm_out_gpios[0] = 15; 
    motor_1_cfg.m_mcpwm_out_gpios[1] = 18; 
    motor_1_cfg.m_mcpwm_unit         = MCPWM_UNIT_0;
    motor_1_cfg.m_mcpwm_timer        = MCPWM_TIMER_0;

    motor_2_cfg.m_mcpwm_out_gpios[0] = 19; 
    motor_2_cfg.m_mcpwm_out_gpios[1] = 21; 
    motor_2_cfg.m_mcpwm_unit         = MCPWM_UNIT_0;
    motor_2_cfg.m_mcpwm_timer        = MCPWM_TIMER_0;

    bdc_motor_setup(&motor_1_cfg);
    bdc_motor_setup(&motor_2_cfg);

    bdc_motor_drive(&motor_1_cfg);
    bdc_motor_drive(&motor_2_cfg);

    return INT_MAX;
}

std::uint32_t ChassisControl::set_right_motors(std::uint32_t voltage) {

    bdc_motor_config_t motor_3_cfg; // right front
    bdc_motor_config_t motor_4_cfg; // right back

    motor_3_cfg.m_mcpwm_out_gpios[0] = 22; 
    motor_3_cfg.m_mcpwm_out_gpios[1] = 23; 
    motor_3_cfg.m_mcpwm_unit         = MCPWM_UNIT_1;
    motor_3_cfg.m_mcpwm_timer        = MCPWM_TIMER_1;

    motor_4_cfg.m_mcpwm_out_gpios[0] = 26; 
    motor_4_cfg.m_mcpwm_out_gpios[1] = 27; 
    motor_4_cfg.m_mcpwm_unit         = MCPWM_UNIT_1;
    motor_4_cfg.m_mcpwm_timer        = MCPWM_TIMER_1;

    bdc_motor_setup(&motor_3_cfg);
    bdc_motor_setup(&motor_4_cfg);

    bdc_motor_drive(&motor_3_cfg);
    bdc_motor_drive(&motor_4_cfg);
    
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
