/*
 * Copyright 2017, Helium Systems, Inc.
 * All Rights Reserved. See LICENCE.txt for license information
 *
 * Taking humidity readings using the SEN0192 capacitive humidity
 * sensor.  Wiring instructions:
 * https://www.dfrobot.com/wiki/index.php/Capacitive_Soil_Moisture_Sensor_SKU:SEN0193
 *
 * Install the following libraries through Sketch->Manage Libraries:
 *     - ArduinoJson
 *     - Helium
 */


#include "Board.h"
#include <Arduino.h>
#include <ArduinoJson.h>
#include <HeliumUtil.h>
#include <Helium.h>
#include <Wire.h>


#define CHANNEL_NAME "Helium MQTT"
// Delay for one second
#define CHANNEL_DELAY 5000
// Send very 60 cycles (seconds)
#define CHANNEL_SEND_CYCLE 12

Helium  helium(&atom_serial);
Channel channel(&helium);
int     channel_counter;

void
setup()
{
    Serial.begin(9600);
    Serial.println(F("Starting"));

    helium.begin(HELIUM_BAUD_RATE); 
    // Connect the Atom to the Helium Network
    helium_connect(&helium);
    // and do a channel connect
    channel_create(&channel, CHANNEL_NAME);

    // Get the initial interval
    channel_counter = 0;
}


#define DRY_VALUE 536 // Taken in air
#define WET_VALUE 303 // Taken in water

#define HUM_RANGE (DRY_VALUE - WET_VALUE)

void
loop()
{
    Serial.print(F("Reading - "));

    float reading = analogRead(A0);
    float percent = 100 * (1 - (reading - WET_VALUE) / HUM_RANGE);
    Serial.print(reading);
    Serial.print(" - ");
    Serial.println(percent);

    if (--channel_counter <= 0)
    {
        DynamicJsonDocument doc(100);
        doc["value"] = reading;
        doc["percent"] = percent;
        char buffer[HELIUM_MAX_DATA_SIZE];
        size_t used = serializeJson(doc, buffer);

        channel_send(&channel, CHANNEL_NAME, buffer, used);

        channel_counter = CHANNEL_SEND_CYCLE;
    }

    delay(CHANNEL_DELAY);
}
