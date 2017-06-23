#include "atom_api.h"

#define R enum caut_status
#define EI struct caut_encode_iter
#define DI struct caut_decode_iter
#define FSET(FS,IX) ((FS) & (1ull << (IX)))

/* schema hash */
hashtype_t const SCHEMA_HASH_atom_api = { 0x00,0x4B,0x26,0xCB,0x70,0x35,0xDB,0x7A,0xB7,0x6E,0xDD,0xD2,0xC6,0xBB,0x65,0x74,0x7C,0xD7,0x9B,0x2F };

/* type encoders */
R encode_res_send(EI * const _c_iter, enum res_send const * const _c_obj) {
  caut_tag8_t _c_tag;

  if (ENUM_MAX_VAL_res_send < *_c_obj) {
    return caut_status_enumeration_out_of_range;
  }

  _c_tag = (caut_tag8_t)*_c_obj;

  STATUS_CHECK(encode_tag8(_c_iter, &_c_tag));

  return caut_status_ok;
}

R encode_res_info(EI * const _c_iter, struct res_info const * const _c_obj) {
  STATUS_CHECK(encode_u64(_c_iter, &_c_obj->mac));
  STATUS_CHECK(encode_u32(_c_iter, &_c_obj->uptime));
  STATUS_CHECK(encode_u32(_c_iter, &_c_obj->time));
  STATUS_CHECK(encode_u32(_c_iter, &_c_obj->fw_version));
  STATUS_CHECK(encode_u8(_c_iter, &_c_obj->radio_count));

  return caut_status_ok;
}

R encode_res_connect(EI * const _c_iter, enum res_connect const * const _c_obj) {
  caut_tag8_t _c_tag;

  if (ENUM_MAX_VAL_res_connect < *_c_obj) {
    return caut_status_enumeration_out_of_range;
  }

  _c_tag = (caut_tag8_t)*_c_obj;

  STATUS_CHECK(encode_tag8(_c_iter, &_c_tag));

  return caut_status_ok;
}

R encode_req_logging(EI * const _c_iter, enum req_logging const * const _c_obj) {
  caut_tag8_t _c_tag;

  if (ENUM_MAX_VAL_req_logging < *_c_obj) {
    return caut_status_enumeration_out_of_range;
  }

  _c_tag = (caut_tag8_t)*_c_obj;

  STATUS_CHECK(encode_tag8(_c_iter, &_c_tag));

  return caut_status_ok;
}

R encode_frame_app(EI * const _c_iter, struct frame_app const * const _c_obj) {
  if (_c_obj->_length > VECTOR_MAX_LEN_frame_app) {
    return caut_status_invalid_length;
  }

  STATUS_CHECK(encode_tag8(_c_iter, &_c_obj->_length));

  for (size_t _c_i = 0; _c_i < _c_obj->_length; _c_i++) {
    STATUS_CHECK(encode_u8(_c_iter, &_c_obj->elems[_c_i]));
  }

  return caut_status_ok;
}

R encode_res_poll(EI * const _c_iter, struct res_poll const * const _c_obj) {
  caut_tag8_t _temp_tag = (caut_tag8_t)_c_obj->_tag;

  if (_temp_tag >= UNION_NUM_FIELDS_res_poll) {
    return caut_status_invalid_tag;
  }

  STATUS_CHECK(encode_tag8(_c_iter, &_temp_tag));

  switch(_c_obj->_tag) {
  case res_poll_tag_none: /* No data for field none with index 0. */ break;
  case res_poll_tag_frame: STATUS_CHECK(encode_frame_app(_c_iter, &_c_obj->frame)); break;
  case res_poll_tag_log: STATUS_CHECK(encode_frame_app(_c_iter, &_c_obj->log)); break;
  }

  return caut_status_ok;
}

R encode_cmd_send(EI * const _c_iter, struct cmd_send const * const _c_obj) {
  caut_tag8_t _temp_tag = (caut_tag8_t)_c_obj->_tag;

  if (_temp_tag >= UNION_NUM_FIELDS_cmd_send) {
    return caut_status_invalid_tag;
  }

  STATUS_CHECK(encode_tag8(_c_iter, &_temp_tag));

  switch(_c_obj->_tag) {
  case cmd_send_tag_req: STATUS_CHECK(encode_frame_app(_c_iter, &_c_obj->req)); break;
  case cmd_send_tag_res: STATUS_CHECK(encode_res_send(_c_iter, &_c_obj->res)); break;
  }

  return caut_status_ok;
}

R encode_cmd_poll(EI * const _c_iter, struct cmd_poll const * const _c_obj) {
  caut_tag8_t _temp_tag = (caut_tag8_t)_c_obj->_tag;

  if (_temp_tag >= UNION_NUM_FIELDS_cmd_poll) {
    return caut_status_invalid_tag;
  }

  STATUS_CHECK(encode_tag8(_c_iter, &_temp_tag));

  switch(_c_obj->_tag) {
  case cmd_poll_tag_req: STATUS_CHECK(encode_frame_app(_c_iter, &_c_obj->req)); break;
  case cmd_poll_tag_res: STATUS_CHECK(encode_res_poll(_c_iter, &_c_obj->res)); break;
  }

  return caut_status_ok;
}

R encode_cmd_mac(EI * const _c_iter, struct cmd_mac const * const _c_obj) {
  caut_tag8_t _temp_tag = (caut_tag8_t)_c_obj->_tag;

  if (_temp_tag >= UNION_NUM_FIELDS_cmd_mac) {
    return caut_status_invalid_tag;
  }

  STATUS_CHECK(encode_tag8(_c_iter, &_temp_tag));

  switch(_c_obj->_tag) {
  case cmd_mac_tag_req: /* No data for field req with index 0. */ break;
  case cmd_mac_tag_res: STATUS_CHECK(encode_u64(_c_iter, &_c_obj->res)); break;
  }

  return caut_status_ok;
}

