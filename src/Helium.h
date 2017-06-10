#ifndef HELIUM_H
#define HELIUM_H

#include "Arduino.h"
#include "helium-client/helium-client.h"

#if defined(ARDUINO_AVR_UNO)
#include <SoftwareSerial.h>
#endif

class Helium
{
  public:
    Helium(HardwareSerial * serial);

#if defined(ARDUINO_AVR_UNO)
    Helium(SoftwareSerial * serial);
#endif

    int begin(enum helium_baud baud);
    int info(struct helium_info * info);

    int connect(struct connection * connection, uint32_t retries);
    int connect()
    {
        return connect(NULL, HELIUM_POLL_RETRIES_5S);
    }

    bool connected();
    int sleep(struct connection * connection);
    int  sleep()
    {
        return sleep(NULL);
    }
    int poll(void * data, const size_t len, size_t used, uint32_t retries);

  private:
    struct helium_ctx _ctx;

    friend class Channel;
};

class Channel
{
  public:
    Channel(Helium * helium);

    int begin(const char * name);
    int send(void const * data, size_t len, int8_t * result);

    int begin(const char * name, uint16_t * token);
    int send(void const * data, size_t len, uint16_t * token);
    int poll(uint16_t token, int8_t * result, uint32_t retries);

  private:
    Helium * _helium;
    int8_t   _channel_id;
};

#endif // HELIUM_H
