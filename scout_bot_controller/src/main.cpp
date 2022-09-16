#include <Arduino.h>
#include "logmgr.h"

Utils::LogManager g_log_mgr;

void setup() {
    g_log_mgr.allocate(UART_NUM_0, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, 512, 512, 115200);
}

void loop() {
    g_log_mgr.println("scout bot controller v1.0");
}