R encode_cmd_logging(EI * const _c_iter, struct cmd_logging const * const _c_obj) {
  caut_tag8_t _temp_tag = (caut_tag8_t)_c_obj->_tag;

  if (_temp_tag >= UNION_NUM_FIELDS_cmd_logging) {
    return caut_status_invalid_tag;
  }

  STATUS_CHECK(encode_tag8(_c_iter, &_temp_tag));

  switch(_c_obj->_tag) {
  case cmd_logging_tag_req: STATUS_CHECK(encode_req_logging(_c_iter, &_c_obj->req)); break;
  case cmd_logging_tag_res: /* No data for field res with index 1. */ break;
  }

  return caut_status_ok;
}

R encode_cmd_info(EI * const _c_iter, struct cmd_info const * const _c_obj) {
  caut_tag8_t _temp_tag = (caut_tag8_t)_c_obj->_tag;

  if (_temp_tag >= UNION_NUM_FIELDS_cmd_info) {
    return caut_status_invalid_tag;
  }

  STATUS_CHECK(encode_tag8(_c_iter, &_temp_tag));

  switch(_c_obj->_tag) {
  case cmd_info_tag_req: /* No data for field req with index 0. */ break;
  case cmd_info_tag_res: STATUS_CHECK(encode_res_info(_c_iter, &_c_obj->res)); break;
  }

  return caut_status_ok;
}

R encode_cmd_connected(EI * const _c_iter, struct cmd_connected const * const _c_obj) {
  caut_tag8_t _temp_tag = (caut_tag8_t)_c_obj->_tag;

  if (_temp_tag >= UNION_NUM_FIELDS_cmd_connected) {
    return caut_status_invalid_tag;
  }

  STATUS_CHECK(encode_tag8(_c_iter, &_temp_tag));

  switch(_c_obj->_tag) {
  case cmd_connected_tag_req: /* No data for field req with index 0. */ break;
  case cmd_connected_tag_res: STATUS_CHECK(encode_bool(_c_iter, &_c_obj->res)); break;
  }

  return caut_status_ok;
}

R encode_atom_baud(EI * const _c_iter, enum atom_baud const * const _c_obj) {
  caut_tag8_t _c_tag;

  if (ENUM_MAX_VAL_atom_baud < *_c_obj) {
    return caut_status_enumeration_out_of_range;
  }

  _c_tag = (caut_tag8_t)*_c_obj;

  STATUS_CHECK(encode_tag8(_c_iter, &_c_tag));

  return caut_status_ok;
}

R encode_cmd_baud(EI * const _c_iter, struct cmd_baud const * const _c_obj) {
  caut_tag8_t _temp_tag = (caut_tag8_t)_c_obj->_tag;

  if (_temp_tag >= UNION_NUM_FIELDS_cmd_baud) {
    return caut_status_invalid_tag;
  }

  STATUS_CHECK(encode_tag8(_c_iter, &_temp_tag));

  switch(_c_obj->_tag) {
  case cmd_baud_tag_req: STATUS_CHECK(encode_atom_baud(_c_iter, &_c_obj->req)); break;
  case cmd_baud_tag_res: /* No data for field res with index 1. */ break;
  }

  return caut_status_ok;
}

R encode_arr_u8_32(EI * const _c_iter, struct arr_u8_32 const * const _c_obj) {
  for (size_t _c_i = 0; _c_i < ARR_LEN(_c_obj->elems); _c_i++) {
    STATUS_CHECK(encode_u8(_c_iter, &_c_obj->elems[_c_i]));
  }

  return caut_status_ok;
}

R encode_connection(EI * const _c_iter, struct connection const * const _c_obj) {
  STATUS_CHECK(encode_u64(_c_iter, &_c_obj->long_addr));
  STATUS_CHECK(encode_u32(_c_iter, &_c_obj->fw_version));
  STATUS_CHECK(encode_u32(_c_iter, &_c_obj->time));
  STATUS_CHECK(encode_u16(_c_iter, &_c_obj->pan_id));
  STATUS_CHECK(encode_u16(_c_iter, &_c_obj->short_addr));
  STATUS_CHECK(encode_u16(_c_iter, &_c_obj->gateway_addr));
  STATUS_CHECK(encode_u8(_c_iter, &_c_obj->connected));
  STATUS_CHECK(encode_u8(_c_iter, &_c_obj->pan_seq));
  STATUS_CHECK(encode_u8(_c_iter, &_c_obj->radio_index));
  STATUS_CHECK(encode_u8(_c_iter, &_c_obj->channel));
  STATUS_CHECK(encode_u8(_c_iter, &_c_obj->speed));
  STATUS_CHECK(encode_u8(_c_iter, &_c_obj->key_slot));
  STATUS_CHECK(encode_arr_u8_32(_c_iter, &_c_obj->sess_key));

  return caut_status_ok;
}

R encode_req_connect(EI * const _c_iter, struct req_connect const * const _c_obj) {
  caut_tag8_t _temp_tag = (caut_tag8_t)_c_obj->_tag;

  if (_temp_tag >= UNION_NUM_FIELDS_req_connect) {
    return caut_status_invalid_tag;
  }

  STATUS_CHECK(encode_tag8(_c_iter, &_temp_tag));

  switch(_c_obj->_tag) {
  case req_connect_tag_cold: /* No data for field cold with index 0. */ break;
  case req_connect_tag_quick: STATUS_CHECK(encode_connection(_c_iter, &_c_obj->quick)); break;
  }

  return caut_status_ok;
}

R encode_cmd_connect(EI * const _c_iter, struct cmd_connect const * const _c_obj) {
  caut_tag8_t _temp_tag = (caut_tag8_t)_c_obj->_tag;

  if (_temp_tag >= UNION_NUM_FIELDS_cmd_connect) {
    return caut_status_invalid_tag;
  }

  STATUS_CHECK(encode_tag8(_c_iter, &_temp_tag));

  switch(_c_obj->_tag) {
  case cmd_connect_tag_req: STATUS_CHECK(encode_req_connect(_c_iter, &_c_obj->req)); break;
  case cmd_connect_tag_res: STATUS_CHECK(encode_res_connect(_c_iter, &_c_obj->res)); break;
  }

  return caut_status_ok;
}

