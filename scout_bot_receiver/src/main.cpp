#include "rfmgr_srv.h"
#include "payload.h"
#include "main.h"
#include "bdc_motor.h"

Utils::LogManager g_log_mgr;
ScoutBot_Server::RFManager_Server g_rfmgr_srv;
bdc_motor_config_t motor_a_cfg;

ChassisControl chassis; 

void setup() {
    g_log_mgr.allocate(UART_NUM_0, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, 1024, 0, 115200);
    g_log_mgr.println("scout bot receiver v1.0");

    g_rfmgr_srv.setup("scout bot receiver v1.0");

    static uint32_t val = 0;
    static uint32_t check = 123;
    auto service = g_rfmgr_srv.add_service(BLEUUID(SERVICE_UUID));
    auto characteristic = service->add_characteristic(BLEUUID(CHARACTERISTIC_UUID), BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);
    characteristic->setValue(check); // sanity check to make sure this is working
    characteristic = service->add_characteristic(BLEUUID(LEFT_JS_Y_CHARACTERISTIC_UUID), BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);
    characteristic->setValue(val);
    characteristic = service->add_characteristic(BLEUUID(RIGHT_JS_Y_CHARACTERISTIC_UUID), BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);
    characteristic->setValue(val);
    characteristic = service->add_characteristic(BLEUUID(RIGHT_JS_X_CHARACTERISTIC_UUID), BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);
    characteristic->setValue(val);
    characteristic = service->add_characteristic(BLEUUID(RANGE_ENABLE_CHARACTERISTIC_UUID), BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);
    characteristic->setValue(val);
    characteristic = service->add_characteristic(BLEUUID(DRIVE_MODE_CHARACTERISTIC_UUID), BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);
    characteristic->setValue(val);

    g_rfmgr_srv.adv_start();

    delay(100);
    auto motor_1_duty_cycle_char = service->add_characteristic(BLEUUID(MOTOR_1_DUTY_UUID), BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);
    static float motor_1_duty_cycle = 0.f;
    motor_1_duty_cycle_char->setValue(motor_1_duty_cycle);

    g_rfmgr_srv.adv_start();
    
    motor_a_cfg.m_mcpwm_out_gpios[0] = 19; 
    motor_a_cfg.m_mcpwm_out_gpios[1] = 21; 
    motor_a_cfg.m_mcpwm_unit         = MCPWM_UNIT_0;
    motor_a_cfg.m_mcpwm_timer        = MCPWM_TIMER_0;
    bdc_motor_setup(&motor_a_cfg);
}

void loop() {
    auto service = g_rfmgr_srv.get_service(BLEUUID(SERVICE_UUID));
    
    // TODO: retrieve data from characteristic table
    auto characteristic = service->get_characteristic(BLEUUID(CHARACTERISTIC_UUID));
    uint32_t leftY = Utils::payload_to_u32(service->get_characteristic(BLEUUID(LEFT_JS_Y_CHARACTERISTIC_UUID))->getData());
    uint32_t rightY = Utils::payload_to_u32(service->get_characteristic(BLEUUID(RIGHT_JS_Y_CHARACTERISTIC_UUID))->getData());
    uint32_t rightX = Utils::payload_to_u32(service->get_characteristic(BLEUUID(RIGHT_JS_X_CHARACTERISTIC_UUID))->getData());
    
    bool rangeEnabled = Utils::payload_to_u32(service->get_characteristic(BLEUUID(RANGE_ENABLE_CHARACTERISTIC_UUID))->getData());
    bool driveMode = Utils::payload_to_u32(service->get_characteristic(BLEUUID(DRIVE_MODE_CHARACTERISTIC_UUID))->getData());

    uint32_t range = chassis.get_range();

    // TODO: Potentially also add redundant protection for motors if BLE is not connected.
    

    // g_log_mgr.println("dbg: characteristic value: " + std::to_string(Utils::payload_to_u32(characteristic->getData())));
    //
    auto motor_1_duty_cycle_char = service->get_characteristic(BLEUUID(MOTOR_1_DUTY_UUID));
    float joystick_data = Utils::payload_to_f32(motor_1_duty_cycle_char->getData());
    g_log_mgr.println(std::to_string(joystick_data));

    bdc_motor_drive(&motor_a_cfg);
    bdc_motor_set_speed(&motor_a_cfg, joystick_data);

    delay(10);
}
