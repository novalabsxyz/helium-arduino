/*
 * Copyright 2017, Helium Systems, Inc.
 * All Rights Reserved. See LICENCE.txt for license information
 */

#include "helium-client.h"
#include <inttypes.h>
#include <stdio.h>
#include <string.h>


#define SOF_CHAR (0x7E) ///< Start-of-frame character.

enum decode_state
{
    sof,
    len_msb,
    len_lsb,
    payload,
    checksum
};

// 1 second timeout
#define SERIAL_WAIT_TIMEOUT 2000
static inline bool
_wait_for_byte(struct helium_ctx * ctx, size_t timeout)
{
    while (timeout--)
    {
        if (helium_serial_readable(ctx->param))
        {
            return true;
        }
        else
        {
            helium_wait_us(ctx->param, 500);
        }
    }
    return false;
}

static size_t
_read_frame(struct helium_ctx * ctx, uint8_t * frame, size_t len)
{
    enum decode_state state            = sof;
    size_t            payload_size     = 0;
    uint8_t           payload_checksum = 0;

    while (_wait_for_byte(ctx, SERIAL_WAIT_TIMEOUT))
    {
        uint8_t ch;
        if (!helium_serial_getc(ctx->param, &ch))
        {
            // Error reading
            return -3;
        }

        switch (state)
        {
        case sof:
            // Waiting for SOF
            if (ch == SOF_CHAR)
            {
                state = len_msb;
            }
            break;
        case len_msb:
            // Length high byte
            payload_size = ch << 8;
            state        = len_lsb;
            break;
        case len_lsb:
        {
            // Length low byte
            payload_size += ch;
            // Overflow error
            if (payload_size > len)
            {
                return -1;
            }
            len   = payload_size;
            state = payload;
        }
        break;
        case payload:
            *frame++ = ch;
            payload_checksum += ch;
            if (--len == 0)
            {
                payload_checksum = 0xFF - payload_checksum;
                state            = checksum;
            }
            break;
        case checksum:
            if (ch == payload_checksum)
            {
                return payload_size;
            }
            else
            {
                // Checksum error
                return -2;
            }
        default:
            break;
        }
    }
    // Timeout error
    return 0;
}

static size_t
_write_frame(struct helium_ctx * ctx, uint8_t * frame, size_t len)
{
    if (len == 0)
    {
        return 0;
    }

    size_t written = 0;
#define CHECK_WRITE(ch, inc)                                                   \
    if (!helium_serial_putc(ctx->param, (ch)))                                 \
    {                                                                          \
        return -1;                                                             \
    }                                                                          \
    else                                                                       \
    {                                                                          \
        written += (inc);                                                      \
    }

    CHECK_WRITE(SOF_CHAR, 0);
    CHECK_WRITE(len >> 8, 0);
    CHECK_WRITE(len & 0xFF, 0);

    uint8_t checksum = 0;
    while (len--)
    {
        int8_t ch = *frame++;
        CHECK_WRITE(ch, 1);
        checksum += ch;
    }
    checksum = 0xFF - checksum;
    CHECK_WRITE(checksum, 0);

    return written;
}

enum send_command_status
{
    send_command_OK,
    send_command_ERR_ENCODE,
    send_command_ERR_DECODE,
    send_command_ERR_COMMUNICATION,
};

static enum send_command_status
send_command(struct helium_ctx * ctx)
{
    uint16_t seq = ctx->txn_seq++;

    ctx->txn.seq = seq;

    struct caut_encode_iter ei;
    caut_encode_iter_init(&ei, ctx->buf, sizeof(ctx->buf));
    if (caut_status_ok != encode_txn(&ei, &ctx->txn))
    {
        return send_command_ERR_ENCODE;
    }

#ifdef HE_DEBUG_ATOM
    printf("SEND: ");
    for (unsigned int i = 0; i < ei.position; i++) {
        printf("%d ", ctx->buf[i]);
    }
    printf("\n");
#endif

    size_t len = _write_frame(ctx, ctx->buf, ei.position);
    if (len != ei.position)
    {
        return send_command_ERR_COMMUNICATION;
    }

    len = _read_frame(ctx, ctx->buf, sizeof(ctx->buf));
    if ((int)len <= 0)
    {
        return send_command_ERR_COMMUNICATION;
    }

#ifdef HE_DEBUG_ATOM
    printf("RECV: ");
    for (unsigned int i = 0; i < ei.position; i++) {
        printf("%d ", ctx->buf[i]);
    }
    printf("\n");
#endif

    struct caut_decode_iter di;
    caut_decode_iter_init(&di, ctx->buf, len);
    if (caut_status_ok != decode_txn(&di, &ctx->txn))
    {
        return send_command_ERR_DECODE;
    }

    return send_command_OK;
}

