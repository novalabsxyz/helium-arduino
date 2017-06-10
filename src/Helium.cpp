#include "Arduino.h"
#include "helium-client/helium-client.h"
#include "Helium.h"

bool
helium_serial_readable(void * param)
{
    Stream * stream = (Stream *)param;
    return stream->available() > 0;
}

bool
helium_serial_getc(void * param, uint8_t * ch)
{
    Stream * stream = (Stream *)param;
    *ch             = stream->read();
    return *ch != -1;
}

bool
helium_serial_putc(void * param, uint8_t ch)
{
    Stream * stream = (Stream *)param;
    return stream->write(ch) == 1;
}

void
helium_wait_us(void * param, uint32_t us)
{
    (void)param;

#if defined(ARDUINO_ARCH_AVR)
#define MAX_DELAY_US 16384
    while (us > MAX_DELAY_US)
    {
        delayMicroseconds(MAX_DELAY_US);
        us -= MAX_DELAY_US;
    }
    delayMicroseconds(us);
#else
    delayMicroseconds(us);
#endif
}

Helium::Helium(HardwareSerial * serial)
{
    helium_init(&_ctx, (void *)serial);
}

#if defined(ARDUINO_AVR_UNO)
Helium::Helium(SoftwareSerial * serial)
{
    helium_init(&_ctx, (void *)serial);
}
#endif

int
Helium::begin(enum helium_baud baud)
{
#if defined(ARDUINO_AVR_UNO)
    SoftwareSerial * serial = (SoftwareSerial *)_ctx.param;
#else
    HardwareSerial * serial = (HardwareSerial *)_ctx.param;
#endif
    serial->begin(9600);

    int result = helium_baud(&_ctx, baud);

    uint32_t serial_baud = 9600;
    switch (baud)
    {
    case helium_baud_b9600:
        serial_baud = 9600;
        break;
    case helium_baud_b14400:
        serial_baud = 14400;
        break;
    case helium_baud_b19200:
        serial_baud = 19200;
        break;
    case helium_baud_b38400:
        serial_baud = 38400;
        break;
    case helium_baud_b57600:
        serial_baud = 57600;
        break;
    case helium_baud_b115200:
        serial_baud = 115200;
        break;
    }
    serial->begin(serial_baud);
    return result;
}

int
Helium::info(struct helium_info * info)
{
    return helium_info(&_ctx, info);
}

int
Helium::connect(struct connection * connection, uint32_t retries)
{
    return helium_connect(&_ctx, connection, retries);
}


bool
Helium::connected()
{
    return helium_connected(&_ctx) == helium_connected_CONNECTED;
}

int
Helium::sleep(struct connection * connection)
{
    return helium_sleep(&_ctx, connection);
}

//
// Channel
//

Channel::Channel(Helium * helium)
{
    _helium = helium;
}

int
Channel::begin(const char * name, uint16_t * token)
{
    return helium_channel_create(&_helium->_ctx, name, strlen(name), token);
}

int
Channel::begin(const char * name)
{
    uint16_t token;
    int      status = begin(name, &token);
    if (helium_status_OK == status)
    {
        status = poll(token, &_channel_id, HELIUM_POLL_RETRIES_5S);
    }
    return status;
}

int
Channel::send(void const * data, size_t len, uint16_t * token)
{
    return helium_channel_send(&_helium->_ctx, _channel_id, data, len, token);
}


int
Channel::send(void const * data, size_t len, int8_t * result)
{
    uint16_t token;
    int      status = send(data, len, &token);
    if (helium_status_OK == status)
    {
        status = poll(token, result, HELIUM_POLL_RETRIES_5S);
    }
    return status;
}

int Channel::poll(uint16_t token, int8_t * result, uint32_t retries)
{
    return helium_channel_poll(&_helium->_ctx, token, result, retries);
}
