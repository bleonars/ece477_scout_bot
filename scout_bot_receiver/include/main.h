#ifndef __MAIN_H__
#define __MAIN_H__

/* macros */
#define SERVER_DBG

/* ble macros */
#define SERVICE_UUID                     "77777777-7777-7777-7777-777777777777"
#define LEFT_JS_Y_CHARACTERISTIC_UUID    "00000000-0000-0000-0000-000000000001"
#define RIGHT_JS_Y_CHARACTERISTIC_UUID   "00000000-0000-0000-0000-000000000002"
#define RIGHT_JS_X_CHARACTERISTIC_UUID   "00000000-0000-0000-0000-000000000003"
#define RANGE_ENABLE_CHARACTERISTIC_UUID "00000000-0000-0000-0000-000000000004"
#define DRIVE_MODE_CHARACTERISTIC_UUID   "00000000-0000-0000-0000-000000000005"


/* freertos */
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

/* esp32 */

/* arduino */
#include "Arduino.h"

/* internal */
#include "logmgr.h"
#include "chassiscontrol.h"

#endif