R encode_res_sleep(EI * const _c_iter, struct res_sleep const * const _c_obj) {
  caut_tag8_t _temp_tag = (caut_tag8_t)_c_obj->_tag;

  if (_temp_tag >= UNION_NUM_FIELDS_res_sleep) {
    return caut_status_invalid_tag;
  }

  STATUS_CHECK(encode_tag8(_c_iter, &_temp_tag));

  switch(_c_obj->_tag) {
  case res_sleep_tag_not_connected: /* No data for field not_connected with index 0. */ break;
  case res_sleep_tag_keep_awake: /* No data for field keep_awake with index 1. */ break;
  case res_sleep_tag_connection: STATUS_CHECK(encode_connection(_c_iter, &_c_obj->connection)); break;
  }

  return caut_status_ok;
}

R encode_cmd_sleep(EI * const _c_iter, struct cmd_sleep const * const _c_obj) {
  caut_tag8_t _temp_tag = (caut_tag8_t)_c_obj->_tag;

  if (_temp_tag >= UNION_NUM_FIELDS_cmd_sleep) {
    return caut_status_invalid_tag;
  }

  STATUS_CHECK(encode_tag8(_c_iter, &_temp_tag));

  switch(_c_obj->_tag) {
  case cmd_sleep_tag_req: /* No data for field req with index 0. */ break;
  case cmd_sleep_tag_res: STATUS_CHECK(encode_res_sleep(_c_iter, &_c_obj->res)); break;
  }

  return caut_status_ok;
}

R encode_cmd(EI * const _c_iter, struct cmd const * const _c_obj) {
  caut_tag8_t _temp_tag = (caut_tag8_t)_c_obj->_tag;

  if (_temp_tag >= UNION_NUM_FIELDS_cmd) {
    return caut_status_invalid_tag;
  }

  STATUS_CHECK(encode_tag8(_c_iter, &_temp_tag));

  switch(_c_obj->_tag) {
  case cmd_tag_info: STATUS_CHECK(encode_cmd_info(_c_iter, &_c_obj->info)); break;
  case cmd_tag_send: STATUS_CHECK(encode_cmd_send(_c_iter, &_c_obj->send)); break;
  case cmd_tag_connect: STATUS_CHECK(encode_cmd_connect(_c_iter, &_c_obj->connect)); break;
  case cmd_tag_connected: STATUS_CHECK(encode_cmd_connected(_c_iter, &_c_obj->connected)); break;
  case cmd_tag_poll: STATUS_CHECK(encode_cmd_poll(_c_iter, &_c_obj->poll)); break;
  case cmd_tag_sleep: STATUS_CHECK(encode_cmd_sleep(_c_iter, &_c_obj->sleep)); break;
  case cmd_tag_baud: STATUS_CHECK(encode_cmd_baud(_c_iter, &_c_obj->baud)); break;
  case cmd_tag_logging: STATUS_CHECK(encode_cmd_logging(_c_iter, &_c_obj->logging)); break;
  }

  return caut_status_ok;
}

R encode_txn(EI * const _c_iter, struct txn const * const _c_obj) {
  STATUS_CHECK(encode_u16(_c_iter, &_c_obj->seq));
  STATUS_CHECK(encode_bool(_c_iter, &_c_obj->needs_reset));
  STATUS_CHECK(encode_cmd(_c_iter, &_c_obj->cmd));

  return caut_status_ok;
}



/* type decoders */
R decode_res_send(DI * const _c_iter, enum res_send * const _c_obj) {
  caut_tag8_t _c_tag;
  STATUS_CHECK(decode_tag8(_c_iter, &_c_tag));

  if (ENUM_MAX_VAL_res_send < _c_tag) {
    return caut_status_enumeration_out_of_range;
  }

  *_c_obj = (enum res_send)_c_tag;

  return caut_status_ok;
}

R decode_res_info(DI * const _c_iter, struct res_info * const _c_obj) {
  STATUS_CHECK(decode_u64(_c_iter, &_c_obj->mac));
  STATUS_CHECK(decode_u32(_c_iter, &_c_obj->uptime));
  STATUS_CHECK(decode_u32(_c_iter, &_c_obj->time));
  STATUS_CHECK(decode_u32(_c_iter, &_c_obj->fw_version));
  STATUS_CHECK(decode_u8(_c_iter, &_c_obj->radio_count));

  return caut_status_ok;
}

R decode_res_connect(DI * const _c_iter, enum res_connect * const _c_obj) {
  caut_tag8_t _c_tag;
  STATUS_CHECK(decode_tag8(_c_iter, &_c_tag));

  if (ENUM_MAX_VAL_res_connect < _c_tag) {
    return caut_status_enumeration_out_of_range;
  }

  *_c_obj = (enum res_connect)_c_tag;

  return caut_status_ok;
}

R decode_req_logging(DI * const _c_iter, enum req_logging * const _c_obj) {
  caut_tag8_t _c_tag;
  STATUS_CHECK(decode_tag8(_c_iter, &_c_tag));

  if (ENUM_MAX_VAL_req_logging < _c_tag) {
    return caut_status_enumeration_out_of_range;
  }

  *_c_obj = (enum req_logging)_c_tag;

  return caut_status_ok;
}

R decode_frame_app(DI * const _c_iter, struct frame_app * const _c_obj) {
  STATUS_CHECK(decode_tag8(_c_iter, &_c_obj->_length));

  if (_c_obj->_length > VECTOR_MAX_LEN_frame_app) {
    return caut_status_invalid_length;
  }

  for (size_t _c_i = 0; _c_i < _c_obj->_length; _c_i++) {
    STATUS_CHECK(decode_u8(_c_iter, &_c_obj->elems[_c_i]));
  }

  return caut_status_ok;
}

R decode_res_poll(DI * const _c_iter, struct res_poll * const _c_obj) {
  caut_tag8_t _temp_tag;
  STATUS_CHECK(decode_tag8(_c_iter, &_temp_tag));

  if (_temp_tag >= UNION_NUM_FIELDS_res_poll) {
    return caut_status_invalid_tag;
  } else {
    _c_obj->_tag = (enum res_poll_tag)_temp_tag;
  }

  switch(_c_obj->_tag) {
  case res_poll_tag_none: /* No data for field i"none" with index 0. */ break;
  case res_poll_tag_frame: STATUS_CHECK(decode_frame_app(_c_iter, &_c_obj->frame)); break;
  case res_poll_tag_log: STATUS_CHECK(decode_frame_app(_c_iter, &_c_obj->log)); break;
  }

  return caut_status_ok;
}