void
helium_init(struct helium_ctx * ctx, void * param)
{
    memset(ctx, 0, sizeof(*ctx));
    if (NULL != param)
    {
        ctx->param = param;
    }
}

bool
helium_needs_reset(struct helium_ctx * ctx)
{
    return ctx->txn.needs_reset;
}

int
helium_baud(struct helium_ctx * ctx, enum helium_baud baud)
{
    ctx->txn.cmd._tag      = cmd_tag_baud;
    ctx->txn.cmd.baud._tag = cmd_baud_tag_req;

    enum atom_baud atom_baud = atom_baud_b9600;
    switch (baud)
    {
    case helium_baud_b115200:
        atom_baud = atom_baud_b115200;
        break;
    case helium_baud_b57600:
        atom_baud = atom_baud_b57600;
        break;
    case helium_baud_b38400:
        atom_baud = atom_baud_b38400;
        break;
    case helium_baud_b19200:
        atom_baud = atom_baud_b19200;
        break;
    case helium_baud_b14400:
        atom_baud = atom_baud_b14400;
        break;
    case helium_baud_b9600:
        atom_baud = atom_baud_b9600;
        break;
    }
    ctx->txn.cmd.baud.req = atom_baud;

    enum send_command_status status = send_command(ctx);
    if (send_command_OK != status)
    {
        return helium_info_ERR_COMMUNICATION;
    }

    return helium_info_OK;
}

int
helium_info(struct helium_ctx * ctx, struct helium_info * info)
{
    ctx->txn.cmd._tag      = cmd_tag_info;
    ctx->txn.cmd.info._tag = cmd_info_tag_req;

    enum send_command_status status = send_command(ctx);
    if (send_command_OK != status)
    {
        return helium_info_ERR_COMMUNICATION;
    }

    info->mac         = ctx->txn.cmd.info.res.mac;
    info->uptime      = ctx->txn.cmd.info.res.uptime;
    info->time        = ctx->txn.cmd.info.res.time;
    info->fw_version  = ctx->txn.cmd.info.res.fw_version;
    info->radio_count = ctx->txn.cmd.info.res.radio_count;

    return helium_info_OK;
}

int
helium_connected(struct helium_ctx * ctx)
{
    ctx->txn.cmd._tag           = cmd_tag_connected;
    ctx->txn.cmd.connected._tag = cmd_connected_tag_req;

    enum send_command_status status = send_command(ctx);
    if (status != send_command_OK)
    {
        return helium_connected_ERR_COMMUNICATION;
    }

    if (!ctx->txn.cmd.connected.res)
    {
        return helium_connected_NOT_CONNECTED;
    }
    return helium_connected_CONNECTED;
}

int
helium_connect(struct helium_ctx * ctx,
               struct connection * connection,
               int32_t             retries)
{
    ctx->txn.cmd._tag         = cmd_tag_connect;
    ctx->txn.cmd.connect._tag = cmd_connect_tag_req;

    if (NULL != connection)
    {
        ctx->txn.cmd.connect.req._tag  = req_connect_tag_quick;
        ctx->txn.cmd.connect.req.quick = *connection;
    }
    else
    {
        ctx->txn.cmd.connect.req._tag = req_connect_tag_cold;
    }

    enum send_command_status status = send_command(ctx);
    if (send_command_OK != status)
    {
        return helium_connect_ERR_COMMUNICATION;
    }

    while (retries-- > 0)
    {
        enum helium_connected_status connected_status = helium_connected(ctx);
        switch (connected_status)
        {
        case helium_connected_CONNECTED:
            return helium_connect_CONNECTED;
        case helium_connected_NOT_CONNECTED:
            if (retries > 0)
            {
                helium_wait_us(ctx->param, HELIUM_POLL_WAIT_US);
            }
            break;
        case helium_connected_ERR_COMMUNICATION:
            return helium_connect_ERR_COMMUNICATION;
        }
    }

    if (retries <= 0)
    {
        return helium_connect_NOT_CONNECTED;
    }

    return helium_connect_ERR_COMMUNICATION;
}

