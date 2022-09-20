#include "rfmgr_cl.h"

Utils::LogManager g_log_mgr;
ScoutBot_Controller::RFManager_Client g_rfmgr_cl;

void setup() {
    g_log_mgr.allocate(UART_NUM_0, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, 512, 512, 115200);
    g_rfmgr_cl.setup("Scout Bot Controller");

    g_rfmgr_cl.adv_scan_start(BLEUUID(""), true, 1349, 449, 5);
}

void loop() {
    g_log_mgr.println("scout bot controller v1.0");
}