R decode_cmd_send(DI * const _c_iter, struct cmd_send * const _c_obj) {
  caut_tag8_t _temp_tag;
  STATUS_CHECK(decode_tag8(_c_iter, &_temp_tag));

  if (_temp_tag >= UNION_NUM_FIELDS_cmd_send) {
    return caut_status_invalid_tag;
  } else {
    _c_obj->_tag = (enum cmd_send_tag)_temp_tag;
  }

  switch(_c_obj->_tag) {
  case cmd_send_tag_req: STATUS_CHECK(decode_frame_app(_c_iter, &_c_obj->req)); break;
  case cmd_send_tag_res: STATUS_CHECK(decode_res_send(_c_iter, &_c_obj->res)); break;
  }

  return caut_status_ok;
}

R decode_cmd_poll(DI * const _c_iter, struct cmd_poll * const _c_obj) {
  caut_tag8_t _temp_tag;
  STATUS_CHECK(decode_tag8(_c_iter, &_temp_tag));

  if (_temp_tag >= UNION_NUM_FIELDS_cmd_poll) {
    return caut_status_invalid_tag;
  } else {
    _c_obj->_tag = (enum cmd_poll_tag)_temp_tag;
  }

  switch(_c_obj->_tag) {
  case cmd_poll_tag_req: STATUS_CHECK(decode_frame_app(_c_iter, &_c_obj->req)); break;
  case cmd_poll_tag_res: STATUS_CHECK(decode_res_poll(_c_iter, &_c_obj->res)); break;
  }

  return caut_status_ok;
}

R decode_cmd_mac(DI * const _c_iter, struct cmd_mac * const _c_obj) {
  caut_tag8_t _temp_tag;
  STATUS_CHECK(decode_tag8(_c_iter, &_temp_tag));

  if (_temp_tag >= UNION_NUM_FIELDS_cmd_mac) {
    return caut_status_invalid_tag;
  } else {
    _c_obj->_tag = (enum cmd_mac_tag)_temp_tag;
  }

  switch(_c_obj->_tag) {
  case cmd_mac_tag_req: /* No data for field i"req" with index 0. */ break;
  case cmd_mac_tag_res: STATUS_CHECK(decode_u64(_c_iter, &_c_obj->res)); break;
  }

  return caut_status_ok;
}

R decode_cmd_logging(DI * const _c_iter, struct cmd_logging * const _c_obj) {
  caut_tag8_t _temp_tag;
  STATUS_CHECK(decode_tag8(_c_iter, &_temp_tag));

  if (_temp_tag >= UNION_NUM_FIELDS_cmd_logging) {
    return caut_status_invalid_tag;
  } else {
    _c_obj->_tag = (enum cmd_logging_tag)_temp_tag;
  }

  switch(_c_obj->_tag) {
  case cmd_logging_tag_req: STATUS_CHECK(decode_req_logging(_c_iter, &_c_obj->req)); break;
  case cmd_logging_tag_res: /* No data for field i"res" with index 1. */ break;
  }

  return caut_status_ok;
}

R decode_cmd_info(DI * const _c_iter, struct cmd_info * const _c_obj) {
  caut_tag8_t _temp_tag;
  STATUS_CHECK(decode_tag8(_c_iter, &_temp_tag));

  if (_temp_tag >= UNION_NUM_FIELDS_cmd_info) {
    return caut_status_invalid_tag;
  } else {
    _c_obj->_tag = (enum cmd_info_tag)_temp_tag;
  }

  switch(_c_obj->_tag) {
  case cmd_info_tag_req: /* No data for field i"req" with index 0. */ break;
  case cmd_info_tag_res: STATUS_CHECK(decode_res_info(_c_iter, &_c_obj->res)); break;
  }

  return caut_status_ok;
}

R decode_cmd_connected(DI * const _c_iter, struct cmd_connected * const _c_obj) {
  caut_tag8_t _temp_tag;
  STATUS_CHECK(decode_tag8(_c_iter, &_temp_tag));

  if (_temp_tag >= UNION_NUM_FIELDS_cmd_connected) {
    return caut_status_invalid_tag;
  } else {
    _c_obj->_tag = (enum cmd_connected_tag)_temp_tag;
  }

  switch(_c_obj->_tag) {
  case cmd_connected_tag_req: /* No data for field i"req" with index 0. */ break;
  case cmd_connected_tag_res: STATUS_CHECK(decode_bool(_c_iter, &_c_obj->res)); break;
  }

  return caut_status_ok;
}

R decode_atom_baud(DI * const _c_iter, enum atom_baud * const _c_obj) {
  caut_tag8_t _c_tag;
  STATUS_CHECK(decode_tag8(_c_iter, &_c_tag));

  if (ENUM_MAX_VAL_atom_baud < _c_tag) {
    return caut_status_enumeration_out_of_range;
  }

  *_c_obj = (enum atom_baud)_c_tag;

  return caut_status_ok;
}

R decode_cmd_baud(DI * const _c_iter, struct cmd_baud * const _c_obj) {
  caut_tag8_t _temp_tag;
  STATUS_CHECK(decode_tag8(_c_iter, &_temp_tag));

  if (_temp_tag >= UNION_NUM_FIELDS_cmd_baud) {
    return caut_status_invalid_tag;
  } else {
    _c_obj->_tag = (enum cmd_baud_tag)_temp_tag;
  }

  switch(_c_obj->_tag) {
  case cmd_baud_tag_req: STATUS_CHECK(decode_atom_baud(_c_iter, &_c_obj->req)); break;
  case cmd_baud_tag_res: /* No data for field i"res" with index 1. */ break;
  }

  return caut_status_ok;
}

R decode_arr_u8_32(DI * const _c_iter, struct arr_u8_32 * const _c_obj) {
  for (size_t _c_i = 0; _c_i < ARR_LEN(_c_obj->elems); _c_i++) {
    STATUS_CHECK(decode_u8(_c_iter, &_c_obj->elems[_c_i]));
  }

  return caut_status_ok;
}

