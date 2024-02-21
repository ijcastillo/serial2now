#include <Arduino.h>
#include "serialComms.h"

uint8_t rxBuff[130];    // buffer de recepción serial
uint8_t ledPin;

enum Instrucciones { 
    none =      0,
    ledStatus = LEDSTATUS,
    test =      TEST
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
        case LEDSTATUS:
            if (Serial.available()) { // nos falta un byte para ejecutar la instrucción
                Serial.readBytes(rxBuff, 1);     
                digitalWrite(ledPin, rxBuff[0]);
                instr_en_curso = none;
                Serial.write(ACK);
            }
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

