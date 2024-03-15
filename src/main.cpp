/*
*   Pendiente:
*       - mandar periódicamente hello si es device y no tiene registrado al gateway
*       - controlar el led en función de los estados
*       - por defecto la placa arranca como device, es necesario configurar el gateway por serial
*/

#include <Arduino.h>
#include "serialComms.h"
#include "espnowComms.h"

#include <WiFi.h>

#define LED_PIN   8  // Pin del LED
#define ESPNOW_CH 0  // canal ESP-NOW (0 a 14)

/* variables declaradas extern en espnowComms, se actualizan allí */
Role    role;           // papel que realiza este dispositivo: gateway, device...
uint8_t macDir[6] {};   // MAC de este dispositivo
uint8_t macApDir[6] {}; // MAC punto de acceso de este dispositivo. Sólo si el wifi es AP
uint8_t macGwDir[6] {}; // MAC del gateway, sólo si este dispositivo es device
bool    gwLocated;      // si es device indica si conoce la MAC del gateway

/* tarea que gestiona la conectividad de la red ESP-NOW */
static void espnowConnect(void *argp)
{
    static uint32_t delayMs; // periodo de ejecución de la tarea en ms en función del rol
    static uint8_t ledStatus = 0;

    for (;;) {
        switch (role) {
            case device:
                if (!gwLocated) {
                    hello();    // buscamos al gateway
                    ledStatus ^= 1; // si no tenemos gateway, el led parpadea
                    digitalWrite(LED_PIN, ledStatus);
                }
                else
                    digitalWrite(LED_PIN, 0); // tenemos gateway, el led se queda encendido
                break;

            case gateway:
                if (getNumPairs() == 0) {
                    ledStatus ^= 1; // no tenemos ningún device pareado, el led parpadea
                    digitalWrite(LED_PIN, ledStatus);
                }
                else
                    digitalWrite(LED_PIN, 0); // tenemos al menos un device pareado, el led se queda encendido
                break;            
        }
        role == gateway? delayMs = 2000 : delayMs = 1000;
        delay(delayMs);        
    }
}

void setup()
{
    role = device; // TODO dar persistencia a esta configuración para gateway
    Serial.begin(115200);
    pinMode(LED_PIN, OUTPUT);
    configSerial(LED_PIN);
    configEspnow(ESPNOW_CH);

    /* configuración RTOS */
    xTaskCreate(espnowConnect, "espnowConnect", 1024, NULL, 1, NULL);
}

void loop()
{
    processComms(); // procesado comunicaciones serial
}


// MACs:
// blanco COM16, MAC EC DA 3B 36 45 08
// negro  COM18, MAC EC DA 3B 36 10 24
