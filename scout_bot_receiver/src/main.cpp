#include "rfmgr_srv.h"
#include "payload.h"

#include "bdc_motor.h"

Utils::LogManager g_log_mgr;
ScoutBot_Server::RFManager_Server g_rfmgr_srv;
bdc_motor_config_t motor_a_cfg;

void setup() {
    g_log_mgr.allocate(UART_NUM_0, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, 1024, 0, 115200);
    g_log_mgr.println("scout bot receiver v1.0");

    motor_a_cfg.m_mcpwm_out_gpios[0] = 2; 
    motor_a_cfg.m_mcpwm_out_gpios[1] = 4; 
    motor_a_cfg.m_mcpwm_unit         = MCPWM_UNIT_0;
    motor_a_cfg.m_mcpwm_timer        = MCPWM_TIMER_0;
    
    bdc_motor_setup(&motor_a_cfg);
    bdc_motor_drive(&motor_a_cfg);
    bdc_motor_set_speed(&motor_a_cfg, 20);
}

void loop() {
}