#ifndef _CAUTERIZE_C11REF_config_api_MESSAGE_
#define _CAUTERIZE_C11REF_config_api_MESSAGE_

#include "config_api.h"

/* message interface */
#define TYPE_TAG_WIDTH_config_api (2)
#define LENGTH_WIDTH_config_api (2)

#define MESSAGE_OVERHEAD_config_api (TYPE_TAG_WIDTH_config_api + LENGTH_WIDTH_config_api)
#define MESSAGE_MAX_SIZE_config_api (MESSAGE_OVERHEAD_config_api + MAX_SIZE_config_api)
#define MESSAGE_MIN_SIZE_config_api (MESSAGE_OVERHEAD_config_api + MIN_SIZE_config_api)

/* type descriptors extern */
typedef struct caut_type_descriptor caut_type_descriptors_config_api_t[NUM_TYPES_config_api];
extern const caut_type_descriptors_config_api_t type_descriptors;

struct message_header_config_api {
  size_t length;
  uint8_t tag[TYPE_TAG_WIDTH_config_api];
};

struct message_config_api {
  enum type_index_config_api _type;

  union {
    struct string msg_string;
    struct value msg_value;
    struct key msg_key;
    struct assoc msg_assoc;
    struct arr_key msg_arr_key;
    struct arr_assoc msg_arr_assoc;
    struct cmd_config_get msg_cmd_config_get;
    struct cmd_config_set msg_cmd_config_set;
    struct cmd_config msg_cmd_config;
  } _data;

};

enum caut_status encode_message_config_api(
  struct caut_encode_iter * const _iter,
  struct message_config_api const * const _obj);

enum caut_status decode_message_header_config_api(
  struct caut_decode_iter * const _iter,
  struct message_header_config_api * const _header);

enum caut_status decode_message_config_api(
  struct caut_decode_iter * const _iter,
  struct message_header_config_api const * const _header,
  struct message_config_api * const _obj);

#endif /* _CAUTERIZE_C11REF_config_api_MESSAGE_ */
