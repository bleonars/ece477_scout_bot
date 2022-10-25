#include "rfmgr_srv.h"
#include "payload.h"

Utils::LogManager g_log_mgr;
ScoutBot_Server::RFManager_Server g_rfmgr_srv;

ChassisControl chassis();

void setup() {
    g_log_mgr.allocate(UART_NUM_0, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, 1024, 0, 115200);
    g_log_mgr.println("scout bot receiver v1.0");
    
    g_rfmgr_srv.setup("scout bot receiver v1.0");

    static uint32_t val = 0;
    auto service = g_rfmgr_srv.add_service(BLEUUID(SERVICE_UUID));
    auto characteristic = service->add_characteristic(BLEUUID(CHARACTERISTIC_UUID), BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);
    characteristic->setValue(123); // sanity check to make sure this is working
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

    delay(10);
}
