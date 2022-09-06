#include "rfmgr_cl.h"

#include <Arduino.h>

using namespace ScoutBot_Controller;

RFManager_Client rfmgr_cl;

void setup() {
  Serial.begin(115200);
  rfmgr_cl.setup("Scout Bot Controller");
  rfmgr_cl.adv_scan_start(BLEUUID("4fafc201-1fb5-459e-8fcc-c5c9c331914b"), true, 1349, 449, 5);
}

void loop() {
  delay(100);
}