int
helium_sleep(struct helium_ctx * ctx, struct connection * connection)
{
    ctx->txn.cmd._tag       = cmd_tag_sleep;
    ctx->txn.cmd.sleep._tag = cmd_sleep_tag_req;

    enum send_command_status status = send_command(ctx);
    if (send_command_OK != status)
    {
        return helium_sleep_ERR_COMMUNICATION;
    }

    switch (ctx->txn.cmd.sleep.res._tag)
    {
    case res_sleep_tag_not_connected:
        return helium_sleep_ERR_NOT_CONNECTED;
    case res_sleep_tag_keep_awake:
        return helium_sleep_ERR_KEEP_AWAKE;
    case res_sleep_tag_connection:
        if (connection)
        {
            *connection = ctx->txn.cmd.sleep.res.connection;
        }
        return helium_sleep_OK;
    }
    return helium_sleep_ERR_COMMUNICATION;
}


enum helium_send_status
{
    helium_send_OK,
    /* Some other Atom module condition caused us to fail. */
    helium_send_ERR_NOT_CONNECTED,
    helium_send_ERR_DROPPED,
    helium_send_ERR_COMMUNICATION,
};

static enum helium_send_status
helium_send(struct helium_ctx * ctx, void const * data, size_t len)
{
    ctx->txn.cmd._tag      = cmd_tag_send;
    ctx->txn.cmd.send._tag = cmd_send_tag_req;

    size_t copylen = data ? len : 0;
    if (copylen > VECTOR_MAX_LEN_frame_app)
    {
        copylen = VECTOR_MAX_LEN_frame_app;
    }

    if (data)
    {
        memcpy(ctx->txn.cmd.send.req.elems, data, copylen);
    }
    ctx->txn.cmd.send.req._length = copylen;

    enum send_command_status status = send_command(ctx);
    if (send_command_OK != status)
    {
        return helium_send_ERR_COMMUNICATION;
    }

    switch (ctx->txn.cmd.send.res)
    {
    case res_send_ok:
        return helium_send_OK;
    case res_send_err_not_connected:
        return helium_send_ERR_NOT_CONNECTED;
    case res_send_err_dropped:
    case res_send_err_nack:
    case res_send_err_channel_access:
        return helium_send_ERR_DROPPED;
    }

    return helium_send_ERR_COMMUNICATION;
}

int
helium_poll(struct helium_ctx * ctx, void * data, const size_t len, size_t * used)
{
    ctx->txn.cmd._tag             = cmd_tag_poll;
    ctx->txn.cmd.poll._tag        = cmd_poll_tag_req;
    ctx->txn.cmd.poll.req._length = *used;
    if (*used > 0)
    {
        memcpy(ctx->txn.cmd.poll.req.elems, data, *used);
    }

    enum send_command_status status = send_command(ctx);
    if (send_command_OK != status)
    {
        return helium_poll_ERR_COMMUNICATION;
    }

    switch (ctx->txn.cmd.poll.res._tag)
    {
    case res_poll_tag_none:
        return helium_poll_OK_NO_DATA;
    case res_poll_tag_log:
        return helium_poll_OK_NO_DATA;
    case res_poll_tag_frame:
    {
        size_t copylen = ctx->txn.cmd.poll.res.frame._length;
        if (copylen > len)
        {
            copylen = len;
        }
        if (data)
        {
            memcpy(data, ctx->txn.cmd.poll.res.frame.elems, copylen);
        }
        *used = copylen;
        return helium_poll_OK_DATA;
    }
    }

    return helium_poll_ERR_COMMUNICATION;
}

static __inline uint16_t
_mk_token(uint8_t frame_id, uint8_t channel_ref)
{
    return channel_ref << 8 | frame_id;
}

static __inline uint8_t
_token_frame_id(uint16_t token)
{
    return token & 0xFF;
}

static __inline uint8_t
_token_frame_ref(uint16_t token)
{
    return token >> 8 & 0xFF;
}

