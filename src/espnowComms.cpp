/*
*   - La dirección dirección mac station será también el número de serie USB-CDC
*   - Para poder obtener la dirección mac access point WiFi.mode() debe incluir AP
*
*
*/


#include <WiFi.h>
#include "serialComms.h"
#include "espnowComms.h"
#include <esp_now.h>

void configEspnow() {
    WiFi.mode(WIFI_STA);    //
    /* dirección station mac de este dispositivo */
    memcpy(macDir, WiFi.macAddress(macDir), 6);  
    
    /* dirección Access Point mac de este dispositivo, esta dirección sólo estará
       disponible si WiFi.mode( ) incluye Access Point */ 
    memcpy(macApDir, WiFi.softAPmacAddress(macApDir), 6);
}

uint8_t* getMac() {
    return macDir;
};

uint8_t* getApMac() {
    return macApDir;    
};