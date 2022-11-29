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
#ifdef CLIENT_DBG
    g_log_mgr.println("scout bot controller v1.0");
#endif

    joystick_setup(&right_joystick, JOYSTICK_R_VERT, JOYSTICK_R_HORZ, JOYSTICK_R_SEL);
    joystick_setup(&left_joystick, JOYSTICK_L_VERT, JOYSTICK_L_HORZ, JOYSTICK_L_SEL);
    button_setup(&buttons, BUTTON_1_SEL, BUTTON_2_SEL, BUTTON_3_SEL, BUTTON_4_SEL);

    g_rfmgr_cl.setup("scout bot controller v1.0");
    while (!g_rfmgr_cl.found()) {
        g_rfmgr_cl.adv_scan_start(BLEUUID(RECEIVER_SERVICE_UUID), true, 1349, 449, 5);
#ifdef CLIENT_DBG
        g_log_mgr.print("\n\n");
#endif
    }

    while (!g_rfmgr_cl.paired())
        g_rfmgr_cl.pair();
}

void loop() {
    auto receiver_service = g_rfmgr_cl.pair_service(BLEUUID(RECEIVER_SERVICE_UUID));

    float right_joy_x = joystick_read_horz(&right_joystick);
    uint8_t *right_joy_x_payload = (uint8_t *) &right_joy_x;
    receiver_service->getCharacteristic(RIGHT_JS_X_CHARACTERISTIC_UUID)->writeValue(right_joy_x_payload, sizeof(float));

    float right_joy_y = joystick_read_vert(&right_joystick);
    uint8_t *right_joy_y_payload = (uint8_t *) &right_joy_y;
    receiver_service->getCharacteristic(RIGHT_JS_Y_CHARACTERISTIC_UUID)->writeValue(right_joy_y_payload, sizeof(float));

    float left_joy_x = joystick_read_horz(&left_joystick);
    uint8_t *left_joy_x_payload = (uint8_t *) &left_joy_x;
    receiver_service->getCharacteristic(LEFT_JS_X_CHARACTERISTIC_UUID)->writeValue(left_joy_x_payload, sizeof(float));

    float left_joy_y = joystick_read_vert(&left_joystick);
    uint8_t *left_joy_y_payload = (uint8_t *) &left_joy_y;
    receiver_service->getCharacteristic(LEFT_JS_Y_CHARACTERISTIC_UUID)->writeValue(left_joy_y_payload, sizeof(float));
    
    static uint8_t drive_btn_last = BTN_UNPRESSED;
    static uint8_t range_btn_last = BTN_UNPRESSED;

    uint8_t range_btn_cur = button_read_sel(&buttons, 0);
    uint8_t drive_btn_cur = button_read_sel(&buttons, 1);
    
    static uint8_t range_enabled = 0;
    static uint8_t drive_mode    = 0;

    if (range_btn_cur == BTN_PRESSED && range_btn_last == BTN_UNPRESSED) {
        range_enabled = !range_enabled;
    } 
    
    if (drive_btn_cur == BTN_PRESSED && drive_btn_last == BTN_UNPRESSED) {
        drive_mode = !drive_mode;
    } 
    
    drive_btn_last = drive_btn_cur;
    range_btn_last = range_btn_cur;

    uint8_t *range_enabled_payload = (uint8_t *) &range_enabled;
    receiver_service->getCharacteristic(RANGE_ENABLE_CHARACTERISTIC_UUID)->writeValue(range_enabled_payload, sizeof(uint8_t));

    uint8_t *drive_mode_payload = (uint8_t *) &drive_mode;
    receiver_service->getCharacteristic(DRIVE_MODE_CHARACTERISTIC_UUID)->writeValue(drive_mode_payload, sizeof(uint8_t));
    
    delay(1);
}