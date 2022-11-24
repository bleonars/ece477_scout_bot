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
#ifdef SERVER_DBG
    g_log_mgr.println("scout bot receiver v1.0");
#endif

    g_chassis_ctrl.init_chassis();
    g_chassis_ctrl.start_motors();

    g_rfmgr_srv.setup("scout bot receiver v1.0");
    
    auto receiver_service = g_rfmgr_srv.add_service(BLEUUID(RECEIVER_SERVICE_UUID));
    setup_ble_characteristics(receiver_service);

    g_rfmgr_srv.adv_start();
}

void loop() {
    if (g_chassis_ctrl.get_connected()) {

        auto receiver_service = g_rfmgr_srv.get_service(BLEUUID(RECEIVER_SERVICE_UUID));
        
        if (g_chassis_ctrl.get_drive_mode(receiver_service) == DRIVE_MODE_ARCADE) {
            static float left_speed_arcade  = 0.f;
            static float right_speed_arcade = 0.f;

            float left_y  = g_chassis_ctrl.get_jstick(receiver_service, JOYSTICK_LEFT_Y);
            float right_x = -g_chassis_ctrl.get_jstick(receiver_service, JOYSTICK_RIGHT_X);

            left_speed_arcade  += g_chassis_ctrl.scale_input_to_speed(left_speed_arcade,  std::clamp(left_y + right_x, -100.f, 100.f));
            right_speed_arcade += g_chassis_ctrl.scale_input_to_speed(right_speed_arcade, std::clamp(left_y - right_x, -100.f, 100.f));
            
            g_chassis_ctrl.set_left_motors_speed(left_speed_arcade);
            g_chassis_ctrl.set_right_motors_speed(right_speed_arcade);
        }

        else if (g_chassis_ctrl.get_drive_mode(receiver_service) == DRIVE_MODE_TANK) {
            static float left_speed_tank  = 0.f;
            static float right_speed_tank = 0.f;

            float left_y  = g_chassis_ctrl.get_jstick(receiver_service, JOYSTICK_LEFT_Y);
            float right_y = g_chassis_ctrl.get_jstick(receiver_service, JOYSTICK_RIGHT_Y);
            
            left_speed_tank  += g_chassis_ctrl.scale_input_to_speed(left_speed_tank, left_y);
            right_speed_tank += g_chassis_ctrl.scale_input_to_speed(right_speed_tank, right_y);
            
            g_chassis_ctrl.set_left_motors_speed(left_speed_tank);
            g_chassis_ctrl.set_right_motors_speed(right_speed_tank);
        }
        
        delay(20);
    }
}