#ifndef __MAIN_H__
#define __MAIN_H__

/* macros */
//#define CLIENT_DBG
#define SERVICE_UUID "77777777-7777-7777-7777-777777777777"
#define MOTOR_1_DUTY_UUID "00000000-0000-0000-0000-000000000001"

/* freertos */
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

/* esp32 */
#include <ESP32Servo.h>

/* arduino */
#include "Arduino.h"

/* internal */
#include "logmgr.h"

#endif