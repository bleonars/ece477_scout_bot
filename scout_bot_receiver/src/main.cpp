#include "rfmgr_srv.h"
#include "chassis_ctrl.h"
#include "main.h"

Utils::LogManager                 g_log_mgr;
ScoutBot_Server::RFManager_Server g_rfmgr_srv;
ChassisControl                    g_chassis_ctrl;

void setup_ble_characteristics(ScoutBot_Server::RFManager_Service *receiver_service) {
    static float    motor_zero_duty   = 0.f; 
    static uint32_t range_enable_off  = 0;
    static uint32_t drive_mode_arcade = 0;

    auto right_joy_x_char = receiver_service->add_characteristic(BLEUUID(RIGHT_JS_X_CHARACTERISTIC_UUID), BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);
    right_joy_x_char->setValue(motor_zero_duty);

    auto right_joy_y_char = receiver_service->add_characteristic(BLEUUID(RIGHT_JS_Y_CHARACTERISTIC_UUID), BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);
    right_joy_y_char->setValue(motor_zero_duty);

    auto left_joy_x_char = receiver_service->add_characteristic(BLEUUID(LEFT_JS_X_CHARACTERISTIC_UUID), BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);
    left_joy_x_char->setValue(motor_zero_duty);

    auto left_joy_y_char = receiver_service->add_characteristic(BLEUUID(LEFT_JS_Y_CHARACTERISTIC_UUID), BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);
    left_joy_y_char->setValue(motor_zero_duty);

    auto range_en_char = receiver_service->add_characteristic(BLEUUID(RANGE_ENABLE_CHARACTERISTIC_UUID), BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);
    range_en_char->setValue(range_enable_off);

    auto drive_mode_char = receiver_service->add_characteristic(BLEUUID(DRIVE_MODE_CHARACTERISTIC_UUID), BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);
    drive_mode_char->setValue(drive_mode_arcade);
}

void setup() {
    g_log_mgr.allocate(UART_NUM_0, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, 1024, 0, 115200);
    g_log_mgr.println("scout bot receiver v1.0");

    g_chassis_ctrl.init_chassis();

    g_rfmgr_srv.setup("scout bot receiver v1.0");
    
    auto receiver_service = g_rfmgr_srv.add_service(BLEUUID(RECEIVER_SERVICE_UUID));
    setup_ble_characteristics(receiver_service);

    g_rfmgr_srv.adv_start();
}

void loop() {
    auto receiver_service = g_rfmgr_srv.get_service(BLEUUID(RECEIVER_SERVICE_UUID));

    /* based on range detection mode, active drive mode, and current joystick readings perform drive code actions        */
    /* notes; each joystick is read with [-100, 100] range. 0 reading indicates joystick is in the middle (no action)    */
    /*      ; write this value to motor_set_speed. remember, we have to arrange the motor direction correctly, so that   */
    /*      ; correct motors move backwards or forwards based on the direction we want to go... might have to negate the */
    /*      ; value read from joysticks accordingly                                                                      */
    
    if (g_chassis_ctrl.get_drive_mode(receiver_service) == DRIVE_MODE_ARCADE) {
        float left_y  = g_chassis_ctrl.get_jstick(receiver_service, JOYSTICK_LEFT_Y);
        float right_x = g_chassis_ctrl.get_jstick(receiver_service, JOYSTICK_RIGHT_X);
        
        /* no input, disable both sides of motors */
        if (left_y == 0.f && right_x == 0.f) {
            g_chassis_ctrl.stop_left_motors();
            g_chassis_ctrl.stop_right_motors();
        }
        /* no vertical input, only left & right */
        else if (left_y == 0.f) {
            g_chassis_ctrl.start_left_motors();
            g_chassis_ctrl.start_right_motors();

            g_chassis_ctrl.set_left_motors(-right_x);
            g_chassis_ctrl.set_right_motors(right_x);
        }
        /* no horizontal input, only forward & backwards */
        else if (right_x == 0.f) {
            g_chassis_ctrl.start_left_motors();
            g_chassis_ctrl.start_right_motors();

            g_chassis_ctrl.set_left_motors(left_y);
            g_chassis_ctrl.set_right_motors(left_y);
        }
        /* both horizontal input and vertical input, forward/backward & left/right */
        else {
            float left_out  = std::clamp(left_y + right_x, -100.f, 100.f);
            float right_out = std::clamp(left_y - right_x, -100.f, 100.f);

            if (left_out == 0.f) {
                g_chassis_ctrl.stop_left_motors();
            }
            else {
                g_chassis_ctrl.start_left_motors();
                g_chassis_ctrl.set_left_motors(left_out);
            }

            if (right_out == 0.f) {
                g_chassis_ctrl.stop_right_motors();
            }
            else {
                g_chassis_ctrl.start_right_motors();
                g_chassis_ctrl.set_right_motors(right_out);
            }
        }
    }
    else if (g_chassis_ctrl.get_drive_mode(receiver_service) == DRIVE_MODE_TANK) {
        float left_y  = g_chassis_ctrl.get_jstick(receiver_service, JOYSTICK_LEFT_Y);
        float right_y = g_chassis_ctrl.get_jstick(receiver_service, JOYSTICK_RIGHT_Y);
        
        /* no input, disable both sides of motors */
        if (left_y == 0.f && right_y == 0.f) {
            g_chassis_ctrl.stop_left_motors();
            g_chassis_ctrl.stop_right_motors();
        }
        /* no left input, disable left side of motors, start right side */
        else if (left_y == 0.f) {
            g_chassis_ctrl.stop_left_motors();
            g_chassis_ctrl.start_right_motors();
            
            g_chassis_ctrl.set_right_motors(right_y);
        }
        /* no right input, disable right side of motors, start left side */
        else if (right_y == 0.f) {
            g_chassis_ctrl.stop_right_motors();
            g_chassis_ctrl.start_left_motors();

            g_chassis_ctrl.set_left_motors(left_y);
        }
        /* left and right input, start both sides of motors */
        else {
            g_chassis_ctrl.start_right_motors();
            g_chassis_ctrl.start_left_motors();

            g_chassis_ctrl.set_left_motors(left_y);
            g_chassis_ctrl.set_right_motors(right_y);
        }
    }
}