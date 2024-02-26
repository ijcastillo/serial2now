#ifndef ESPNOWCOMMS_H
#define ESPNOWCOMMS_H

#include <Arduino.h>

void configEspnow();
uint8_t* getMac();
uint8_t* getApMac();

#endif // ESPNOWCOMMS_H