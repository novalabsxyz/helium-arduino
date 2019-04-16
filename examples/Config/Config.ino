/*
 * Copyright 2017, Helium Systems, Inc.
 * All Rights Reserved. See LICENCE.txt for license information
 */

#include "Arduino.h"
#include "Board.h"
#include "Helium.h"
#include "HeliumUtil.h"
#include "ArduinoJson.h"

// NOTE: This sample uses the "Helium MQTT" channel which is available
// by default in the Helium Dashboard.
//
// Once the application is running on the device change the global
// config variable "interval_ms" in the Config section of the Atom you
// are running this code on.
//
// After one previous sleep cycle you should see the message arrive at
// the new interval rate (in milliseconds)
//
// You should also see the updated configuration arrive in the serial
// console as it prints out a "Fetching Config" message when the
// configuration change is noticed.
//
// Install the ArduinoJson library before running through Sketch->Include 
// Libraries->Manage Libraries search for ArduinoJson and click Install. 
// Then restart the Arduino IDE.
//
// For device/thing/config shadows interface on Azure/AWS/Google 
// or any channel that supports configuration, the channel configuration 
// variable format is "channel.<variable name>" as shown below

#define CHANNEL_NAME "Helium MQTT"
// #define CONFIG_INTERVAL_KEY "channel.interval_ms" 
#define CONFIG_INTERVAL_KEY "config.interval_ms"

Helium  helium(&atom_serial);
Channel channel(&helium);
Config  config(&channel);

int32_t send_interval;

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
    DynamicJsonDocument doc(35);
    doc["interval"] = send_interval;
    char buffer[HELIUM_MAX_DATA_SIZE];
    size_t used = serializeJson(doc, buffer);
    // Send data to channel
    channel_send(&channel, CHANNEL_NAME, buffer, used);
    // Print status and result
    update_config(config.is_stale());
    // Wait a while till the next time
    delay(send_interval);
}