R decode_connection(DI * const _c_iter, struct connection * const _c_obj) {
  STATUS_CHECK(decode_u64(_c_iter, &_c_obj->long_addr));
  STATUS_CHECK(decode_u32(_c_iter, &_c_obj->fw_version));
  STATUS_CHECK(decode_u32(_c_iter, &_c_obj->time));
  STATUS_CHECK(decode_u16(_c_iter, &_c_obj->pan_id));
  STATUS_CHECK(decode_u16(_c_iter, &_c_obj->short_addr));
  STATUS_CHECK(decode_u16(_c_iter, &_c_obj->gateway_addr));
  STATUS_CHECK(decode_u8(_c_iter, &_c_obj->connected));
  STATUS_CHECK(decode_u8(_c_iter, &_c_obj->pan_seq));
  STATUS_CHECK(decode_u8(_c_iter, &_c_obj->radio_index));
  STATUS_CHECK(decode_u8(_c_iter, &_c_obj->channel));
  STATUS_CHECK(decode_u8(_c_iter, &_c_obj->speed));
  STATUS_CHECK(decode_u8(_c_iter, &_c_obj->key_slot));
  STATUS_CHECK(decode_arr_u8_32(_c_iter, &_c_obj->sess_key));

  return caut_status_ok;
}

R decode_req_connect(DI * const _c_iter, struct req_connect * const _c_obj) {
  caut_tag8_t _temp_tag;
  STATUS_CHECK(decode_tag8(_c_iter, &_temp_tag));

  if (_temp_tag >= UNION_NUM_FIELDS_req_connect) {
    return caut_status_invalid_tag;
  } else {
    _c_obj->_tag = (enum req_connect_tag)_temp_tag;
  }

  switch(_c_obj->_tag) {
  case req_connect_tag_cold: /* No data for field i"cold" with index 0. */ break;
  case req_connect_tag_quick: STATUS_CHECK(decode_connection(_c_iter, &_c_obj->quick)); break;
  }

  return caut_status_ok;
}

R decode_cmd_connect(DI * const _c_iter, struct cmd_connect * const _c_obj) {
  caut_tag8_t _temp_tag;
  STATUS_CHECK(decode_tag8(_c_iter, &_temp_tag));

  if (_temp_tag >= UNION_NUM_FIELDS_cmd_connect) {
    return caut_status_invalid_tag;
  } else {
    _c_obj->_tag = (enum cmd_connect_tag)_temp_tag;
  }

  switch(_c_obj->_tag) {
  case cmd_connect_tag_req: STATUS_CHECK(decode_req_connect(_c_iter, &_c_obj->req)); break;
  case cmd_connect_tag_res: STATUS_CHECK(decode_res_connect(_c_iter, &_c_obj->res)); break;
  }

  return caut_status_ok;
}

R decode_res_sleep(DI * const _c_iter, struct res_sleep * const _c_obj) {
  caut_tag8_t _temp_tag;
  STATUS_CHECK(decode_tag8(_c_iter, &_temp_tag));

  if (_temp_tag >= UNION_NUM_FIELDS_res_sleep) {
    return caut_status_invalid_tag;
  } else {
    _c_obj->_tag = (enum res_sleep_tag)_temp_tag;
  }

  switch(_c_obj->_tag) {
  case res_sleep_tag_not_connected: /* No data for field i"not_connected" with index 0. */ break;
  case res_sleep_tag_keep_awake: /* No data for field i"keep_awake" with index 1. */ break;
  case res_sleep_tag_connection: STATUS_CHECK(decode_connection(_c_iter, &_c_obj->connection)); break;
  }

  return caut_status_ok;
}

R decode_cmd_sleep(DI * const _c_iter, struct cmd_sleep * const _c_obj) {
  caut_tag8_t _temp_tag;
  STATUS_CHECK(decode_tag8(_c_iter, &_temp_tag));

  if (_temp_tag >= UNION_NUM_FIELDS_cmd_sleep) {
    return caut_status_invalid_tag;
  } else {
    _c_obj->_tag = (enum cmd_sleep_tag)_temp_tag;
  }

  switch(_c_obj->_tag) {
  case cmd_sleep_tag_req: /* No data for field i"req" with index 0. */ break;
  case cmd_sleep_tag_res: STATUS_CHECK(decode_res_sleep(_c_iter, &_c_obj->res)); break;
  }

  return caut_status_ok;
}

R decode_cmd(DI * const _c_iter, struct cmd * const _c_obj) {
  caut_tag8_t _temp_tag;
  STATUS_CHECK(decode_tag8(_c_iter, &_temp_tag));

  if (_temp_tag >= UNION_NUM_FIELDS_cmd) {
    return caut_status_invalid_tag;
  } else {
    _c_obj->_tag = (enum cmd_tag)_temp_tag;
  }

  switch(_c_obj->_tag) {
  case cmd_tag_info: STATUS_CHECK(decode_cmd_info(_c_iter, &_c_obj->info)); break;
  case cmd_tag_send: STATUS_CHECK(decode_cmd_send(_c_iter, &_c_obj->send)); break;
  case cmd_tag_connect: STATUS_CHECK(decode_cmd_connect(_c_iter, &_c_obj->connect)); break;
  case cmd_tag_connected: STATUS_CHECK(decode_cmd_connected(_c_iter, &_c_obj->connected)); break;
  case cmd_tag_poll: STATUS_CHECK(decode_cmd_poll(_c_iter, &_c_obj->poll)); break;
  case cmd_tag_sleep: STATUS_CHECK(decode_cmd_sleep(_c_iter, &_c_obj->sleep)); break;
  case cmd_tag_baud: STATUS_CHECK(decode_cmd_baud(_c_iter, &_c_obj->baud)); break;
  case cmd_tag_logging: STATUS_CHECK(decode_cmd_logging(_c_iter, &_c_obj->logging)); break;
  }

  return caut_status_ok;
}

R decode_txn(DI * const _c_iter, struct txn * const _c_obj) {
  STATUS_CHECK(decode_u16(_c_iter, &_c_obj->seq));
  STATUS_CHECK(decode_bool(_c_iter, &_c_obj->needs_reset));
  STATUS_CHECK(decode_cmd(_c_iter, &_c_obj->cmd));

  return caut_status_ok;
}



/* type initializers */
void init_res_send(enum res_send * _c_obj) {
  *_c_obj = res_send_ok;
}

