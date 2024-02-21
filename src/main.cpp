#include <Arduino.h>
#include "serialComms.h"

#define LED_PIN 8  // Pin del LED en la placa bridge serial-now

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  configSerial(LED_PIN);
}

void loop() {
  processComms(); // procesado comunicaciones serial
}


