#include "rfmgr_srv.h"
#include "payload.h"

Utils::LogManager g_log_mgr;
ScoutBot_Server::RFManager_Server g_rfmgr_srv;

void setup() {
    g_log_mgr.allocate(UART_NUM_0, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, 1024, 0, 115200);
    g_log_mgr.println("scout bot receiver v1.0");
    
    g_rfmgr_srv.setup("scout bot receiver v1.0");

    auto service = g_rfmgr_srv.add_service(BLEUUID(SERVICE_UUID));
    auto characteristic = service->add_characteristic(BLEUUID(CHARACTERISTIC_UUID), BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);
    
    // TODO: Add characteristics found in main.h, migrate the uuid definitions from main.h to config.h

    static uint32_t val = 123;
    characteristic->setValue(val);

    // TODO: Initialize the values for characteristics

    g_rfmgr_srv.adv_start();
    
    delay(100);
}

void loop() {
    auto service = g_rfmgr_srv.get_service(BLEUUID(SERVICE_UUID));
    auto characteristic = service->get_characteristic(BLEUUID(CHARACTERISTIC_UUID));
    
    // TODO: Process the characteristic IDs, potentially also add redundant protection for motors if BLE is not connected.

    g_log_mgr.println("dbg: characteristic value: " + std::to_string(Utils::payload_to_u32(characteristic->getData())));

    delay(5000);
}