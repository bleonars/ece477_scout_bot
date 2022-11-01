#include "ControllerTest.h"

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  // Button Control
  pinMode(SW1, INPUT_PULLUP);
  pinMode(SW2, INPUT_PULLUP);
  pinMode(SW3, INPUT_PULLUP);
  pinMode(SW4, INPUT_PULLUP);

  // Button LEDs
  pinMode(L1, OUTPUT);
  pinMode(L2, OUTPUT);
  pinMode(L3, OUTPUT);
  pinMode(L4, OUTPUT);

  // Joystick LEDs
  pinMode(L1X, OUTPUT);
  pinMode(L1Y, OUTPUT);
  pinMode(L2X, OUTPUT);
  pinMode(L2Y, OUTPUT);

  Serial.println("Setup complete.");
}

int min(int a, int b){
  if (a < b){
    return a;
  }else{
    return b;
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  j1XR = analogRead(J1X);
  j2XR = analogRead(J2X);
  j1YR = analogRead(J1Y);
  j2YR = analogRead(J2Y);

  // Not refined for motor control, Joystick 1 X Axis
  if (j1XR < 75)
    digitalWrite(L1X, HIGH);
  else if (1000 < j1XR)
    digitalWrite(L1X, HIGH);
  else
    digitalWrite(L1X, LOW);  

  // Not refined for motor control, Joystick 1 Y Axis
  if (j1YR > 1050)
    digitalWrite(L1Y, HIGH);
  else if (j1YR < 50)
    digitalWrite(L1Y, HIGH);
  else
    digitalWrite(L1Y, LOW);

  // Not refined for motor control, Joystick 2 X Axis
  if (j2XR < 50)
    digitalWrite(L2X, HIGH);
  else if (j2XR > 1150)
    digitalWrite(L2X, HIGH);
  else
    digitalWrite(L2X, LOW);

  // Not refined for motor control, Joystick 2 Y Axis
  if (j2YR > 1150)
    digitalWrite(L2Y, HIGH);
  else if (j2YR < 50)
    digitalWrite(L2Y, HIGH);
  else
    digitalWrite(L2Y, LOW);

  // SW1
  state1 = digitalRead(SW1);
  if (last1 == LOW && state1 == HIGH){
    Serial.println("SW1 released.");
    digitalWrite(L1, LOW);
    delay(50);
  }
  else if (last1 == HIGH && state1 == LOW){
    Serial.println("SW1 pressed.");
    digitalWrite(L1, HIGH);
    delay(50);
  }
  last1 = state1;
  
  // SW2
  state2 = digitalRead(SW2);
  if (last2 == LOW && state2 == HIGH){
    Serial.println("SW2 released.");
    digitalWrite(L2, LOW);
    delay(50);
  }
  else if (last2 == HIGH && state2 == LOW){
    Serial.println("SW2 pressed.");
    digitalWrite(L2, HIGH);
    delay(50);
  }
  last2 = state2;

  
  // SW3
  state3 = digitalRead(SW3);
  
  if (last3 == LOW && state3 == HIGH){
    Serial.println("SW3 released.");
    digitalWrite(L3, LOW);
    delay(50);
  }
  else if (last3 == HIGH && state3 == LOW){
    Serial.println("SW3 pressed.");
    
    // Manual Value Print
    Serial.print("Value for joystick 1: x: ");
    Serial.print(j1XR);
    Serial.print(" y: ");
    Serial.println(j1YR);
    
    digitalWrite(L3, HIGH);
    delay(50);
  }
  
  last3 = state3;

  // SW4
  state4 = digitalRead(SW4);
  
  if (last4 == LOW && state4 == HIGH){
    Serial.println("SW4 released.");
    digitalWrite(L4, LOW);
    delay(50);
  }
  else if (last4 == HIGH && state4 == LOW){
    Serial.println("SW4 pressed.");
    
    // Manual Value Print
    Serial.print("Value for joystick 2: x: ");
    Serial.print(j2XR);
    Serial.print(" y: ");
    Serial.println(j2YR);
    
    digitalWrite(L4, HIGH);
    delay(50);
  }
  last4 = state4;
}
