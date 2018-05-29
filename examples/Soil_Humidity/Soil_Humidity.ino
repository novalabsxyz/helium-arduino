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
connect()
{
    while (!helium.connected())
    {
        Serial.print(F("Connecting - "));
        int status = helium.connect();
        report_status(status);
        if (helium_status_OK != status)
        {
            delay(1000);
        }
    }
}

void
channel_create(const char * channel_name)
{
    int8_t result;
    int    status;
    do
    {
        // Ensure we're connected
        connect();
        Serial.print(F("Creating Channel - "));
        status = channel.begin(channel_name, &result);
        // Print status and result
        report_status(status, result);
        if (helium_status_OK != status)
        {
            delay(1000);
        }
    } while (helium_status_OK != status || result != 0);
}

void
channel_send(const char * channel_name, void const * data, size_t len)
{
    int    status;
    int8_t result;

    do
    {
        // Try to send
        Serial.print(F("Sending - "));
        status = channel.send(data, len, &result);
        report_status(status, result);
        // Create the channel if any service errors are returned
        if (status == helium_status_OK && result != 0)
        {
            channel_create(channel_name);
        }
        else if (status != helium_status_OK)
        {
            delay(1000);
        }
    } while (helium_status_OK != status || result != 0);
}


void
setup()
{
    Serial.begin(9600);
    Serial.println(F("Starting"));

    helium.begin(HELIUM_BAUD_RATE);
    channel_create(CHANNEL_NAME);
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
        StaticJsonBuffer<JSON_OBJECT_SIZE(2) + 100> jsonBuffer;
        JsonObject & root = jsonBuffer.createObject();

        root[F("value")]    = reading;
        root[F("percent")] = percent;

        char   buffer[HELIUM_MAX_DATA_SIZE];
        size_t used = root.printTo(buffer, HELIUM_MAX_DATA_SIZE);

        channel_send(CHANNEL_NAME, buffer, used);

        channel_counter = CHANNEL_SEND_CYCLE;
    }

    delay(CHANNEL_DELAY);
}
