#include <Arduino.h>
#include "Helium.h"
#include "Boards.h"

Helium helium(&serial);
Channel channel(&helium);

void setup() {
    Serial.begin(9600);
    while (!Serial);
    Serial.println("Starting");

#if defined (ARDUINO_AVR_UNO)
    pinMode(RX_PIN, INPUT);
    pinMode(TX_PIN, OUTPUT);
#endif

    serial.begin(9600);

    Serial.println("Getting Info");
    struct res_info helium_info;
    if (carbon_status_OK != helium.info(&helium_info)) {
        Serial.println("ERROR: Failed getting info");
    } else {
        Serial.println("GOT MAC");
    }

    /* Serial.print("MAC: "); */
    /* Serial.println((long)helium_info.mac, HEX); */

    if (carbon_status_OK != helium.connect()) {
        Serial.println("ERROR: Failed to connect");
    }

    if (carbon_status_OK != channel.begin("azure")) {
        Serial.println("ERROR: Failed to create channel");
    }
}

void loop() {
    const char *data = "{ \"temperature\": 17 }";
    if (carbon_status_OK != channel.send(data, strlen(data))) {
        Serial.println("ERROR: Failed to send on channel");
    }
    delay(500);
}
