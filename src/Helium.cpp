/*
 * Copyright 2017, Helium Systems, Inc.
 * All Rights Reserved. See LICENCE.txt for license information
 */

#include "Helium.h"
#include "Arduino.h"
#include "helium-client/helium-client.h"

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

bool
Helium::needs_reset()
{
    return helium_needs_reset(&_ctx);
}

int
Helium::reset()
{
    return helium_reset(&_ctx);
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
Channel::begin(const char * name, int8_t * result)
{
    uint16_t token;
    int      status = begin(name, &token);
    _channel_id     = -1;
    if (helium_status_OK == status)
    {
        status = poll_result(token, &_channel_id);
    }

    if (result)
    {
        *result = status == helium_status_OK && _channel_id > 0 ? 0 : _channel_id;
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
        status = poll_result(token, result);
    }
    return status;
}

int
Channel::poll_result(uint16_t token, int8_t * result, uint32_t retries)
{
    return helium_channel_poll_result(&_helium->_ctx, token, result, retries);
}

int
Channel::poll_data(void * data, size_t len, size_t * used, uint32_t retries)
{
    return helium_channel_poll_data(&_helium->_ctx,
                                    _channel_id,
                                    data,
                                    len,
                                    used,
                                    retries);
}

//
// Config
//

Config::Config(Channel * channel)
{
    _channel = channel;
}

int
Config::get(const char * key, uint16_t * token)
{
    return helium_channel_config_get(&_channel->_helium->_ctx,
                                     _channel->_channel_id,
                                     key,
                                     token);
}


int
Config::_get(const char *            config_key,
             enum helium_config_type config_type,
             void *                  value,
             size_t                  value_len,
             void *                  default_value,
             size_t                  default_value_len,
             uint32_t                retries)
{
    uint16_t token;
    int      status = get(config_key, &token);
    if (helium_status_OK == status)
    {
        status = poll_get_result(token,
                                 config_key,
                                 config_type,
                                 value,
                                 value_len,
                                 default_value,
                                 default_value_len);
    }
    return status;
}


/** Context for the poll_get handlder
 *
 * An instance of this if filled in the #Config::poll_get_result
 * implementation and the result of the poll handler is returned in
 * the status field
 */
struct _poll_get_context
{
    //! The config key to look for
    const char * filter_key;
    //! The config type to check for
    enum helium_config_type filter_type;
    //! The destination buffer
    void * dest;
    //! The length of the destination buffer
    size_t dest_len;
    //! The default value. Assumed to be the same type as filter_type
    void * default_value;
    //! The length of the default_value
    size_t default_value_len;
    //! The result status of the result handler
    enum config_poll_get_status status;
};


static bool
_poll_get_result_handler(void *                  handler_ctx,
                         const char *            key,
                         enum helium_config_type value_type,
                         void *                  value)
{
    struct _poll_get_context * ctx = (struct _poll_get_context *)handler_ctx;
    size_t                     len = ctx->dest_len;
    void *                     src = value;

    if (strcmp(ctx->filter_key, key) != 0)
    {
        // Not the right key, keep going
        return true;
    }
    if (value_type == ctx->filter_type)
    {
        // Found and the right type
        // Use the given value and the destination buffer size
        ctx->status = config_status_POLL_FOUND;
    }
    else
    {
        // Found but not the right type, return an error
        ctx->status = config_status_POLL_ERR_TYPE;
        // And use the context's default
        value_type = ctx->filter_type;
        // Us the default value as the source
        src = ctx->default_value;
        // Check for a shorter default value length (only really valid for
        // strings)
        len = len > ctx->default_value_len ? ctx->default_value_len : len;
    }

    switch (value_type)
    {
    case helium_config_bool:
    case helium_config_f32:
    case helium_config_i32:
    case helium_config_str:
        break;
    case helium_config_null:
        ctx->status = config_status_POLL_FOUND_NULL;
        len         = 0;
        break;
    }
    memcpy(ctx->dest, src, len);
    return false;
}

int
Config::poll_get_result(uint16_t           token,
                        const char *       config_key,
                        helium_config_type config_type,
                        void *             value,
                        size_t             value_len,
                        void *             default_value,
                        size_t             default_value_len,
                        uint32_t           retries)
{
    struct _poll_get_context handler_ctx = {
        .filter_key        = config_key,
        .filter_type       = config_type,
        .dest              = value,
        .dest_len          = value_len,
        .default_value     = default_value,
        .default_value_len = default_value_len,
        .status            = config_status_POLL_FOUND_NULL,
    };
    int status = helium_channel_config_get_poll_result(&_channel->_helium->_ctx,
                                                       token,
                                                       _poll_get_result_handler,
                                                       &handler_ctx,
                                                       retries);
    return status != helium_status_OK ? status : handler_ctx.status;
}

int
Config::set(const char *       config_key,
            helium_config_type config_type,
            void *             value,
            uint16_t *         token)
{
    return helium_channel_config_set(&_channel->_helium->_ctx,
                                     _channel->_channel_id,
                                     config_key,
                                     config_type,
                                     value,
                                     token);
}

int
Config::_set(const char *            config_key,
             enum helium_config_type value_type,
             void *                  value,
             uint32_t                retries)
{
    uint16_t token;
    int      status = set(config_key, value_type, value, &token);
    int8_t   result = 0;
    if (helium_status_OK == status)
    {
        status = poll_set_result(token, &result, retries);
    }
    if (helium_status_OK == status && result != 0)
    {
        status = helium_status_ERR_COMMUNICATION;
    }
    return status;
}

bool
Config::is_stale()
{
    bool result = false;
    helium_channel_config_poll_invalidate(&_channel->_helium->_ctx,
                                          _channel->_channel_id,
                                          &result,
                                          0);
    return result;
}
