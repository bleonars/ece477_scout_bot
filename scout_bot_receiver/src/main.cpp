#include "rfmgr_srv.h"
#include "payload.h"

#include "bdc_motor.h"

Utils::LogManager g_log_mgr;
ScoutBot_Server::RFManager_Server g_rfmgr_srv;
bdc_motor_config_t motor_a_cfg;

void setup() {
    g_log_mgr.allocate(UART_NUM_0, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, 1024, 0, 115200);
    g_log_mgr.println("scout bot receiver v1.0");

    g_rfmgr_srv.setup("scout bot receiver v1.0");

    auto service = g_rfmgr_srv.add_service(BLEUUID(SERVICE_UUID));
    auto motor_1_duty_cycle_char = service->add_characteristic(BLEUUID(MOTOR_1_DUTY_UUID), BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);
    static float motor_1_duty_cycle = 0;
    motor_1_duty_cycle_char->setValue(motor_1_duty_cycle);

    g_rfmgr_srv.adv_start();

    motor_a_cfg.m_mcpwm_out_gpios[0] = 15; 
    motor_a_cfg.m_mcpwm_out_gpios[1] = 2; 
    motor_a_cfg.m_mcpwm_unit         = MCPWM_UNIT_0;
    motor_a_cfg.m_mcpwm_timer        = MCPWM_TIMER_0;
    bdc_motor_setup(&motor_a_cfg);
    bdc_motor_drive(&motor_a_cfg);
}

void loop() {
    auto service = g_rfmgr_srv.get_service(BLEUUID(SERVICE_UUID));
    auto motor_1_duty_cycle_char = service->get_characteristic(BLEUUID(MOTOR_1_DUTY_UUID));
    g_log_mgr.println(std::to_string(Utils::payload_to_f32(motor_1_duty_cycle_char->getData())));
    bdc_motor_set_speed(&motor_a_cfg, Utils::payload_to_f32(motor_1_duty_cycle_char->getData()));
}