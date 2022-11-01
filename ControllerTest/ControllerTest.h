#ifndef CONTROLLER_TEST_H_
#define CONTROLLER_TEST_H_

#include <Arduino.h>
#include <math.h>
#include <string.h>

// Button LED Ports
#define L1 25
#define L2 12
#define L3 13
#define L4 14

// Button Ports
#define SW1 5
#define SW2 22
#define SW3 23
#define SW4 21

// Joystick Ports
#define J1X 35
#define J1Y 34
#define J2X 32
#define J2Y 33

// Joystick LED Ports
#define L1X 19
#define L2X 27
#define L1Y 18
#define L2Y 26

// Switch Related Variables
int state1;
int state2;
int state3;
int state4;
int last1 = LOW;
int last2 = LOW;
int last3 = LOW;
int last4 = LOW;

// Joystick Related Variables
int j1XR;
int j1YR;
int j2XR;
int j2YR;



int min(int a, int b);

void loop();


   

#endif