static int
helium_channel_poll_token(struct helium_ctx * ctx,
                          uint16_t            token,
                          void *              data,
                          size_t              len,
                          size_t *            used,
                          uint32_t            retries)
{
    while (retries-- > 0)
    {
        uint8_t * frame     = ctx->buf;
        size_t    poll_used = 2;
        *frame++            = _token_frame_id(token);
        *frame++            = _token_frame_ref(token);

        enum helium_poll_status poll_status =
            helium_poll(ctx, ctx->buf, sizeof(ctx->buf), &poll_used);
        switch (poll_status)
        {
        case helium_poll_OK_DATA:
        {
            size_t copylen = poll_used - 2;
            if (copylen > len)
            {
                copylen = len;
            }
            memcpy(data, ctx->buf + 2, copylen);
            if (used)
            {
                *used = copylen;
            }
            return helium_poll_OK_DATA;
        }
        case helium_poll_OK_NO_DATA:
            if (retries > 0)
            {
                helium_wait_us(ctx->param, HELIUM_POLL_WAIT_US);
            }
            break;
        case helium_poll_ERR_COMMUNICATION:
            return helium_poll_ERR_COMMUNICATION;
        }
    }

    return helium_poll_OK_NO_DATA;
}

int
helium_channel_poll_result(struct helium_ctx * ctx,
                           uint16_t            token,
                           int8_t *            result,
                           uint32_t            retries)
{
    return helium_channel_poll_token(ctx,
                                     token,
                                     result,
                                     sizeof(*result),
                                     NULL,
                                     retries);
}

#define CHANNEL_DATA 0x05

int
helium_channel_poll_data(struct helium_ctx * ctx,
                         uint8_t             channel_id,
                         void *              data,
                         size_t              len,
                         size_t *            used,
                         uint32_t            retries)
{
    uint16_t token = _mk_token(CHANNEL_DATA, channel_id);
    return helium_channel_poll_token(ctx, token, data, len, used, retries);
}

#define CHANNEL_CREATE 0x01
#define CHANNEL_CREATE_RESULT 0x02

int
helium_channel_create(struct helium_ctx * ctx,
                      const char *        name,
                      size_t              len,
                      uint16_t *          token)
{
    uint8_t * frame       = ctx->buf;
    uint8_t   channel_ref = ctx->channel_ref++;
    if (len > HELIUM_MAX_CHANNEL_NAME_SIZE)
    {
        len = HELIUM_MAX_CHANNEL_NAME_SIZE;
    }

    *frame++ = CHANNEL_CREATE;
    *frame++ = channel_ref;
    memcpy(frame, name, len);
    frame += len;

    enum helium_send_status status = helium_send(ctx, ctx->buf, frame - ctx->buf);
    switch (status)
    {
    case helium_send_OK:
        if (token)
        {
            *token = _mk_token(CHANNEL_CREATE_RESULT, channel_ref);
        }
        return helium_channel_create_OK;
    case helium_send_ERR_NOT_CONNECTED:
        return helium_channel_create_ERR_NOT_CONNECTED;
    case helium_send_ERR_DROPPED:
        return helium_channel_create_ERR_DROPPED;
    case helium_send_ERR_COMMUNICATION:
        return helium_channel_create_ERR_COMMUNICATION;
    }

    return helium_channel_create_ERR_COMMUNICATION;
}


#define CHANNEL_SEND 0x03
#define CHANNEL_SEND_RESULT 0x04

int
helium_channel_send(struct helium_ctx * ctx,
                    uint8_t             channel_id,
                    void const *        data,
                    size_t              len,
                    uint16_t *          token)
{
    uint8_t * frame       = ctx->buf;
    uint8_t   channel_ref = ctx->channel_ref++;
    if (len > HELIUM_MAX_DATA_SIZE)
    {
        len = HELIUM_MAX_DATA_SIZE;
    }

    *frame++ = CHANNEL_SEND;
    *frame++ = channel_ref;
    *frame++ = channel_id;
    memcpy(frame, data, len);
    frame += len;

    enum helium_send_status status = helium_send(ctx, ctx->buf, frame - ctx->buf);
    switch (status)
    {
    case helium_send_OK:
        if (token)
        {
            *token = _mk_token(CHANNEL_SEND_RESULT, channel_ref);
        }
        return helium_channel_send_OK;
    case helium_send_ERR_NOT_CONNECTED:
        return helium_channel_send_ERR_NOT_CONNECTED;
    case helium_send_ERR_DROPPED:
        return helium_channel_send_ERR_DROPPED;
    case helium_send_ERR_COMMUNICATION:
        return helium_channel_send_ERR_COMMUNICATION;
    }

    return helium_channel_send_ERR_COMMUNICATION;
}
