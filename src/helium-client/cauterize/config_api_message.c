#include "config_api_message.h"

#define R enum caut_status
#define EI struct caut_encode_iter
#define DI struct caut_decode_iter
#define FSET(FS,IX) ((FS) & (1ull << (IX)))

/* type descriptors */
const caut_type_descriptors_config_api_t type_descriptors = {
  {
    .name = "string",
    .hash = { 0x9C,0x11,0xE8,0xA6,0x39,0x7E,0xE5,0xD8,0x52,0xDD,0xDC,0x70,0x91,0x06,0xA0,0x31,0x28,0x0E,0x54,0x10 },
    .encode = (gen_encode*)encode_string,
    .decode = (gen_decode*)decode_string,
    .min_size = 1,
    .max_size = 1,
  },
  {
    .name = "value",
    .hash = { 0x80,0x54,0xF4,0xD4,0x7B,0xFB,0x8A,0xC2,0xC9,0xCB,0x85,0x18,0x55,0xE8,0xC7,0xCB,0xF4,0x3C,0xD0,0x82 },
    .encode = (gen_encode*)encode_value,
    .decode = (gen_decode*)decode_value,
    .min_size = 1,
    .max_size = 1,
  },
  {
    .name = "key",
    .hash = { 0xFC,0x1A,0xCD,0x1A,0x1E,0x69,0xE6,0x5A,0x45,0xDB,0xEC,0xED,0x68,0x6C,0x65,0x60,0x27,0xF1,0x20,0x20 },
    .encode = (gen_encode*)encode_key,
    .decode = (gen_decode*)decode_key,
    .min_size = 1,
    .max_size = 1,
  },
  {
    .name = "cmd_config_set_res",
    .hash = { 0xB9,0xA5,0xD2,0x67,0x41,0xB8,0x7F,0x98,0x9F,0xCA,0x53,0x80,0xDC,0xAD,0x34,0xF1,0x4E,0x34,0x97,0xDA },
    .encode = (gen_encode*)encode_cmd_config_set_res,
    .decode = (gen_decode*)decode_cmd_config_set_res,
    .min_size = 1,
    .max_size = 1,
  },
  {
    .name = "assoc",
    .hash = { 0x54,0x97,0x42,0x2E,0x7A,0xBC,0x9D,0xB9,0x6E,0xA1,0x09,0x1D,0xDD,0x87,0x62,0x3E,0x5C,0x2A,0xF7,0xE3 },
    .encode = (gen_encode*)encode_assoc,
    .decode = (gen_decode*)decode_assoc,
    .min_size = 2,
    .max_size = 2,
  },
  {
    .name = "arr_key",
    .hash = { 0x2A,0x68,0x5A,0x97,0xB8,0xC3,0x6E,0x82,0xA3,0x8F,0xBF,0xE2,0xC4,0x61,0x4A,0x1B,0xAC,0xFD,0x57,0x9E },
    .encode = (gen_encode*)encode_arr_key,
    .decode = (gen_decode*)decode_arr_key,
    .min_size = 1,
    .max_size = 1,
  },
  {
    .name = "arr_assoc",
    .hash = { 0xCF,0x0F,0x50,0xB0,0xB2,0x9D,0x3F,0xCC,0xCB,0xCF,0x64,0xA7,0x6D,0x03,0xFC,0xFF,0x26,0x7C,0x5D,0x16 },
    .encode = (gen_encode*)encode_arr_assoc,
    .decode = (gen_decode*)decode_arr_assoc,
    .min_size = 1,
    .max_size = 1,
  },
  {
    .name = "cmd_config_get_res",
    .hash = { 0xAD,0x8E,0x46,0xEC,0x07,0x59,0x0C,0x04,0x5D,0x6A,0x12,0x18,0xA2,0x8C,0x31,0xDD,0xE0,0x07,0x55,0x85 },
    .encode = (gen_encode*)encode_cmd_config_get_res,
    .decode = (gen_decode*)decode_cmd_config_get_res,
    .min_size = 2,
    .max_size = 2,
  },
  {
    .name = "cmd_config_get",
    .hash = { 0x45,0x3B,0x9E,0x9A,0xA3,0x3F,0xA7,0x4F,0x19,0xE6,0x73,0xFF,0x73,0x72,0xBD,0x77,0xF3,0x30,0xE6,0x8A },
    .encode = (gen_encode*)encode_cmd_config_get,
    .decode = (gen_decode*)decode_cmd_config_get,
    .min_size = 2,
    .max_size = 2,
  },
  {
    .name = "cmd_config_set",
    .hash = { 0x63,0x47,0x3A,0x51,0x1E,0x7F,0x6F,0x5F,0xC5,0xD9,0xD5,0x51,0x4E,0x93,0xBD,0x8B,0x53,0xEA,0xED,0xD8 },
    .encode = (gen_encode*)encode_cmd_config_set,
    .decode = (gen_decode*)decode_cmd_config_set,
    .min_size = 2,
    .max_size = 2,
  },
  {
    .name = "cmd_config",
    .hash = { 0x2B,0x4D,0x33,0xBE,0xC6,0x49,0x74,0xFE,0x09,0x95,0x16,0xC5,0x36,0x73,0x62,0x29,0xC5,0xCF,0xA5,0xA3 },
    .encode = (gen_encode*)encode_cmd_config,
    .decode = (gen_decode*)decode_cmd_config,
    .min_size = 1,
    .max_size = 1,
  }
};

/* message interface */
R encode_message_config_api(EI * const _iter, struct message_config_api const * const _obj) {
  const struct caut_type_descriptor * const desc = &type_descriptors[_obj->_type];
  size_t _data_position = 0;
  uint16_t _data_len = 0;
  void * len_ptr = 0;

  STATUS_CHECK(__caut_encode_reserve(_iter, LENGTH_WIDTH_config_api, &len_ptr));
  STATUS_CHECK(__caut_encode_raw_bytes(_iter, desc->hash, TYPE_TAG_WIDTH_config_api));
  _data_position = _iter->position;
  STATUS_CHECK(desc->encode(_iter, &_obj->_data));
  _data_len = (uint16_t)(_iter->position - _data_position);

  memmove(len_ptr, &_data_len, sizeof(_data_len));

  return caut_status_ok;
}

R decode_message_header_config_api(DI * const _iter, struct message_header_config_api * const _header) {
  uint16_t length = 0;
  STATUS_CHECK(decode_u16(_iter, &length));
  _header->length = length;
  STATUS_CHECK(__caut_decode_raw_bytes(_iter, _header->tag, sizeof(_header->tag)));

  return caut_status_ok;
}

R decode_message_config_api(DI * const _iter, struct message_header_config_api const * const _header, struct message_config_api * const _obj) {
  const struct caut_type_descriptor * desc = 0;

  for (size_t i = 0; i < ARR_LEN(type_descriptors); i++) {
    if (0 == memcmp(_header->tag, type_descriptors[i].hash, TYPE_TAG_WIDTH_config_api)) {
      desc = &type_descriptors[i];
      _obj->_type = (enum type_index_config_api)i;
      break;
    }
  }

  if (0 == desc) {
    return caut_status_invalid_tag;
  } else {
    STATUS_CHECK(desc->decode(_iter, &_obj->_data));
  }

  return caut_status_ok;
}
