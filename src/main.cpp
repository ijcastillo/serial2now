/*
*
**/

#include <Arduino.h>
#include "serialComms.h"
#include "espnowComms.h"

#include <WiFi.h>

#define LED_PIN 8  // Pin del LED en la placa bridge serial-now

extern Role role;  // se actualiza desde serialComms

void setup() {
  role = device;
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  configSerial(LED_PIN);
  configEspnow();
}

void loop() {
  processComms(); // procesado comunicaciones serial

}


// MACs:
// 1: EC DA 3B 36 45 08
// 2: EC DA 3B 36 4D 38