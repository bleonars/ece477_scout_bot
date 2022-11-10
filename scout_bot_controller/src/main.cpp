#include "rfmgr_cl.h"
#include <main.h>
#include "controls.h"

Utils::LogManager g_log_mgr;
ScoutBot_Client::RFManager_Client g_rfmgr_cl;
joystick_config_t right_joystick;
joystick_config_t left_joystick;
button_config_t buttons;

void setup() {
    g_log_mgr.allocate(UART_NUM_0, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, 1024, 0, 115200);
    g_log_mgr.println("scout bot controller v1.0");

    joystick_setup(&right_joystick, JOYSTICK_R_VERT, JOYSTICK_R_HORZ, JOYSTICK_R_SEL);
    joystick_setup(&left_joystick, JOYSTICK_L_VERT, JOYSTICK_L_HORZ, JOYSTICK_L_SEL);
    button_setup(&buttons, BUTTON_1_SEL, BUTTON_2_SEL, BUTTON_3_SEL, BUTTON_4_SEL);

    g_rfmgr_cl.setup("scout bot controller v1.0");
    while (!g_rfmgr_cl.found()) {
        g_rfmgr_cl.adv_scan_start(BLEUUID(SERVICE_UUID), true, 1349, 449, 5);
        g_log_mgr.print("\n\n");
    }

    while (!g_rfmgr_cl.paired())
        g_rfmgr_cl.pair();
}

void loop() {
    auto service = g_rfmgr_cl.pair_service(BLEUUID(SERVICE_UUID));
    auto motor_1_duty_cycle_char = g_rfmgr_cl.pair_characteristic(service, BLEUUID(MOTOR_1_DUTY_UUID));
    auto joystick_vert_val = joystick_read_vert(&right_joystick);
    g_log_mgr.println("wrote value: " + std::to_string(joystick_vert_val));
    uint8_t *joystick_vert_payload = (uint8_t *) &joystick_vert_val;
    motor_1_duty_cycle_char->writeValue(joystick_vert_payload, sizeof(float));
}