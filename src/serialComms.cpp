#include <Arduino.h>
#include "serialComms.h"
#include "espnowComms.h"

#include <WiFi.h>       // TODO quitar esto

uint8_t rxBuff[130];    // buffer de recepción serial
uint8_t ledPin;

Role role = device;     // extern en main.cpp

enum Instrucciones { 
    none =      0,
    ledOn =     LED_ON,
    test =      TEST,
    getMAC =    GET_MAC,
    getAPMAC =  GET_APMAC,
    setRole =   SET_ROLE
}; 
Instrucciones instr_en_curso = none;

void configSerial(const uint8_t _ledPin)
{
    ledPin = _ledPin;   // recibimos el número de pin del led
}

void processComms(void)
{
    uint8_t opCode;
    if (Serial.available()) {
        if (instr_en_curso == none) {
           Serial.readBytes(&opCode, 1);    // leemos el 1er byte, será el opcode 
           instr_en_curso = (Instrucciones)opCode;
        } 
        /* ya hemos recibido el opcode, esperamos recibir el reso de la instrucción completa */
        switch (instr_en_curso) {
        case TEST:
            instr_en_curso = none;
            Serial.write(ACK);
            break;

        case LED_ON:
            if (Serial.available()) { // nos falta un byte para ejecutar la instrucción
                Serial.readBytes(rxBuff, 1);     
                digitalWrite(ledPin, rxBuff[0]);
                instr_en_curso = none;
                Serial.write(ACK);
            }
            break;

        case SET_ROLE:
            if (Serial.available()) { // nos falta un byte para ejecutar la instrucción
                Serial.readBytes(rxBuff, 1);
                role = (Role)rxBuff[0];     
                instr_en_curso = none;
                Serial.write(ACK);
            }
            break;

        case GET_MAC:     
            instr_en_curso = none;
            Serial.write(getMac(), 6);
            break;

        case GET_APMAC:     
            instr_en_curso = none;
            Serial.write(getApMac(), 6);
            break;        

        default:
            /* opcode desconocido */
            Serial.write(NACK);
                    // Serial.write(instr_en_curso);
            instr_en_curso = none;
            break;
        }
    }
}

