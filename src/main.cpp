/*
*
**/

#include <Arduino.h>
#include "serialComms.h"
#include "espnow.h"

#include <WiFi.h>

#define LED_PIN 8  // Pin del LED en la placa bridge serial-now

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  configSerial(LED_PIN);
  initEspnow();

  // WiFi.mode(WIFI_STA); // TODO poner lo que corresponda
}

void loop() {
  processComms(); // procesado comunicaciones serial
}


// MACs:
// 1: EC DA 3B 36 45 08
// 2: EC DA 3B 36 4D 38