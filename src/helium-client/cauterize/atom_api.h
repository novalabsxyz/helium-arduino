#ifndef _CAUTERIZE_C11REF_atom_api_
#define _CAUTERIZE_C11REF_atom_api_

#include "cauterize.h"

/* library meta information */
#define NAME_atom_api "atom_api"
#define VERSION_atom_api "1.0.0"
#define MIN_SIZE_atom_api (1)
#define MAX_SIZE_atom_api (110)
#define NUM_TYPES_atom_api (22)

/* schema hash */
extern hashtype_t const SCHEMA_HASH_atom_api;

/* type indicies */
enum type_index_atom_api {
  type_index_atom_api_res_send = 0,
  type_index_atom_api_res_info = 1,
  type_index_atom_api_res_connect = 2,
  type_index_atom_api_req_logging = 3,
  type_index_atom_api_frame_app = 4,
  type_index_atom_api_res_poll = 5,
  type_index_atom_api_cmd_send = 6,
  type_index_atom_api_cmd_poll = 7,
  type_index_atom_api_cmd_mac = 8,
  type_index_atom_api_cmd_logging = 9,
  type_index_atom_api_cmd_info = 10,
  type_index_atom_api_cmd_connected = 11,
  type_index_atom_api_atom_baud = 12,
  type_index_atom_api_cmd_baud = 13,
  type_index_atom_api_arr_u8_32 = 14,
  type_index_atom_api_connection = 15,
  type_index_atom_api_req_connect = 16,
  type_index_atom_api_cmd_connect = 17,
  type_index_atom_api_res_sleep = 18,
  type_index_atom_api_cmd_sleep = 19,
  type_index_atom_api_cmd = 20,
  type_index_atom_api_txn = 21,
};

/* type definitions */
#define ENUM_MAX_VAL_res_send (res_send_err_channel_access)
enum res_send {
  res_send_ok = 0,
  res_send_err_not_connected = 1,
  res_send_err_dropped = 2,
  res_send_err_nack = 3,
  res_send_err_channel_access = 4,
};

struct res_info {
  uint64_t mac;
  uint32_t uptime;
  uint32_t time;
  uint32_t fw_version;
  uint8_t radio_count;
};

#define ENUM_MAX_VAL_res_connect (res_connect_err_fallback_to_slow_connect)
enum res_connect {
  res_connect_ok = 0,
  res_connect_err_fallback_to_slow_connect = 1,
};

#define ENUM_MAX_VAL_req_logging (req_logging_disable)
enum req_logging {
  req_logging_enable = 0,
  req_logging_disable = 1,
};

#define VECTOR_MAX_LEN_frame_app (103)
struct frame_app {
  caut_tag8_t _length;
  uint8_t elems[VECTOR_MAX_LEN_frame_app];
};

#define UNION_NUM_FIELDS_res_poll (0x3ull)
struct res_poll {
  enum res_poll_tag {
    res_poll_tag_none = 0,
    res_poll_tag_frame = 1,
    res_poll_tag_log = 2,
  } _tag;


  union {
    /* no data for field i"none" with index 0 */
    struct frame_app frame;
    struct frame_app log;
  };

};

#define UNION_NUM_FIELDS_cmd_send (0x2ull)
struct cmd_send {
  enum cmd_send_tag {
    cmd_send_tag_req = 0,
    cmd_send_tag_res = 1,
  } _tag;


  union {
    struct frame_app req;
    enum res_send res;
  };

};

#define UNION_NUM_FIELDS_cmd_poll (0x2ull)
struct cmd_poll {
  enum cmd_poll_tag {
    cmd_poll_tag_req = 0,
    cmd_poll_tag_res = 1,
  } _tag;


  union {
    struct frame_app req;
    struct res_poll res;
  };

};

#define UNION_NUM_FIELDS_cmd_mac (0x2ull)
struct cmd_mac {
  enum cmd_mac_tag {
    cmd_mac_tag_req = 0,
    cmd_mac_tag_res = 1,
  } _tag;


  union {
    /* no data for field i"req" with index 0 */
    uint64_t res;
  };

};

#define UNION_NUM_FIELDS_cmd_logging (0x2ull)
struct cmd_logging {
  enum cmd_logging_tag {
    cmd_logging_tag_req = 0,
    cmd_logging_tag_res = 1,
  } _tag;


  union {
    enum req_logging req;
    /* no data for field i"res" with index 1 */
  };

};