void init_res_info(struct res_info * _c_obj) {
  init_u64(&_c_obj->mac);
  init_u32(&_c_obj->uptime);
  init_u32(&_c_obj->time);
  init_u32(&_c_obj->fw_version);
  init_u8(&_c_obj->radio_count);
}

void init_res_connect(enum res_connect * _c_obj) {
  *_c_obj = res_connect_ok;
}

void init_req_logging(enum req_logging * _c_obj) {
  *_c_obj = req_logging_enable;
}

void init_frame_app(struct frame_app * _c_obj) {
  _c_obj->_length = 0;
}

void init_res_poll(struct res_poll * _c_obj) {
  _c_obj->_tag = (caut_tag8_t) res_poll_tag_none;
  /* No initializer for empty field none */
}

void init_cmd_send(struct cmd_send * _c_obj) {
  _c_obj->_tag = (caut_tag8_t) cmd_send_tag_req;
  init_frame_app(&_c_obj->req);
}

void init_cmd_poll(struct cmd_poll * _c_obj) {
  _c_obj->_tag = (caut_tag8_t) cmd_poll_tag_req;
  init_frame_app(&_c_obj->req);
}

void init_cmd_mac(struct cmd_mac * _c_obj) {
  _c_obj->_tag = (caut_tag8_t) cmd_mac_tag_req;
  /* No initializer for empty field req */
}

void init_cmd_logging(struct cmd_logging * _c_obj) {
  _c_obj->_tag = (caut_tag8_t) cmd_logging_tag_req;
  init_req_logging(&_c_obj->req);
}

void init_cmd_info(struct cmd_info * _c_obj) {
  _c_obj->_tag = (caut_tag8_t) cmd_info_tag_req;
  /* No initializer for empty field req */
}

void init_cmd_connected(struct cmd_connected * _c_obj) {
  _c_obj->_tag = (caut_tag8_t) cmd_connected_tag_req;
  /* No initializer for empty field req */
}

void init_atom_baud(enum atom_baud * _c_obj) {
  *_c_obj = atom_baud_b9600;
}

void init_cmd_baud(struct cmd_baud * _c_obj) {
  _c_obj->_tag = (caut_tag8_t) cmd_baud_tag_req;
  init_atom_baud(&_c_obj->req);
}

void init_arr_u8_32(struct arr_u8_32 * _c_obj) {
  for (size_t _c_i = 0; _c_i < ARR_LEN(_c_obj->elems); _c_i++) {
    init_u8(&_c_obj->elems[_c_i]);
  }
}

void init_connection(struct connection * _c_obj) {
  init_u64(&_c_obj->long_addr);
  init_u32(&_c_obj->fw_version);
  init_u32(&_c_obj->time);
  init_u16(&_c_obj->pan_id);
  init_u16(&_c_obj->short_addr);
  init_u16(&_c_obj->gateway_addr);
  init_u8(&_c_obj->connected);
  init_u8(&_c_obj->pan_seq);
  init_u8(&_c_obj->radio_index);
  init_u8(&_c_obj->channel);
  init_u8(&_c_obj->speed);
  init_u8(&_c_obj->key_slot);
  init_arr_u8_32(&_c_obj->sess_key);
}

void init_req_connect(struct req_connect * _c_obj) {
  _c_obj->_tag = (caut_tag8_t) req_connect_tag_cold;
  /* No initializer for empty field cold */
}

void init_cmd_connect(struct cmd_connect * _c_obj) {
  _c_obj->_tag = (caut_tag8_t) cmd_connect_tag_req;
  init_req_connect(&_c_obj->req);
}

void init_res_sleep(struct res_sleep * _c_obj) {
  _c_obj->_tag = (caut_tag8_t) res_sleep_tag_not_connected;
  /* No initializer for empty field not_connected */
}

void init_cmd_sleep(struct cmd_sleep * _c_obj) {
  _c_obj->_tag = (caut_tag8_t) cmd_sleep_tag_req;
  /* No initializer for empty field req */
}

void init_cmd(struct cmd * _c_obj) {
  _c_obj->_tag = (caut_tag8_t) cmd_tag_info;
  init_cmd_info(&_c_obj->info);
}

void init_txn(struct txn * _c_obj) {
  init_u16(&_c_obj->seq);
  init_bool(&_c_obj->needs_reset);
  init_cmd(&_c_obj->cmd);
}



/* type comparators */
enum caut_ord compare_res_send(enum res_send const * const _c_a, enum res_send const * const _c_b) {
  return CAUT_ORDER(*_c_a, *_c_b);
}

enum caut_ord compare_res_info(struct res_info const * const _c_a, struct res_info const * const _c_b) {
  enum caut_ord _c_o;

  if (caut_ord_eq != (_c_o = compare_u64(&_c_a->mac, &_c_b->mac))) { return _c_o; }
  if (caut_ord_eq != (_c_o = compare_u32(&_c_a->uptime, &_c_b->uptime))) { return _c_o; }
  if (caut_ord_eq != (_c_o = compare_u32(&_c_a->time, &_c_b->time))) { return _c_o; }
  if (caut_ord_eq != (_c_o = compare_u32(&_c_a->fw_version, &_c_b->fw_version))) { return _c_o; }
  if (caut_ord_eq != (_c_o = compare_u8(&_c_a->radio_count, &_c_b->radio_count))) { return _c_o; }

  return caut_ord_eq;

}

enum caut_ord compare_res_connect(enum res_connect const * const _c_a, enum res_connect const * const _c_b) {
  return CAUT_ORDER(*_c_a, *_c_b);
}

enum caut_ord compare_req_logging(enum req_logging const * const _c_a, enum req_logging const * const _c_b) {
  return CAUT_ORDER(*_c_a, *_c_b);
}

enum caut_ord compare_frame_app(struct frame_app const * const _c_a, struct frame_app const * const _c_b) {
  for (size_t _c_i = 0; _c_i < _c_a->_length && _c_i < _c_b->_length ; _c_i++) {
    const enum caut_ord _c_o = compare_u8(&_c_a->elems[_c_i], &_c_b->elems[_c_i]);
    if (caut_ord_eq != _c_o) {
      return _c_o;
    }
  }

