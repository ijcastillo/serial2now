#ifndef SERIAL_COMMS_H
#define SERIAL_COMMS_H

/* Definición de los opcodes usados en la comunicación serial */
#define TEST 				0x05U
#define ACK 				0x06U
#define NACK 				0x07U
#define TERMINADOR_MSG		0x10U

#define LEDSTATUS           0x20U   // fuerza encendido o apagado del led

void processComms(void);
void configSerial(const uint8_t _ledPin);

#endif  // define SERIAL_COMMS_H