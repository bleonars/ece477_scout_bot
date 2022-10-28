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
    /*
        g_log_mgr.println("\nright horz: " + std::to_string(joystick_read_horz(&right_joystick)));
        g_log_mgr.println("right vert: " + std::to_string(joystick_read_vert(&right_joystick)));
        g_log_mgr.println("right sel: " + std::to_string(joystick_read_sel(&right_joystick)));

        g_log_mgr.println("\nleft horz: " + std::to_string(joystick_read_horz(&left_joystick)));
        g_log_mgr.println("left vert: " + std::to_string(joystick_read_vert(&left_joystick)));
        g_log_mgr.println("left sel: " + std::to_string(joystick_read_sel(&left_joystick)));

        g_log_mgr.println("\nbutton 1 sel: " + std::to_string(button_read_sel(&buttons, 0)));
        g_log_mgr.println("button 2 sel: " + std::to_string(button_read_sel(&buttons, 1)));
        g_log_mgr.println("button 3 sel: " + std::to_string(button_read_sel(&buttons, 2)));
        g_log_mgr.println("button 4 sel: " + std::to_string(button_read_sel(&buttons, 3)));
    */
    
    auto service = g_rfmgr_cl.pair_service(BLEUUID(SERVICE_UUID));
    auto motor_1_duty_cycle_char = g_rfmgr_cl.pair_characteristic(service, BLEUUID(MOTOR_1_DUTY_UUID));
    auto joystick_vert_val = 100 * joystick_read_vert(&right_joystick);
    g_log_mgr.println("wrote value: " + std::to_string(joystick_vert_val));
    uint8_t *joystick_vert_payload = (uint8_t *) &joystick_vert_val;
    motor_1_duty_cycle_char->writeValue(joystick_vert_payload, sizeof(float));
}