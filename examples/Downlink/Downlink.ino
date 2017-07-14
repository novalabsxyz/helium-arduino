/*
 * Copyright 2017, Helium Systems, Inc.
 * All Rights Reserved. See LICENCE.txt for license information
 */

#include "Arduino.h"
#include "Board.h"
#include "Helium.h"

// NOTE: Please ensure you've created a channel called "Helium
// MQTT" called in the Helium Dashboard.
#define CHANNEL_NAME "Helium MQTT"

Helium  helium(&atom_serial);
Channel channel(&helium);

void
report_status(int status, int result = 0)
{
    if (helium_status_OK == status)
    {
        if (result == 0)
        {
            Serial.println("Succeeded");
        }
        else
        {
            Serial.print("Failed - ");
            Serial.println(result);
        }
    }
    else
    {
        Serial.println("Failed");
    }
}


void
connect()
{
    while (!helium.connected())
    {
        Serial.print("Connecting - ");
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
        Serial.print("Creating Channel - ");
        status = channel.begin(channel_name, &result);
        // Print status and result
        report_status(status, result);

    } while (helium_status_OK != status || result != 0);
}

void
channel_poll(void * data, size_t len, size_t * used)
{
    int    status;
    do
    {
        Serial.println("Polling - ");
        status = channel.poll_data(data, len, used);
        report_status(status);
    } while (helium_status_OK != status);
}

void
setup()
{
    Serial.begin(9600);
    Serial.println("Starting");

    // Begin communication with the Helium Atom
    // The baud rate differs per supported board
    // and is configured in Board.h
    helium.begin(HELIUM_BAUD_RATE);

    // Connect the Atom to the Helium Network
    connect();
    // and ensure the channel is created
    channel_create(CHANNEL_NAME);

    pinMode(LED_BUILTIN, OUTPUT);
}


#define CMD_LED_ON "on"
#define CMD_LED_OFF "off"

void
loop()
{
    char data[HELIUM_MAX_DATA_SIZE];
    size_t data_used;

    channel_poll(data, HELIUM_MAX_DATA_SIZE, &data_used);
    if (data_used > 0)
    {
        Serial.print("Command - ");
        if (strncmp(CMD_LED_ON, data, data_used))
        {
            digitalWrite(LED_BUILTIN, HIGH);
            Serial.println("ON");
        } else if (strncmp(CMD_LED_OFF, data, data_used))
        {
            digitalWrite(LED_BUILTIN, LOW);
            Serial.println("OFF");
        }
    }

    // Wait a while till the next poll
    delay(5000);
}
