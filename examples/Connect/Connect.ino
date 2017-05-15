#include <Arduino.h>
#include "Helium.h"

#if defined(ARDUINO_AVR_UNO)
#include <SoftwareSerial.h>
SoftwareSerial serial(8, 9);

#elif defined(ARDUINO_SAM_DUE)

#define serial Serial2

#elif defined(ARDUINO_SAMD_ZERO)

#include "wiring_private.h"

// Serial2 pin and pad definitions (in Arduino files Variant.h & Variant.cpp)
#define PIN_SERIAL2_TX       (6ul)              // Pin description number for PA20 on D6
#define PIN_SERIAL2_RX       (7ul)               // Pin description number for PA21 on D7
#define PAD_SERIAL2_TX       (UART_TX_PAD_2)     // SERCOM pad 2
#define PAD_SERIAL2_RX       (SERCOM_RX_PAD_3)   // SERCOM pad 3

// Instantiate the Serial2 class
Uart Serial2(&sercom3, PIN_SERIAL2_RX, PIN_SERIAL2_TX, PAD_SERIAL2_RX, PAD_SERIAL2_TX);

#define serial Serial2

#endif

void setup() {
    Serial.begin(9600);
    while (!Serial);
    Serial.println("Starting");

#if defined(ARDUINO_SAMD_ZERO)
    pinPeripheral(6, PIO_SERCOM_ALT);
    pinPeripheral(7, PIO_SERCOM_ALT);
#endif

    serial.begin(HELIUM_BAUD_RATE);
    Helium helium(&serial);

    Serial.println("Getting Info");
    struct res_info helium_info;
    if (carbon_info_OK != helium.info(&helium_info)) {
        Serial.println("ERROR: Failed getting info");
    }
    Serial.print("MAC: ");
    Serial.println((long)helium_info.mac, HEX);

    if (carbon_connect_CONNECTED != helium.connect()) {
        Serial.println("ERROR: Failed to connect");
    }

    /* const char *data = "HELLO"; */
    /* helium.send(data, strlen(data)); */
}

void loop() {
    while (1);
}
