/*
 * Copyright 2017, Helium Systems, Inc.
 * All Rights Reserved. See LICENCE.txt for license information
 */

#include "Arduino.h"
#include "Board.h"
#include "Helium.h"

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
setup()
{
    Serial.begin(9600);
    Serial.println("Starting");

    // Begin communication with the Helium Atom
    // The baud rate differs per supported board
    // and is configured in Board.h
    helium.begin(HELIUM_BAUD_RATE);

    // Connect the Atom to the Helium Network
    Serial.print("Connecting - ");
    int status = helium.connect();
    // Print status
    report_status(status);

    // Begin communicating with the channel. This should only need to
    // be done once.
    //
    // NOTE: Please ensure you've created a channel called "Helium
    // MQTT" called in the Helium Dashboard.
    int8_t result;
    Serial.print("Creating Channel - ");
    status = channel.begin("Helium MQTT", &result);
    // Print status and result
    report_status(status, result);
}

void
loop()
{
    const char * data = "Hello Helium!";

    // Send some data to the configured channel
    int8_t result;
    Serial.print("Sending - ");
    int    status = channel.send(data, strlen(data), &result);
    // Print status and result
    report_status(status, result);

    // Wait a while till the next time
    delay(5000);
}
