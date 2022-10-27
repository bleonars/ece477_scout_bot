#include "rfmgr_cl.h"
#include "controls.h"

Utils::LogManager g_log_mgr;
ScoutBot_Client::RFManager_Client g_rfmgr_cl;
joystick_config_t right_joystick;
joystick_config_t left_joystick;
button_config_t buttons;

void setup() {
    g_log_mgr.allocate(UART_NUM_0, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, 1024, 0, 115200);
    g_log_mgr.println("scout bot controller v1.0");

    //g_rfmgr_cl.setup("scout bot controller v1.0");
    //while (!g_rfmgr_cl.found()) {
        //g_rfmgr_cl.adv_scan_start(BLEUUID(SERVICE_UUID), true, 1349, 449, 5);
        //g_log_mgr.print("\n\n");
    //}

    //while (!g_rfmgr_cl.paired())
        //g_rfmgr_cl.pair();
    
    joystick_setup(&right_joystick, JOYSTICK_R_VERT, JOYSTICK_R_HORZ, JOYSTICK_R_SEL);
    joystick_setup(&left_joystick, JOYSTICK_L_VERT, JOYSTICK_L_HORZ, JOYSTICK_L_SEL);
    button_setup(&buttons, BUTTON_1_SEL, BUTTON_2_SEL, BUTTON_3_SEL, BUTTON_4_SEL);
}

void loop() {
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
    // Arcade and Tank goes here later
    
    delay(2000);
}