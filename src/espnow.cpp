#include <WiFi.h>
#include "serialComms.h"
#include "espnow.h"

uint8_t macDir[6] = {0x00}; // dirección mac de este dispositivo

void initEspnow() {
    memcpy(macDir, getMac(), 6);
}

/* Devuelve la dirección mac de éste dispositivo */
uint8_t* getMac() {
    return WiFi.macAddress(macDir);
}