#define UNION_NUM_FIELDS_cmd_info (0x2ull)
struct cmd_info {
  enum cmd_info_tag {
    cmd_info_tag_req = 0,
    cmd_info_tag_res = 1,
  } _tag;


  union {
    /* no data for field i"req" with index 0 */
    struct res_info res;
  };

};

#define UNION_NUM_FIELDS_cmd_connected (0x2ull)
struct cmd_connected {
  enum cmd_connected_tag {
    cmd_connected_tag_req = 0,
    cmd_connected_tag_res = 1,
  } _tag;


  union {
    /* no data for field i"req" with index 0 */
    bool res;
  };

};

#define ENUM_MAX_VAL_atom_baud (atom_baud_b230400)
enum atom_baud {
  atom_baud_b9600 = 0,
  atom_baud_b14400 = 1,
  atom_baud_b19200 = 2,
  atom_baud_b38400 = 3,
  atom_baud_b57600 = 4,
  atom_baud_b115200 = 5,
  atom_baud_b128000 = 6,
  atom_baud_b230400 = 7,
};

#define UNION_NUM_FIELDS_cmd_baud (0x2ull)
struct cmd_baud {
  enum cmd_baud_tag {
    cmd_baud_tag_req = 0,
    cmd_baud_tag_res = 1,
  } _tag;


  union {
    enum atom_baud req;
    /* no data for field i"res" with index 1 */
  };

};

#define ARRAY_LEN_arr_u8_32 (32)
struct arr_u8_32 {
  uint8_t elems[ARRAY_LEN_arr_u8_32];
};

struct connection {
  uint64_t long_addr;
  uint32_t fw_version;
  uint32_t time;
  uint16_t pan_id;
  uint16_t short_addr;
  uint16_t gateway_addr;
  uint8_t connected;
  uint8_t pan_seq;
  uint8_t radio_index;
  uint8_t channel;
  uint8_t speed;
  uint8_t key_slot;
  struct arr_u8_32 sess_key;
};

#define UNION_NUM_FIELDS_req_connect (0x2ull)
struct req_connect {
  enum req_connect_tag {
    req_connect_tag_cold = 0,
    req_connect_tag_quick = 1,
  } _tag;


  union {
    /* no data for field i"cold" with index 0 */
    struct connection quick;
  };

};

#define UNION_NUM_FIELDS_cmd_connect (0x2ull)
struct cmd_connect {
  enum cmd_connect_tag {
    cmd_connect_tag_req = 0,
    cmd_connect_tag_res = 1,
  } _tag;


  union {
    struct req_connect req;
    enum res_connect res;
  };

};

#define UNION_NUM_FIELDS_res_sleep (0x3ull)
struct res_sleep {
  enum res_sleep_tag {
    res_sleep_tag_not_connected = 0,
    res_sleep_tag_keep_awake = 1,
    res_sleep_tag_connection = 2,
  } _tag;


  union {
    /* no data for field i"not_connected" with index 0 */
    /* no data for field i"keep_awake" with index 1 */
    struct connection connection;
  };

};

#define UNION_NUM_FIELDS_cmd_sleep (0x2ull)
struct cmd_sleep {
  enum cmd_sleep_tag {
    cmd_sleep_tag_req = 0,
    cmd_sleep_tag_res = 1,
  } _tag;


  union {
    /* no data for field i"req" with index 0 */
    struct res_sleep res;
  };

};

#define UNION_NUM_FIELDS_cmd (0x8ull)
struct cmd {
  enum cmd_tag {
    cmd_tag_info = 0,
    cmd_tag_send = 1,
    cmd_tag_connect = 2,
    cmd_tag_connected = 3,
    cmd_tag_poll = 4,
    cmd_tag_sleep = 5,
    cmd_tag_baud = 6,
    cmd_tag_logging = 7,
  } _tag;


  union {
    struct cmd_info info;
    struct cmd_send send;
    struct cmd_connect connect;
    struct cmd_connected connected;
    struct cmd_poll poll;
    struct cmd_sleep sleep;
    struct cmd_baud baud;
    struct cmd_logging logging;
  };

};

struct txn {
  uint16_t seq;
  bool needs_reset;
  struct cmd cmd;
};


/* function prototypes */
enum caut_status encode_res_send(struct caut_encode_iter * const _c_iter, enum res_send const * const _c_obj);
enum caut_status decode_res_send(struct caut_decode_iter * const _c_iter, enum res_send * const _c_obj);
void init_res_send(enum res_send * _c_obj);
enum caut_ord compare_res_send(enum res_send const * const _c_a, enum res_send const * const _c_b);

