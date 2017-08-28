/*
 * Copyright 2017, Helium Systems, Inc.
 * All Rights Reserved. See LICENCE.txt for license information
 */

#include "Arduino.h"
#include "Board.h"
#include "Helium.h"
#include "HeliumUtil.h"

// NOTE: Please ensure you've created a channel with the above
// CHANNEL_NAME as it's name.
#define CHANNEL_NAME "Helium MQTT"

Helium  helium(&atom_serial);
Channel channel(&helium);

void
setup()
{
    Serial.begin(9600);
    DBG_PRINTLN(F("Starting"));

    // Begin communication with the Helium Atom
    // The baud rate differs per supported board
    // and is configured in Board.h
    helium.begin(HELIUM_BAUD_RATE);

    // Connect the Atom to the Helium Network
    helium_connect(&helium);

    // Begin communicating with the channel. This should only need to
    // be done once. The HeliumUtil functions add simple retry logic
    // to re-create a channel if it disconnects.
    channel_create(&channel, CHANNEL_NAME);
}

void
loop()
{
    const char * data = "Hello Helium!";

    // Send data to channel
    channel_send(&channel, CHANNEL_NAME, data, strlen(data));

    // Wait about 5 seconds
    delay(5000);
}
