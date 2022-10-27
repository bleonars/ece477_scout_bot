#include "bdc_motor.h"

void bdc_motor_setup(bdc_motor_config_t *bdc_config) {
    mcpwm_config_t     pwm_config;
    mcpwm_pin_config_t pin_config;
    
    if (bdc_config->m_mcpwm_unit == MCPWM_UNIT_0) {
        pin_config.mcpwm0a_out_num = bdc_config->m_mcpwm_out_gpios[0];
        pin_config.mcpwm0b_out_num = bdc_config->m_mcpwm_out_gpios[1];
    }
    
    else if (bdc_config->m_mcpwm_unit == MCPWM_UNIT_1) {
        pin_config.mcpwm1a_out_num = bdc_config->m_mcpwm_out_gpios[0];
        pin_config.mcpwm1b_out_num = bdc_config->m_mcpwm_out_gpios[1];
    }
    
    mcpwm_set_pin(bdc_config->m_mcpwm_unit, &pin_config);
    
    pwm_config.frequency    = VEX393_PWM_BASE_FREQ;
    pwm_config.cmpr_a       = 0;
    pwm_config.cmpr_b       = 0;
    pwm_config.duty_mode    = MCPWM_DUTY_MODE_0;
    pwm_config.counter_mode = MCPWM_UP_COUNTER;
    
    mcpwm_init(bdc_config->m_mcpwm_unit, bdc_config->m_mcpwm_timer, &pwm_config);
}

void bdc_motor_drive(bdc_motor_config_t *bdc_config) {
    mcpwm_start(bdc_config->m_mcpwm_unit, bdc_config->m_mcpwm_timer);
}

void bdc_motor_stop(bdc_motor_config_t *bdc_config) {
    mcpwm_stop(bdc_config->m_mcpwm_unit, bdc_config->m_mcpwm_timer);
}

void bdc_motor_set_speed(bdc_motor_config_t *bdc_config, float duty_cycle_percentage) {
    /*mcpwm_set_signal_low(bdc_config->m_mcpwm_unit, bdc_config->m_mcpwm_timer, MCPWM_GEN_A);*/
    /*mcpwm_set_signal_low(bdc_config->m_mcpwm_unit, bdc_config->m_mcpwm_timer, MCPWM_GEN_B);*/

    /* motor moves forward, with duty cycle = duty % */
    if (duty_cycle_percentage > 0) {
        mcpwm_set_duty(bdc_config->m_mcpwm_unit, bdc_config->m_mcpwm_timer, MCPWM_GEN_A, duty_cycle_percentage);
        mcpwm_set_duty(bdc_config->m_mcpwm_unit, bdc_config->m_mcpwm_timer, MCPWM_GEN_B, duty_cycle_percentage);
        mcpwm_set_duty_type(bdc_config->m_mcpwm_unit, bdc_config->m_mcpwm_timer, MCPWM_GEN_A, MCPWM_DUTY_MODE_1);
        mcpwm_set_duty_type(bdc_config->m_mcpwm_unit, bdc_config->m_mcpwm_timer, MCPWM_GEN_B, MCPWM_DUTY_MODE_0);
    }

     /* motor moves backward, with duty cycle = -duty % */
    else {
        mcpwm_set_duty(bdc_config->m_mcpwm_unit, bdc_config->m_mcpwm_timer, MCPWM_GEN_A, -1 * duty_cycle_percentage);
        mcpwm_set_duty(bdc_config->m_mcpwm_unit, bdc_config->m_mcpwm_timer, MCPWM_GEN_B, -1 * duty_cycle_percentage);
        mcpwm_set_duty_type(bdc_config->m_mcpwm_unit, bdc_config->m_mcpwm_timer, MCPWM_GEN_A, MCPWM_DUTY_MODE_0);
        mcpwm_set_duty_type(bdc_config->m_mcpwm_unit, bdc_config->m_mcpwm_timer, MCPWM_GEN_B, MCPWM_DUTY_MODE_1);
    }
}
