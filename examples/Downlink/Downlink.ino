/*
 * Copyright 2017, Helium Systems, Inc.
 * All Rights Reserved. See LICENCE.txt for license information
 */

#include "Arduino.h"
#include "Board.h"
#include "Helium.h"
#include "HeliumUtil.h"

// NOTE: Please ensure you've created a channel called "Helium
// MQTT" called in the Helium Dashboard.
#define CHANNEL_NAME "Helium MQTT"

Helium  helium(&atom_serial);
Channel channel(&helium);


void
channel_poll(void * data, size_t len, size_t * used)
{
    int status;
    do
    {
        Serial.print("Polling - ");
        // Poll the channel for some data for some time
        status = channel.poll_data(data, len, used);
    } while (report_status(status) != helium_status_OK);
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
    helium_connect(&helium);
    // and ensure the channel is created
    channel_create(&channel, CHANNEL_NAME);
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