  return CAUT_ORDER(_c_a->_length, _c_b->_length);
}

enum caut_ord compare_res_poll(struct res_poll const * const _c_a, struct res_poll const * const _c_b) {
  enum caut_ord _c_o;

  if (caut_ord_eq != (_c_o = CAUT_ORDER(_c_a->_tag, _c_b->_tag))) {
    return _c_o;
  }

  switch(_c_a->_tag) {
  case res_poll_tag_none:
    _c_o = caut_ord_eq; /* No comparison for empty field none */
    break;
  case res_poll_tag_frame:
    _c_o = compare_frame_app(&_c_a->frame, &_c_b->frame);
    break;
  case res_poll_tag_log:
    _c_o = compare_frame_app(&_c_a->log, &_c_b->log);
    break;
  }

  return _c_o;
}

enum caut_ord compare_cmd_send(struct cmd_send const * const _c_a, struct cmd_send const * const _c_b) {
  enum caut_ord _c_o;

  if (caut_ord_eq != (_c_o = CAUT_ORDER(_c_a->_tag, _c_b->_tag))) {
    return _c_o;
  }

  switch(_c_a->_tag) {
  case cmd_send_tag_req:
    _c_o = compare_frame_app(&_c_a->req, &_c_b->req);
    break;
  case cmd_send_tag_res:
    _c_o = compare_res_send(&_c_a->res, &_c_b->res);
    break;
  }

  return _c_o;
}

enum caut_ord compare_cmd_poll(struct cmd_poll const * const _c_a, struct cmd_poll const * const _c_b) {
  enum caut_ord _c_o;

  if (caut_ord_eq != (_c_o = CAUT_ORDER(_c_a->_tag, _c_b->_tag))) {
    return _c_o;
  }

  switch(_c_a->_tag) {
  case cmd_poll_tag_req:
    _c_o = compare_frame_app(&_c_a->req, &_c_b->req);
    break;
  case cmd_poll_tag_res:
    _c_o = compare_res_poll(&_c_a->res, &_c_b->res);
    break;
  }

  return _c_o;
}

enum caut_ord compare_cmd_mac(struct cmd_mac const * const _c_a, struct cmd_mac const * const _c_b) {
  enum caut_ord _c_o;

  if (caut_ord_eq != (_c_o = CAUT_ORDER(_c_a->_tag, _c_b->_tag))) {
    return _c_o;
  }

  switch(_c_a->_tag) {
  case cmd_mac_tag_req:
    _c_o = caut_ord_eq; /* No comparison for empty field req */
    break;
  case cmd_mac_tag_res:
    _c_o = compare_u64(&_c_a->res, &_c_b->res);
    break;
  }

  return _c_o;
}

enum caut_ord compare_cmd_logging(struct cmd_logging const * const _c_a, struct cmd_logging const * const _c_b) {
  enum caut_ord _c_o;

  if (caut_ord_eq != (_c_o = CAUT_ORDER(_c_a->_tag, _c_b->_tag))) {
    return _c_o;
  }

  switch(_c_a->_tag) {
  case cmd_logging_tag_req:
    _c_o = compare_req_logging(&_c_a->req, &_c_b->req);
    break;
  case cmd_logging_tag_res:
    _c_o = caut_ord_eq; /* No comparison for empty field res */
    break;
  }

  return _c_o;
}

enum caut_ord compare_cmd_info(struct cmd_info const * const _c_a, struct cmd_info const * const _c_b) {
  enum caut_ord _c_o;

  if (caut_ord_eq != (_c_o = CAUT_ORDER(_c_a->_tag, _c_b->_tag))) {
    return _c_o;
  }

  switch(_c_a->_tag) {
  case cmd_info_tag_req:
    _c_o = caut_ord_eq; /* No comparison for empty field req */
    break;
  case cmd_info_tag_res:
    _c_o = compare_res_info(&_c_a->res, &_c_b->res);
    break;
  }

  return _c_o;
}

enum caut_ord compare_cmd_connected(struct cmd_connected const * const _c_a, struct cmd_connected const * const _c_b) {
  enum caut_ord _c_o;

  if (caut_ord_eq != (_c_o = CAUT_ORDER(_c_a->_tag, _c_b->_tag))) {
    return _c_o;
  }

  switch(_c_a->_tag) {
  case cmd_connected_tag_req:
    _c_o = caut_ord_eq; /* No comparison for empty field req */
    break;
  case cmd_connected_tag_res:
    _c_o = compare_bool(&_c_a->res, &_c_b->res);
    break;
  }

  return _c_o;
}

enum caut_ord compare_atom_baud(enum atom_baud const * const _c_a, enum atom_baud const * const _c_b) {
  return CAUT_ORDER(*_c_a, *_c_b);
}

enum caut_ord compare_cmd_baud(struct cmd_baud const * const _c_a, struct cmd_baud const * const _c_b) {
  enum caut_ord _c_o;

  if (caut_ord_eq != (_c_o = CAUT_ORDER(_c_a->_tag, _c_b->_tag))) {
    return _c_o;
  }

  switch(_c_a->_tag) {
  case cmd_baud_tag_req:
    _c_o = compare_atom_baud(&_c_a->req, &_c_b->req);
    break;
  case cmd_baud_tag_res:
    _c_o = caut_ord_eq; /* No comparison for empty field res */
    break;
  }

  return _c_o;
}

enum caut_ord compare_arr_u8_32(struct arr_u8_32 const * const _c_a, struct arr_u8_32 const * const _c_b) {
  for (size_t _c_i = 0; _c_i < ARR_LEN(_c_a->elems); _c_i++) {
    const enum caut_ord _c_o = compare_u8(&_c_a->elems[_c_i], &_c_b->elems[_c_i]);
    if (caut_ord_eq != _c_o) {
      return _c_o;
    }
  }

  return caut_ord_eq;
}

enum caut_ord compare_connection(struct connection const * const _c_a, struct connection const * const _c_b) {
  enum caut_ord _c_o;

