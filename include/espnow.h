#ifndef ESPNOW_H
#define ESPNOW_H

#include <Arduino.h>

void initEspnow();
uint8_t* getMac();
uint8_t* getApMac();

#endif // ESPNOW_H