/*
 * Copyright 2017, Helium Systems, Inc.
 * All Rights Reserved. See LICENCE.txt for license information
 */

#ifndef BOARD_H
#define BOARD_H


#if defined(ARDUINO_AVR_UNO)
#include "SoftwareSerial.h"
SoftwareSerial atom_serial(8, 9);
#define HELIUM_BAUD_RATE helium_baud_b9600

#elif defined(ARDUINO_AVR_UNO_WIFI_REV2)
#include "SoftwareSerial.h"
SoftwareSerial atom_serial(8, 9);
#define HELIUM_BAUD_RATE helium_baud_b9600

#elif defined(ARDUINO_AVR_MEGA2560)
#include "SoftwareSerial.h"
SoftwareSerial atom_serial(10,11);
#define HELIUM_BAUD_RATE helium_baud_b9600 

#elif defined(ARDUINO_SAM_ZERO)
// Arduino M0 Pro
#define atom_serial Serial5

#elif defined(ARDUINO_SAMD_ZERO)
// Arduino Zero
#define atom_serial Serial1

#elif defined(ARDUINO_SAM_DUE)
// Arduino Due with Serial3 (pin 15, 14)
// mapped to pin 8, 9 on the adapter
#define atom_serial Serial3
#endif

#if defined(CORE_TEENSY)
//Teensy with Serial1 (pin 0, 1)
#define atom_serial Serial1
extern "C"{

    int _write(int f, char *ptr,int len){
        int i;
        for(i=0;i<len;i++)
            {
                atom_serial.write(*ptr++);
            }
        return len;
    }
    int _read (int f, char *ptr, int len)
    {
        *ptr=atom_serial.read();  
        
        return len;
    }
}

#elif defined(ARDUINO_AVR_PRO)
//ProMini/Micro with Serial pins (8,9)
#include "SoftwareSerial.h"
SoftwareSerial atom_serial(8,9);

#endif

#ifndef HELIUM_BAUD_RATE
#define HELIUM_BAUD_RATE helium_baud_b115200
#endif

#endif // BOARD_H
