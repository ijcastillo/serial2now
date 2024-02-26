#ifndef SERIAL_COMMS_H
#define SERIAL_COMMS_H

/* Definición de los opcodes usados en la comunicación serial */
#define TEST 				0x05U
#define ACK 				0x06U
#define NACK 				0x07U
#define TERMINADOR_MSG		0x10U

#define LED_ON              0x20U   // fuerza encendido o apagado del led

#define GET_MAC             0x30U   // devuelve la MAC del dispositivo bridge serial-now


void processComms();
void configSerial(const uint8_t _ledPin);

#endif  // define SERIAL_COMMS_H