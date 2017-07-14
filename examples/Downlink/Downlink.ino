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
}


void
loop()
{
    // Create a buffer of the maximum Helium network packet size
    char   data[HELIUM_MAX_DATA_SIZE];
    size_t data_used;

    // Call the poll utility function below which pings the network to
    // ask for data.
    channel_poll(data, HELIUM_MAX_DATA_SIZE, &data_used);
    // Convert to a printable string
    data[data_used] = '\0';
    if (data_used > 0)
    {
        Serial.print("Data - ");
        Serial.println(data);
    }
}


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
    int status;
    do
    {
        Serial.print("Polling - ");
        // Poll the channel for some data for some time
        status = channel.poll_data(data, len, used);

        if (helium_status_OK_NO_DATA == status)
        {
            // The Helium Network will deliver pending channel data as
            // soon as it sees any channel messages from a Helium
            // Atom. In normal usecases this will be when the device
            // delivers some valuable data to a channel. We fake it
            // here by just sending some empty data to the channel to
            // get the network to trigger if there's anything out
            // there.
            channel.send(NULL, 0, (int8_t *)NULL);
        }

        report_status(status);
    } while (helium_status_OK != status);
}
