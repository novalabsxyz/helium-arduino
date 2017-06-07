#ifndef BOARD_H
#define BOARD_H

#if defined(ARDUINO_AVR_UNO)

#include <SoftwareSerial.h>
#define RX_PIN 8
#define TX_PIN 9
SoftwareSerial serial = SoftwareSerial(RX_PIN, TX_PIN);

#elif defined(ARDUINO_SAM_DUE)

#define serial Serial2

#elif defined(ARDUINO_SAMD_ZERO)

#define serial Serial5

#endif


#endif // BOARD_H
