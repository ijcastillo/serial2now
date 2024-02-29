#ifndef ESPNOWCOMMS_H
#define ESPNOWCOMMS_H

#include <Arduino.h>

extern uint8_t macDir[6];   // dirección mac station de este dispositivo
extern uint8_t macApDir[6]; // dirección mac access point de este dispositivo

void configEspnow();
uint8_t* getMac();
uint8_t* getApMac();

#endif // ESPNOWCOMMS_H