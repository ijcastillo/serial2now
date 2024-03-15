#ifndef ESPNOWCOMMS_H
#define ESPNOWCOMMS_H

#include <Arduino.h>

extern uint8_t macDir[6];       // dirección mac station de este dispositivo
extern uint8_t macApDir[6];     // dirección mac access point de este dispositivo
extern uint8_t macGwDir[6];     // dirección mac del dispositivo gateway al que estamos conectados
extern bool    gwLocated;       // si es device indica si conoce la MAC del gateway

void configEspnow(const uint8_t _espnowCh);
uint8_t* getMac();
uint8_t* getApMac();
void hello();
uint8_t getNumPairs();
uint8_t* getPairs();

#endif // ESPNOWCOMMS_H