/*
 * Copyright 2017, Helium Systems, Inc.
 * All Rights Reserved. See LICENCE.txt for license information
 */

#include "Arduino.h"
#include "Board.h"
#include "Helium.h"
#include "HeliumUtil.h"
#include "ArduinoJson.h"

// NOTE: This sample uses the "AWS IoT" channel which is available
// by in the Helium Dashboard. To use another channel, change the channel
// name to the target channel and adjust the Config variable name to match
// the cloud service's variable name
//
// ***AWS IoT*** Once the application is running on the device, change the global
// config variable "interval" in the Device Shadow.
//
// After one previous sleep cycle you should see the message arrive at
// the new interval rate (in milliseconds)
//
#define CHANNEL_NAME "YOUR CHANNEL NAME HERE"
#define CONFIG_INTERVAL_KEY "channel.interval"

Helium  helium(&atom_serial);
Channel channel(&helium);
Config  config(&channel);

int32_t send_interval;
String message = "Hello Helium!";

void
update_config(bool stale)
{
    if (stale)
    {
        DBG_PRINT(F("Fetching Config - "));
        int status = config.get(CONFIG_INTERVAL_KEY, &send_interval, 5000);
        report_status(status);

        if (status == helium_status_OK)
        {
            DBG_PRINT(F("Updating Config - "));
            status = config.set(CONFIG_INTERVAL_KEY, send_interval);
            report_status(status);
        }
    }
}

void
setup()
{
    Serial.begin(9600);
    DBG_PRINTLN(F("Starting"));

    // Begin communication with the Helium Atom The baud rate differs
    // per supported board and is configured in Board.h
    helium.begin(HELIUM_BAUD_RATE);

    // Connect the Atom to the Helium Network
    helium_connect(&helium);
    // and do a channel connect
    channel_create(&channel, CHANNEL_NAME);

    // Get the initial interval
    update_config(true);
}

void
loop()
{
    StaticJsonBuffer<JSON_OBJECT_SIZE(2) + 100> jsonBuffer;
    JsonObject & root = jsonBuffer.createObject();
    root[F("Interval")] = send_interval;
    
    int8_t result;
    char buffer[HELIUM_MAX_DATA_SIZE];
    size_t used = root.printTo(buffer, HELIUM_MAX_DATA_SIZE);    
    // Send data to channel

    int    status = channel.send(buffer, used, &result);
        // Print status and result
    update_config(config.is_stale());
    // Wait a while till the next time
    delay(send_interval);
}
