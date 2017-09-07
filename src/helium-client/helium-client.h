/*
 * Copyright 2017, Helium Systems, Inc.
 * All Rights Reserved. See LICENSE.txt for license information
 */

#ifndef HELIUM_CLIENT_H
#define HELIUM_CLIENT_H

#include "cauterize/atom_api.h"
#include "cauterize/config_api.h"
#include "helium-client-error.h"
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define HELIUM_MAX_DATA_SIZE (VECTOR_MAX_LEN_frame_app)
#define HELIUM_MAX_CHANNEL_NAME_SIZE (VECTOR_MAX_LEN_frame_app - 1)
#define HELIUM_POLL_WAIT_US 500000
#define HELIUM_POLL_RETRIES_5S ((1000000 / HELIUM_POLL_WAIT_US) * 5)
#define HELIUM_POLL_RETRIES_60S ((1000000 / HELIUM_POLL_WAIT_US) * 60)

enum helium_baud
{
    helium_baud_b9600,
    helium_baud_b14400,
    helium_baud_b19200,
    helium_baud_b38400,
    helium_baud_b57600,
    helium_baud_b115200,
};

struct helium_info
{
    uint64_t mac;
    uint32_t uptime;
    uint32_t time;
    uint32_t fw_version;
    uint8_t  radio_count;
};

struct helium_ctx
{
    void * param;

    /* Internal state */
    uint8_t    channel_ref;
    uint16_t   txn_seq;
    struct txn txn;
    uint8_t    buf[HELIUM_MAX_DATA_SIZE];
};

void
helium_init(struct helium_ctx * ctx, void * param);

bool
helium_needs_reset(struct helium_ctx * ctx);

int
helium_baud(struct helium_ctx * ctx, enum helium_baud baud);

int
helium_info(struct helium_ctx * ctx, struct helium_info * info);

int
helium_connected(struct helium_ctx * ctx);

int
helium_connect(struct helium_ctx * ctx,
               struct connection * connection,
               int32_t             retries);

int
helium_sleep(struct helium_ctx * ctx, struct connection * connection);

int
helium_reset(struct helium_ctx * ctx);

int
helium_poll(struct helium_ctx * ctx, void * data, const size_t len, size_t * used);

int
helium_poll_token(struct helium_ctx * ctx,
                  uint16_t            token,
                  void *              data,
                  size_t              len,
                  size_t *            used,
                  uint32_t            retries);

int
helium_poll_result(struct helium_ctx * ctx,
                   uint16_t            token,
                   int8_t *            result,
                   uint32_t            retries);

int
helium_create_channel(struct helium_ctx * ctx,
                      const char *        name,
                      size_t              len,
                      uint16_t *          token);

//
// Channel functions
//

struct helium_channel
{
    struct helium_ctx * helium;
    uint8_t             channel_id;
};

void
helium_channel_init(struct helium_channel * channel,
                    struct helium_ctx *     helium,
                    uint8_t                 channel_id);

int
helium_channel_poll_data(struct helium_channel * channel,
                         void *                  data,
                         size_t                  len,
                         size_t *                used,
                         uint32_t                retries);

int
helium_channel_send(struct helium_channel * channel,
                    void const *            data,
                    size_t                  len,
                    uint16_t *              token);

//
// Configuration
//

struct helium_config
{
    struct helium_channel * channel;
    struct cmd_config       cmd;
    uint8_t                 buf[HELIUM_MAX_DATA_SIZE];
};

enum helium_config_type
{
    helium_config_i32,
    helium_config_f32,
    helium_config_str,
    helium_config_bool,
    helium_config_null,
};

void
helium_config_init(struct helium_config *  config,
                   struct helium_channel * channel);

int
helium_config_get(struct helium_config * config,
                  const char *           config_key,
                  uint16_t *             token);

typedef bool (*helium_config_handler)(void *                  handler_ctx,
                                      const char *            key,
                                      enum helium_config_type value_type,
                                      void *                  value);

int
helium_config_get_poll_result(struct helium_config * config,
                              uint16_t               token,
                              helium_config_handler  handler,
                              void *                 handler_ctx,
                              int8_t *               result,
                              uint32_t               retries);


int
helium_config_set(struct helium_config *  config,
                  const char *            config_key,
                  enum helium_config_type value_type,
                  void *                  value,
                  uint16_t *              token);

int
helium_config_set_poll_result(struct helium_config * config,
                              uint16_t               token,
                              int8_t *               result,
                              uint32_t               retries);

int
helium_config_poll_invalidate(struct helium_config * config,
                              bool *                 result,
                              uint32_t               retries);

//
// Externally required functions
//

extern bool
helium_serial_readable(void * param);
extern bool
helium_serial_getc(void * param, uint8_t * ch);
extern bool
helium_serial_putc(void * param, uint8_t ch);
extern void
helium_wait_us(void * param, uint32_t us);

#ifdef __cplusplus
}
#endif

#endif /* HELIUM_CLIENT_H */
