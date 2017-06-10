#include "Arduino.h"
#include "Board.h"
#include "Helium.h"

Helium helium(&atom_serial);
Channel azure(&helium);

void
setup()
{
    Serial.begin(9600);
    while (!Serial)
        ;
    Serial.println("Starting");

    helium.begin(HELIUM_BAUD_RATE);

    Serial.println("Connecting");
    if (helium_status_OK != helium.connect())
    {
        Serial.println("ERROR: Failed to connect");
    }
    else
    {
        Serial.println("Connected");
    }
}

void
loop()
{
    Serial.println("HELLO!");
    while (1)
        ;
}
