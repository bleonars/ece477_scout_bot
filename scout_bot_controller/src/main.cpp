#include "rfmgr_cl.h"

Utils::LogManager g_log_mgr;
ScoutBot_Client::RFManager_Client g_rfmgr_cl;

void setup() {
    g_log_mgr.allocate(UART_NUM_0, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, 1024, 0, 115200);
    g_log_mgr.println("scout bot controller v1.0");

    g_rfmgr_cl.setup("scout bot controller v1.0");
    while (!g_rfmgr_cl.found()) {
        g_rfmgr_cl.adv_scan_start(BLEUUID(SERVICE_UUID), true, 1349, 449, 5);
        g_log_mgr.print("\n\n");
    }

    while (!g_rfmgr_cl.paired())
        g_rfmgr_cl.pair();
}

void loop() {

}