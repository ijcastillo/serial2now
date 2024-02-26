/*
*   - La dirección dirección mac station será también el número de serie USB-CDC
*   - Para poder obtener la dirección mac access point WiFi.mode() debe incluir AP
*
*
*/


#include <WiFi.h>
#include "serialComms.h"
#include "espnow.h"
#include <esp_now.h>

uint8_t macDir[6] =   {0x00}; // dirección mac station de este dispositivo
uint8_t macApDir[6] = {0x00}; // dirección mac access point de este dispositivo

void initEspnow() {
    WiFi.mode(WIFI_AP_STA);
    memcpy(macDir, WiFi.macAddress(macDir), 6);           // dirección station mac de este dispositivo
    memcpy(macApDir, WiFi.softAPmacAddress(macApDir), 6); // dirección Access Point mac de este dispositivo
}

uint8_t* getMac() {
    return macDir;
};

uint8_t* getApMac() {
    return macApDir;    
};