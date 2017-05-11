#ifndef CARBON_H
#define CARBON_H

#include <stdint.h>
#include <stdbool.h>
#include "cauterize/atom_api.h"

#ifdef __cplusplus
extern "C" {
#endif

enum carbon_info_status {
    carbon_info_OK,
    carbon_info_ERR_COMMUNICATION,
};

enum carbon_connected_status {
    carbon_connected_CONNECTED,
    carbon_connected_NOT_CONNECTED,
    carbon_connected_ERR_COMMUNICATION,
};

enum carbon_connect_status {
    carbon_connect_CONNECTED,
    carbon_connect_NOT_CONNECTED,
    carbon_connect_ERR_CONNECT_TIMEOUT,
    carbon_connect_ERR_COMMUNICATION,
};

enum carbon_sleep_status {
    carbon_sleep_OK,
    carbon_sleep_ERR_NOT_CONNECTED,
    carbon_sleep_ERR_KEEP_AWAKE,
    carbon_sleep_ERR_NEEDS_RESET,
    carbon_sleep_ERR_COMMUNICATION,
};


enum carbon_send_status {
    /* Send was ACKed by the bridge. */
    carbon_send_OK,
    /* A temporary condition prevented us from sending. */
    carbon_send_ERR_CHANNEL_ACCESS,
    carbon_send_ERR_NACK,
    /* Some other Atom module condition caused us to fail. */
    carbon_send_ERR_NOT_CONNECTED,
    carbon_send_ERR_DROPPED,
    carbon_send_ERR_COMMUNICATION,
};

static inline bool carbon_send_recoverable(enum carbon_send_status s) {
    switch (s) {
    case carbon_send_OK:
    case carbon_send_ERR_CHANNEL_ACCESS:
    case carbon_send_ERR_NACK:
        return true;
    default:
        return false;
    }
}

struct carbon_ctx {
    void *param;
    /* The sequence number for Atom transactions. */
    uint16_t txn_seq;
    struct txn txn;
    uint8_t buf[MAX_SIZE_atom_api];
    uint8_t ser_buf[4 + MAX_SIZE_atom_api];
};


enum carbon_poll_status {
    carbon_poll_OK_DATA,
    carbon_poll_OK_NO_DATA,
    carbon_poll_OK_NEEDS_RESET,

    carbon_poll_ERR_COMMUNICATION,
};



#define CARBON_MAX_DATA_SIZE (VECTOR_MAX_LEN_frame_app)

/* Called once at application startup. */
void carbon_init(struct carbon_ctx * ctx, void *param);
enum carbon_info_status carbon_info(struct carbon_ctx *ctx, struct res_info *info);
enum carbon_connected_status carbon_connected(struct carbon_ctx *ctx);
enum carbon_connect_status carbon_connect(struct carbon_ctx *ctx, struct connection *connection);
enum carbon_sleep_status carbon_sleep(struct carbon_ctx *ctx, struct connection *connection);
enum carbon_send_status carbon_send(struct carbon_ctx *ctx, void const *data, size_t len);

/* Poll the Atom for received data. If the buffer provided is not
 * large enough to hold the received frame, as much as possible will
 * be copied in and a status of _OK_TRUNCATED will be returned.
 *
 * Parameters:
 *   - ctx: the carbon context (required)
 *   - data: the buffer to copy RX data into (required)
 *   - len: the length of the buffer (required)
 *   - used: how much of the buffer was used (required)
 *   - drops: how many packets have been dropped due to a low polling
 *            rate since the Atom was last powered on (optional)
 *
 * Returns:
 *   - _OK_DATA: when received bytes are copied into data
 *   - _OK_NO_DATA: no data was received
 *   - _NEEDS_RESET: the atom received a firmware upgrade and needs to
 *       be reset
 */
enum carbon_poll_status carbon_poll(struct carbon_ctx * ctx,
                                    void * data, size_t len, size_t * used,
                                    uint32_t * drops);


extern bool carbon_serial_readable(void *param);
extern bool carbon_serial_getc(void *param, uint8_t *ch);
extern bool carbon_serial_putc(void *param, uint8_t ch);
extern void carbon_wait_ms(void *param, uint32_t ms);

#ifdef __cplusplus
}
#endif

#endif /* CARBON_H */