enum caut_status encode_res_info(struct caut_encode_iter * const _c_iter, struct res_info const * const _c_obj);
enum caut_status decode_res_info(struct caut_decode_iter * const _c_iter, struct res_info * const _c_obj);
void init_res_info(struct res_info * _c_obj);
enum caut_ord compare_res_info(struct res_info const * const _c_a, struct res_info const * const _c_b);

enum caut_status encode_res_connect(struct caut_encode_iter * const _c_iter, enum res_connect const * const _c_obj);
enum caut_status decode_res_connect(struct caut_decode_iter * const _c_iter, enum res_connect * const _c_obj);
void init_res_connect(enum res_connect * _c_obj);
enum caut_ord compare_res_connect(enum res_connect const * const _c_a, enum res_connect const * const _c_b);

enum caut_status encode_req_logging(struct caut_encode_iter * const _c_iter, enum req_logging const * const _c_obj);
enum caut_status decode_req_logging(struct caut_decode_iter * const _c_iter, enum req_logging * const _c_obj);
void init_req_logging(enum req_logging * _c_obj);
enum caut_ord compare_req_logging(enum req_logging const * const _c_a, enum req_logging const * const _c_b);

enum caut_status encode_frame_app(struct caut_encode_iter * const _c_iter, struct frame_app const * const _c_obj);
enum caut_status decode_frame_app(struct caut_decode_iter * const _c_iter, struct frame_app * const _c_obj);
void init_frame_app(struct frame_app * _c_obj);
enum caut_ord compare_frame_app(struct frame_app const * const _c_a, struct frame_app const * const _c_b);

enum caut_status encode_res_poll(struct caut_encode_iter * const _c_iter, struct res_poll const * const _c_obj);
enum caut_status decode_res_poll(struct caut_decode_iter * const _c_iter, struct res_poll * const _c_obj);
void init_res_poll(struct res_poll * _c_obj);
enum caut_ord compare_res_poll(struct res_poll const * const _c_a, struct res_poll const * const _c_b);

enum caut_status encode_cmd_send(struct caut_encode_iter * const _c_iter, struct cmd_send const * const _c_obj);
enum caut_status decode_cmd_send(struct caut_decode_iter * const _c_iter, struct cmd_send * const _c_obj);
void init_cmd_send(struct cmd_send * _c_obj);
enum caut_ord compare_cmd_send(struct cmd_send const * const _c_a, struct cmd_send const * const _c_b);

enum caut_status encode_cmd_poll(struct caut_encode_iter * const _c_iter, struct cmd_poll const * const _c_obj);
enum caut_status decode_cmd_poll(struct caut_decode_iter * const _c_iter, struct cmd_poll * const _c_obj);
void init_cmd_poll(struct cmd_poll * _c_obj);
enum caut_ord compare_cmd_poll(struct cmd_poll const * const _c_a, struct cmd_poll const * const _c_b);

enum caut_status encode_cmd_mac(struct caut_encode_iter * const _c_iter, struct cmd_mac const * const _c_obj);
enum caut_status decode_cmd_mac(struct caut_decode_iter * const _c_iter, struct cmd_mac * const _c_obj);
void init_cmd_mac(struct cmd_mac * _c_obj);
enum caut_ord compare_cmd_mac(struct cmd_mac const * const _c_a, struct cmd_mac const * const _c_b);

enum caut_status encode_cmd_logging(struct caut_encode_iter * const _c_iter, struct cmd_logging const * const _c_obj);
enum caut_status decode_cmd_logging(struct caut_decode_iter * const _c_iter, struct cmd_logging * const _c_obj);
void init_cmd_logging(struct cmd_logging * _c_obj);
enum caut_ord compare_cmd_logging(struct cmd_logging const * const _c_a, struct cmd_logging const * const _c_b);

enum caut_status encode_cmd_info(struct caut_encode_iter * const _c_iter, struct cmd_info const * const _c_obj);
enum caut_status decode_cmd_info(struct caut_decode_iter * const _c_iter, struct cmd_info * const _c_obj);
void init_cmd_info(struct cmd_info * _c_obj);
enum caut_ord compare_cmd_info(struct cmd_info const * const _c_a, struct cmd_info const * const _c_b);

enum caut_status encode_cmd_connected(struct caut_encode_iter * const _c_iter, struct cmd_connected const * const _c_obj);
enum caut_status decode_cmd_connected(struct caut_decode_iter * const _c_iter, struct cmd_connected * const _c_obj);
void init_cmd_connected(struct cmd_connected * _c_obj);
enum caut_ord compare_cmd_connected(struct cmd_connected const * const _c_a, struct cmd_connected const * const _c_b);

