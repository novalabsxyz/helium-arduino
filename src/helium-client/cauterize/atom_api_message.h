#ifndef _CAUTERIZE_C11REF_atom_api_MESSAGE_
#define _CAUTERIZE_C11REF_atom_api_MESSAGE_

#include "atom_api.h"

/* message interface */
#define TYPE_TAG_WIDTH_atom_api (2)
#define LENGTH_WIDTH_atom_api (1)

#define MESSAGE_OVERHEAD_atom_api (TYPE_TAG_WIDTH_atom_api + LENGTH_WIDTH_atom_api)
#define MESSAGE_MAX_SIZE_atom_api (MESSAGE_OVERHEAD_atom_api + MAX_SIZE_atom_api)
#define MESSAGE_MIN_SIZE_atom_api (MESSAGE_OVERHEAD_atom_api + MIN_SIZE_atom_api)

/* type descriptors extern */
typedef struct caut_type_descriptor caut_type_descriptors_atom_api_t[NUM_TYPES_atom_api];
extern const caut_type_descriptors_atom_api_t type_descriptors;

struct message_header_atom_api {
  size_t length;
  uint8_t tag[TYPE_TAG_WIDTH_atom_api];
};

struct message_atom_api {
  enum type_index_atom_api _type;

  union {
    enum res_send msg_res_send;
    struct res_info msg_res_info;
    enum res_connect msg_res_connect;
    enum req_logging msg_req_logging;
    struct frame_app msg_frame_app;
    struct res_poll msg_res_poll;
    struct cmd_send msg_cmd_send;
    struct cmd_poll msg_cmd_poll;
    struct cmd_mac msg_cmd_mac;
    struct cmd_logging msg_cmd_logging;
    struct cmd_info msg_cmd_info;
    struct cmd_connected msg_cmd_connected;
    enum atom_baud msg_atom_baud;
    struct cmd_baud msg_cmd_baud;
    struct arr_u8_32 msg_arr_u8_32;
    struct connection msg_connection;
    struct req_connect msg_req_connect;
    struct cmd_connect msg_cmd_connect;
    struct res_sleep msg_res_sleep;
    struct cmd_sleep msg_cmd_sleep;
    struct cmd msg_cmd;
    struct txn msg_txn;
  } _data;

};

enum caut_status encode_message_atom_api(
  struct caut_encode_iter * const _iter,
  struct message_atom_api const * const _obj);

enum caut_status decode_message_header_atom_api(
  struct caut_decode_iter * const _iter,
  struct message_header_atom_api * const _header);

enum caut_status decode_message_atom_api(
  struct caut_decode_iter * const _iter,
  struct message_header_atom_api const * const _header,
  struct message_atom_api * const _obj);

#endif /* _CAUTERIZE_C11REF_atom_api_MESSAGE_ */