  if (caut_ord_eq != (_c_o = compare_u64(&_c_a->long_addr, &_c_b->long_addr))) { return _c_o; }
  if (caut_ord_eq != (_c_o = compare_u32(&_c_a->fw_version, &_c_b->fw_version))) { return _c_o; }
  if (caut_ord_eq != (_c_o = compare_u32(&_c_a->time, &_c_b->time))) { return _c_o; }
  if (caut_ord_eq != (_c_o = compare_u16(&_c_a->pan_id, &_c_b->pan_id))) { return _c_o; }
  if (caut_ord_eq != (_c_o = compare_u16(&_c_a->short_addr, &_c_b->short_addr))) { return _c_o; }
  if (caut_ord_eq != (_c_o = compare_u16(&_c_a->gateway_addr, &_c_b->gateway_addr))) { return _c_o; }
  if (caut_ord_eq != (_c_o = compare_u8(&_c_a->connected, &_c_b->connected))) { return _c_o; }
  if (caut_ord_eq != (_c_o = compare_u8(&_c_a->pan_seq, &_c_b->pan_seq))) { return _c_o; }
  if (caut_ord_eq != (_c_o = compare_u8(&_c_a->radio_index, &_c_b->radio_index))) { return _c_o; }
  if (caut_ord_eq != (_c_o = compare_u8(&_c_a->channel, &_c_b->channel))) { return _c_o; }
  if (caut_ord_eq != (_c_o = compare_u8(&_c_a->speed, &_c_b->speed))) { return _c_o; }
  if (caut_ord_eq != (_c_o = compare_u8(&_c_a->key_slot, &_c_b->key_slot))) { return _c_o; }
  if (caut_ord_eq != (_c_o = compare_arr_u8_32(&_c_a->sess_key, &_c_b->sess_key))) { return _c_o; }

  return caut_ord_eq;

}

enum caut_ord compare_req_connect(struct req_connect const * const _c_a, struct req_connect const * const _c_b) {
  enum caut_ord _c_o;

  if (caut_ord_eq != (_c_o = CAUT_ORDER(_c_a->_tag, _c_b->_tag))) {
    return _c_o;
  }

  switch(_c_a->_tag) {
  case req_connect_tag_cold:
    _c_o = caut_ord_eq; /* No comparison for empty field cold */
    break;
  case req_connect_tag_quick:
    _c_o = compare_connection(&_c_a->quick, &_c_b->quick);
    break;
  }

  return _c_o;
}

enum caut_ord compare_cmd_connect(struct cmd_connect const * const _c_a, struct cmd_connect const * const _c_b) {
  enum caut_ord _c_o;

  if (caut_ord_eq != (_c_o = CAUT_ORDER(_c_a->_tag, _c_b->_tag))) {
    return _c_o;
  }

  switch(_c_a->_tag) {
  case cmd_connect_tag_req:
    _c_o = compare_req_connect(&_c_a->req, &_c_b->req);
    break;
  case cmd_connect_tag_res:
    _c_o = compare_res_connect(&_c_a->res, &_c_b->res);
    break;
  }

  return _c_o;
}

enum caut_ord compare_res_sleep(struct res_sleep const * const _c_a, struct res_sleep const * const _c_b) {
  enum caut_ord _c_o;

  if (caut_ord_eq != (_c_o = CAUT_ORDER(_c_a->_tag, _c_b->_tag))) {
    return _c_o;
  }

  switch(_c_a->_tag) {
  case res_sleep_tag_not_connected:
    _c_o = caut_ord_eq; /* No comparison for empty field not_connected */
    break;
  case res_sleep_tag_keep_awake:
    _c_o = caut_ord_eq; /* No comparison for empty field keep_awake */
    break;
  case res_sleep_tag_connection:
    _c_o = compare_connection(&_c_a->connection, &_c_b->connection);
    break;
  }

  return _c_o;
}

enum caut_ord compare_cmd_sleep(struct cmd_sleep const * const _c_a, struct cmd_sleep const * const _c_b) {
  enum caut_ord _c_o;

  if (caut_ord_eq != (_c_o = CAUT_ORDER(_c_a->_tag, _c_b->_tag))) {
    return _c_o;
  }

  switch(_c_a->_tag) {
  case cmd_sleep_tag_req:
    _c_o = caut_ord_eq; /* No comparison for empty field req */
    break;
  case cmd_sleep_tag_res:
    _c_o = compare_res_sleep(&_c_a->res, &_c_b->res);
    break;
  }

  return _c_o;
}

enum caut_ord compare_cmd(struct cmd const * const _c_a, struct cmd const * const _c_b) {
  enum caut_ord _c_o;

  if (caut_ord_eq != (_c_o = CAUT_ORDER(_c_a->_tag, _c_b->_tag))) {
    return _c_o;
  }

  switch(_c_a->_tag) {
  case cmd_tag_info:
    _c_o = compare_cmd_info(&_c_a->info, &_c_b->info);
    break;
  case cmd_tag_send:
    _c_o = compare_cmd_send(&_c_a->send, &_c_b->send);
    break;
  case cmd_tag_connect:
    _c_o = compare_cmd_connect(&_c_a->connect, &_c_b->connect);
    break;
  case cmd_tag_connected:
    _c_o = compare_cmd_connected(&_c_a->connected, &_c_b->connected);
    break;
  case cmd_tag_poll:
    _c_o = compare_cmd_poll(&_c_a->poll, &_c_b->poll);
    break;
  case cmd_tag_sleep:
    _c_o = compare_cmd_sleep(&_c_a->sleep, &_c_b->sleep);
    break;
  case cmd_tag_baud:
    _c_o = compare_cmd_baud(&_c_a->baud, &_c_b->baud);
    break;
  case cmd_tag_logging:
    _c_o = compare_cmd_logging(&_c_a->logging, &_c_b->logging);
    break;
  }

  return _c_o;
}

enum caut_ord compare_txn(struct txn const * const _c_a, struct txn const * const _c_b) {
  enum caut_ord _c_o;

  if (caut_ord_eq != (_c_o = compare_u16(&_c_a->seq, &_c_b->seq))) { return _c_o; }
  if (caut_ord_eq != (_c_o = compare_bool(&_c_a->needs_reset, &_c_b->needs_reset))) { return _c_o; }
  if (caut_ord_eq != (_c_o = compare_cmd(&_c_a->cmd, &_c_b->cmd))) { return _c_o; }

  return caut_ord_eq;

}


#undef R
#undef I

