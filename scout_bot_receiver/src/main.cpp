#include "rfmgr_srv.h"
#include "payload.h"

Utils::LogManager g_log_mgr;
ScoutBot_Server::RFManager_Server g_rfmgr_srv;

void setup() {
    g_log_mgr.allocate(UART_NUM_0, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, 1024, 0, 115200);
    g_log_mgr.println("scout bot receiver v1.0");
    
    pinMode(2, OUTPUT);
    pinMode(15, OUTPUT);
}

void loop() {
    digitalWrite(2, HIGH);
    digitalWrite(15, LOW);
    delay(1000);
    digitalWrite(2, LOW);
    digitalWrite(15, HIGH);
    delay(1000);
}