enum caut_status encode_atom_baud(struct caut_encode_iter * const _c_iter, enum atom_baud const * const _c_obj);
enum caut_status decode_atom_baud(struct caut_decode_iter * const _c_iter, enum atom_baud * const _c_obj);
void init_atom_baud(enum atom_baud * _c_obj);
enum caut_ord compare_atom_baud(enum atom_baud const * const _c_a, enum atom_baud const * const _c_b);

enum caut_status encode_cmd_baud(struct caut_encode_iter * const _c_iter, struct cmd_baud const * const _c_obj);
enum caut_status decode_cmd_baud(struct caut_decode_iter * const _c_iter, struct cmd_baud * const _c_obj);
void init_cmd_baud(struct cmd_baud * _c_obj);
enum caut_ord compare_cmd_baud(struct cmd_baud const * const _c_a, struct cmd_baud const * const _c_b);

enum caut_status encode_arr_u8_32(struct caut_encode_iter * const _c_iter, struct arr_u8_32 const * const _c_obj);
enum caut_status decode_arr_u8_32(struct caut_decode_iter * const _c_iter, struct arr_u8_32 * const _c_obj);
void init_arr_u8_32(struct arr_u8_32 * _c_obj);
enum caut_ord compare_arr_u8_32(struct arr_u8_32 const * const _c_a, struct arr_u8_32 const * const _c_b);

enum caut_status encode_connection(struct caut_encode_iter * const _c_iter, struct connection const * const _c_obj);
enum caut_status decode_connection(struct caut_decode_iter * const _c_iter, struct connection * const _c_obj);
void init_connection(struct connection * _c_obj);
enum caut_ord compare_connection(struct connection const * const _c_a, struct connection const * const _c_b);

enum caut_status encode_req_connect(struct caut_encode_iter * const _c_iter, struct req_connect const * const _c_obj);
enum caut_status decode_req_connect(struct caut_decode_iter * const _c_iter, struct req_connect * const _c_obj);
void init_req_connect(struct req_connect * _c_obj);
enum caut_ord compare_req_connect(struct req_connect const * const _c_a, struct req_connect const * const _c_b);

enum caut_status encode_cmd_connect(struct caut_encode_iter * const _c_iter, struct cmd_connect const * const _c_obj);
enum caut_status decode_cmd_connect(struct caut_decode_iter * const _c_iter, struct cmd_connect * const _c_obj);
void init_cmd_connect(struct cmd_connect * _c_obj);
enum caut_ord compare_cmd_connect(struct cmd_connect const * const _c_a, struct cmd_connect const * const _c_b);

enum caut_status encode_res_sleep(struct caut_encode_iter * const _c_iter, struct res_sleep const * const _c_obj);
enum caut_status decode_res_sleep(struct caut_decode_iter * const _c_iter, struct res_sleep * const _c_obj);
void init_res_sleep(struct res_sleep * _c_obj);
enum caut_ord compare_res_sleep(struct res_sleep const * const _c_a, struct res_sleep const * const _c_b);

enum caut_status encode_cmd_sleep(struct caut_encode_iter * const _c_iter, struct cmd_sleep const * const _c_obj);
enum caut_status decode_cmd_sleep(struct caut_decode_iter * const _c_iter, struct cmd_sleep * const _c_obj);
void init_cmd_sleep(struct cmd_sleep * _c_obj);
enum caut_ord compare_cmd_sleep(struct cmd_sleep const * const _c_a, struct cmd_sleep const * const _c_b);

enum caut_status encode_cmd(struct caut_encode_iter * const _c_iter, struct cmd const * const _c_obj);
enum caut_status decode_cmd(struct caut_decode_iter * const _c_iter, struct cmd * const _c_obj);
void init_cmd(struct cmd * _c_obj);
enum caut_ord compare_cmd(struct cmd const * const _c_a, struct cmd const * const _c_b);

enum caut_status encode_txn(struct caut_encode_iter * const _c_iter, struct txn const * const _c_obj);
enum caut_status decode_txn(struct caut_decode_iter * const _c_iter, struct txn * const _c_obj);
void init_txn(struct txn * _c_obj);
enum caut_ord compare_txn(struct txn const * const _c_a, struct txn const * const _c_b);


#endif /* _CAUTERIZE_C11REF_atom_api_ */
