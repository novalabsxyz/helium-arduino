#ifndef BOARD_H
#define BOARD_H


#if defined(ARDUINO_AVR_UNO)
#include "SoftwareSerial.h"
SoftwareSerial atom_serial(8, 9);
#define HELIUM_BAUD_RATE helium_baud_b9600

#elif defined(ARDUINO_SAMD_ZERO)

#ifdef Serial5
// Arduino M0 Pro
#define atom_serial Serial5
#else
// Arduino Zero
#define atom_serial Serial1
#endif

#endif

#ifndef HELIUM_BAUD_RATE
#define HELIUM_BAUD_RATE helium_baud_b115200
#endif


#endif // BOARD_H
