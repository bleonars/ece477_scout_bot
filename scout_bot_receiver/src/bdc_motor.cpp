#include "bdc_motor.h"

void bdc_motor_setup(bdc_motor_config_t *bdc_config) {
    
    if (bdc_config->m_mcpwm_unit == MCPWM_UNIT_0) {
        bdc_config->m_pin_config.mcpwm0a_out_num = bdc_config->m_mcpwm_out_gpios[0];
        bdc_config->m_pin_config.mcpwm0b_out_num = bdc_config->m_mcpwm_out_gpios[1];
    }
    
    else if (bdc_config->m_mcpwm_unit == MCPWM_UNIT_1) {
        bdc_config->m_pin_config.mcpwm1a_out_num = bdc_config->m_mcpwm_out_gpios[0];
        bdc_config->m_pin_config.mcpwm1b_out_num = bdc_config->m_mcpwm_out_gpios[1];
    }
    
    mcpwm_set_pin(bdc_config->m_mcpwm_unit, &bdc_config->m_pin_config);
    
    bdc_config->m_pwm_config.frequency    = VEX393_PWM_BASE_FREQ;
    bdc_config->m_pwm_config.cmpr_a       = 50.f;
    bdc_config->m_pwm_config.cmpr_b       = 50.f;
    bdc_config->m_pwm_config.duty_mode    = MCPWM_DUTY_MODE_0;
    bdc_config->m_pwm_config.counter_mode = MCPWM_UP_COUNTER;
    
    mcpwm_init(bdc_config->m_mcpwm_unit, bdc_config->m_mcpwm_timer, &bdc_config->m_pwm_config);

    bdc_motor_stop(bdc_config);
}

void bdc_motor_drive(bdc_motor_config_t *bdc_config) {
    mcpwm_start(bdc_config->m_mcpwm_unit, bdc_config->m_mcpwm_timer);
}

void bdc_motor_stop(bdc_motor_config_t *bdc_config) {
    bdc_motor_set_speed(bdc_config, 0.f);
    mcpwm_stop(bdc_config->m_mcpwm_unit, bdc_config->m_mcpwm_timer);
}

void bdc_motor_set_speed(bdc_motor_config_t *bdc_config, float duty_cycle_percentage) {
    duty_cycle_percentage = std::clamp(duty_cycle_percentage, -100.f, 100.f);

    if (abs(duty_cycle_percentage) < 5.f)
        duty_cycle_percentage = 0.f;

    /* motor stops, set all signals low */
    if (duty_cycle_percentage == 0.f) {
        mcpwm_set_signal_low(bdc_config->m_mcpwm_unit, bdc_config->m_mcpwm_timer, MCPWM_GEN_A);
        mcpwm_set_signal_low(bdc_config->m_mcpwm_unit, bdc_config->m_mcpwm_timer, MCPWM_GEN_B);
    }
    
    /* motor moves fowards */
    else if (duty_cycle_percentage > 0.f) {
        float scaled_duty_cycle = 50.f + 50.f * (duty_cycle_percentage / 100.f);

        mcpwm_set_duty(bdc_config->m_mcpwm_unit, bdc_config->m_mcpwm_timer, MCPWM_GEN_A, scaled_duty_cycle);
        mcpwm_set_duty(bdc_config->m_mcpwm_unit, bdc_config->m_mcpwm_timer, MCPWM_GEN_B, scaled_duty_cycle);

        mcpwm_set_duty_type(bdc_config->m_mcpwm_unit, bdc_config->m_mcpwm_timer, MCPWM_GEN_A, MCPWM_DUTY_MODE_0);
        mcpwm_set_duty_type(bdc_config->m_mcpwm_unit, bdc_config->m_mcpwm_timer, MCPWM_GEN_B, MCPWM_DUTY_MODE_1);
    }
    
    /* motor moves backwards */
    else if (duty_cycle_percentage < 0.f) {
        float scaled_duty_cycle = 50.f - 50.f * (-duty_cycle_percentage / 100.f);

        mcpwm_set_duty(bdc_config->m_mcpwm_unit, bdc_config->m_mcpwm_timer, MCPWM_GEN_A, scaled_duty_cycle);
        mcpwm_set_duty(bdc_config->m_mcpwm_unit, bdc_config->m_mcpwm_timer, MCPWM_GEN_B, scaled_duty_cycle);

        mcpwm_set_duty_type(bdc_config->m_mcpwm_unit, bdc_config->m_mcpwm_timer, MCPWM_GEN_A, MCPWM_DUTY_MODE_0);
        mcpwm_set_duty_type(bdc_config->m_mcpwm_unit, bdc_config->m_mcpwm_timer, MCPWM_GEN_B, MCPWM_DUTY_MODE_1);
    }
}
