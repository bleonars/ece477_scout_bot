#include "rfmgr_srv.h"
#include "payload.h"
#include "main.h"
#include "bdc_motor.h"

Utils::LogManager                         g_log_mgr;
ScoutBot_Server::RFManager_Server         g_rfmgr_srv;

static std::array<bdc_motor_config_t, 4>  motor_cfg;
static ChassisControl                     chassis_ctrl; 

void setup_characteristics(ScoutBot_Server::RFManager_Service *receiver_service) {
    static float    motor_zero_duty   = 0.f; 
    static uint32_t range_enable_off  = 0;
    static uint32_t drive_mode_arcade = 0;

    auto left_joy_y_char = receiver_service->add_characteristic(BLEUUID(LEFT_JS_Y_CHARACTERISTIC_UUID), BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);
    left_joy_y_char->setValue(motor_zero_duty);

    auto right_joy_y_char = receiver_service->add_characteristic(BLEUUID(RIGHT_JS_Y_CHARACTERISTIC_UUID), BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);
    right_joy_y_char->setValue(motor_zero_duty);

    auto right_joy_x_char = receiver_service->add_characteristic(BLEUUID(RIGHT_JS_X_CHARACTERISTIC_UUID), BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);
    right_joy_x_char->setValue(motor_zero_duty);

    auto range_en_char = receiver_service->add_characteristic(BLEUUID(RANGE_ENABLE_CHARACTERISTIC_UUID), BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);
    range_en_char->setValue(range_enable_off);

    auto drive_mode_char = receiver_service->add_characteristic(BLEUUID(DRIVE_MODE_CHARACTERISTIC_UUID), BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);
    drive_mode_char->setValue(drive_mode_arcade);
}

void setup_motors() {
    motor_cfg[0].m_mcpwm_out_gpios[0] = 5; 
    motor_cfg[0].m_mcpwm_out_gpios[1] = 19; 
    motor_cfg[0].m_mcpwm_unit         = MCPWM_UNIT_0;
    motor_cfg[0].m_mcpwm_timer        = MCPWM_TIMER_0;
    bdc_motor_setup(&motor_cfg[0]);

    motor_cfg[1].m_mcpwm_out_gpios[0] = 19; 
    motor_cfg[1].m_mcpwm_out_gpios[1] = 21; 
    motor_cfg[1].m_mcpwm_unit         = MCPWM_UNIT_0;
    motor_cfg[1].m_mcpwm_timer        = MCPWM_TIMER_0;
    bdc_motor_setup(&motor_cfg[1]);
   
    motor_cfg[2].m_mcpwm_out_gpios[0] = 22; 
    motor_cfg[2].m_mcpwm_out_gpios[1] = 23; 
    motor_cfg[2].m_mcpwm_unit         = MCPWM_UNIT_1;
    motor_cfg[2].m_mcpwm_timer        = MCPWM_TIMER_1;
    bdc_motor_setup(&motor_cfg[2]);
    
    motor_cfg[3].m_mcpwm_out_gpios[0] = 26; 
    motor_cfg[3].m_mcpwm_out_gpios[1] = 27; 
    motor_cfg[3].m_mcpwm_unit         = MCPWM_UNIT_1;
    motor_cfg[3].m_mcpwm_timer        = MCPWM_TIMER_1;
    bdc_motor_setup(&motor_cfg[3]);
}

void setup() {
    g_log_mgr.allocate(UART_NUM_0, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, 1024, 0, 115200);
    g_log_mgr.println("scout bot receiver v1.0");

    g_rfmgr_srv.setup("scout bot receiver v1.0");
    
    auto receiver_service = g_rfmgr_srv.add_service(BLEUUID(SERVICE_UUID));
    setup_characteristics(receiver_service);

    g_rfmgr_srv.adv_start();
    
    setup_motors();
}

void loop() {
    auto receiver_service = g_rfmgr_srv.get_service(BLEUUID(SERVICE_UUID));
    
    float left_joy_y  = Utils::payload_to_f32(receiver_service->get_characteristic(BLEUUID(LEFT_JS_Y_CHARACTERISTIC_UUID))->getData());
    float right_joy_y = Utils::payload_to_f32(receiver_service->get_characteristic(BLEUUID(RIGHT_JS_Y_CHARACTERISTIC_UUID))->getData());
    float right_joy_x = Utils::payload_to_f32(receiver_service->get_characteristic(BLEUUID(RIGHT_JS_X_CHARACTERISTIC_UUID))->getData());
    
    uint32_t range_enbled = Utils::payload_to_u32(receiver_service->get_characteristic(BLEUUID(RANGE_ENABLE_CHARACTERISTIC_UUID))->getData());
    uint32_t drive_mode   = Utils::payload_to_u32(receiver_service->get_characteristic(BLEUUID(DRIVE_MODE_CHARACTERISTIC_UUID))->getData());

    uint32_t range = chassis_ctrl.get_range();

    /* based on range detection mode, active drive mode, and current joystick readings perform drive code actions        */
    /* notes; each joystick is read with [-100, 100] range. 0 reading indicates joystick is in the middle (no action)    */
    /*      ; write this value to motor_set_speed. remember, we have to arrange the motor direction correctly, so that   */
    /*      ; correct motors move backwards or forwards based on the direction we want to go... might have to negate the */
    /*      ; value read from joysticks accordingly                                                                      */
}