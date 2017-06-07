#include "Helium.h"
#include "Arduino.h"

bool
carbon_serial_readable(void * param)
{
    Stream * serial = (Stream *)param;
    return serial->available() > 0;
}

bool
carbon_serial_getc(void * param, uint8_t * ch)
{
    Stream * serial = (Stream *)param;
    *ch             = serial->read();
    return *ch != -1;
}

bool
carbon_serial_putc(void * param, uint8_t ch)
{
    Stream * serial = (Stream *)param;
    return serial->write(ch) == 1;
}

void
carbon_wait_us(void * param, uint32_t us)
{
    (void)param;
    delayMicroseconds(us);
}


Helium::Helium(Stream * stream)
{
    carbon_init(&_ctx, (void *)stream);
}

int
Helium::connect()
{
    return carbon_connect(&_ctx, NULL);
}

int
Helium::info(struct res_info * info)
{
    return carbon_info(&_ctx, info);
}

bool
Helium::connected()
{
    return carbon_connected(&_ctx) == carbon_status_OK;
}

int
Helium::sleep()
{
    return carbon_sleep(&_ctx, NULL);
}

//
// Channel
//

Channel::Channel(Helium * helium)
{
    _helium = helium;
}

int
Channel::begin(const char * name)
{
    return carbon_channel_create(&_helium->_ctx, name, strlen(name), &_id);
}

int
Channel::send(void const * data, size_t len)
{
    uint8_t result;
    int     send_result =
        carbon_channel_send(&_helium->_ctx, _id, data, len, &result);
    if (send_result == carbon_status_OK)
    {
        return result;
    }
    else
    {
        return -1;
    }
}
