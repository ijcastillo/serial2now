#ifndef SERIAL_COMMS_H
#define SERIAL_COMMS_H

/* 
*   Definición de los opcodes usados en la comunicación serial
*/
#define TEST 				0x05U
#define ACK 				0x06U
#define NACK 				0x07U
#define TERMINADOR_MSG		0x10U

#define LED_ON              0x20U   // fuerza encendido o apagado del led

#define GET_MAC             0x30U   // devuelve la MAC station del dispositivo 
#define GET_APMAC           0x31U   // devuelve la MAC access point del dispositivo
#define GET_GW_MAC          0x32U   // devuelve la MAC del dispositivo gateway al que estamos conectados


#define SET_ROLE            0x40U   // configura el rol del dispositivo
#define GET_PAIRS           0x41U   // devuelve los pares registrados
#define AD_PAIR             0x42U   // TODO añade un nuevo par para ser registrado registrado

/*
*   Role del dispositivo:
*   - device:   dispositivo que únicamente se comunicará con el gateway
*   - gateway:  dispositivo que se comunica con todos los devices y se comunicará con el servidor web por serial
*/
enum Role { 
    device =  0,  // dispositivo que se comunicará únicamente con el gateway
    gateway,      // gateway ESP-NOW  a serial
};  

extern Role role;

void processComms();
void configSerial(const uint8_t _ledPin);

#endif  // define SERIAL_COMMS_H