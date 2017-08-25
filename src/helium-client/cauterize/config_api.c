#include "config_api.h"

#define R enum caut_status
#define EI struct caut_encode_iter
#define DI struct caut_decode_iter
#define FSET(FS,IX) ((FS) & (1ull << (IX)))

/* schema hash */
hashtype_t const SCHEMA_HASH_config_api = { 0x82,0x0C,0x22,0xDB,0xDF,0xAF,0x52,0x86,0x09,0xA7,0xD4,0xB4,0x33,0x23,0xB4,0x80,0x7A,0xD8,0xFB,0x8F };

/* type encoders */
R encode_string(EI * const _c_iter, struct string const * const _c_obj) {
  if (_c_obj->_length > VECTOR_MAX_LEN_string) {
    return caut_status_invalid_length;
  }

  STATUS_CHECK(encode_tag8(_c_iter, &_c_obj->_length));

  for (size_t _c_i = 0; _c_i < _c_obj->_length; _c_i++) {
    STATUS_CHECK(encode_u8(_c_iter, &_c_obj->elems[_c_i]));
  }

  return caut_status_ok;
}

R encode_value(EI * const _c_iter, struct value const * const _c_obj) {
  caut_tag8_t _temp_tag = (caut_tag8_t)_c_obj->_tag;

  if (_temp_tag >= UNION_NUM_FIELDS_value) {
    return caut_status_invalid_tag;
  }

  STATUS_CHECK(encode_tag8(_c_iter, &_temp_tag));

  switch(_c_obj->_tag) {
  case value_tag_b: STATUS_CHECK(encode_bool(_c_iter, &_c_obj->b)); break;
  case value_tag_i: STATUS_CHECK(encode_s32(_c_iter, &_c_obj->i)); break;
  case value_tag_f: STATUS_CHECK(encode_f32(_c_iter, &_c_obj->f)); break;
  case value_tag_s: STATUS_CHECK(encode_string(_c_iter, &_c_obj->s)); break;
  case value_tag_n: /* No data for field n with index 4. */ break;
  }

  return caut_status_ok;
}

R encode_key(EI * const _c_iter, struct key const * const _c_obj) {
  if (_c_obj->_length > VECTOR_MAX_LEN_key) {
    return caut_status_invalid_length;
  }

  STATUS_CHECK(encode_tag8(_c_iter, &_c_obj->_length));

  for (size_t _c_i = 0; _c_i < _c_obj->_length; _c_i++) {
    STATUS_CHECK(encode_u8(_c_iter, &_c_obj->elems[_c_i]));
  }

  return caut_status_ok;
}

R encode_assoc(EI * const _c_iter, struct assoc const * const _c_obj) {
  STATUS_CHECK(encode_key(_c_iter, &_c_obj->k));
  STATUS_CHECK(encode_value(_c_iter, &_c_obj->v));

  return caut_status_ok;
}

R encode_arr_key(EI * const _c_iter, struct arr_key const * const _c_obj) {
  if (_c_obj->_length > VECTOR_MAX_LEN_arr_key) {
    return caut_status_invalid_length;
  }

  STATUS_CHECK(encode_tag8(_c_iter, &_c_obj->_length));

  for (size_t _c_i = 0; _c_i < _c_obj->_length; _c_i++) {
    STATUS_CHECK(encode_key(_c_iter, &_c_obj->elems[_c_i]));
  }

  return caut_status_ok;
}

R encode_arr_assoc(EI * const _c_iter, struct arr_assoc const * const _c_obj) {
  if (_c_obj->_length > VECTOR_MAX_LEN_arr_assoc) {
    return caut_status_invalid_length;
  }

  STATUS_CHECK(encode_tag8(_c_iter, &_c_obj->_length));

  for (size_t _c_i = 0; _c_i < _c_obj->_length; _c_i++) {
    STATUS_CHECK(encode_assoc(_c_iter, &_c_obj->elems[_c_i]));
  }

  return caut_status_ok;
}

R encode_cmd_config_get(EI * const _c_iter, struct cmd_config_get const * const _c_obj) {
  caut_tag8_t _temp_tag = (caut_tag8_t)_c_obj->_tag;

  if (_temp_tag >= UNION_NUM_FIELDS_cmd_config_get) {
    return caut_status_invalid_tag;
  }

  STATUS_CHECK(encode_tag8(_c_iter, &_temp_tag));

  switch(_c_obj->_tag) {
  case cmd_config_get_tag_req: STATUS_CHECK(encode_arr_key(_c_iter, &_c_obj->req)); break;
  case cmd_config_get_tag_res: STATUS_CHECK(encode_arr_assoc(_c_iter, &_c_obj->res)); break;
  }

  return caut_status_ok;
}

R encode_cmd_config_set(EI * const _c_iter, struct cmd_config_set const * const _c_obj) {
  caut_tag8_t _temp_tag = (caut_tag8_t)_c_obj->_tag;

  if (_temp_tag >= UNION_NUM_FIELDS_cmd_config_set) {
    return caut_status_invalid_tag;
  }

  STATUS_CHECK(encode_tag8(_c_iter, &_temp_tag));

  switch(_c_obj->_tag) {
  case cmd_config_set_tag_req: STATUS_CHECK(encode_arr_assoc(_c_iter, &_c_obj->req)); break;
  case cmd_config_set_tag_res: STATUS_CHECK(encode_u8(_c_iter, &_c_obj->res)); break;
  }

  return caut_status_ok;
}

R encode_cmd_config(EI * const _c_iter, struct cmd_config const * const _c_obj) {
  caut_tag8_t _temp_tag = (caut_tag8_t)_c_obj->_tag;

  if (_temp_tag >= UNION_NUM_FIELDS_cmd_config) {
    return caut_status_invalid_tag;
  }

  STATUS_CHECK(encode_tag8(_c_iter, &_temp_tag));

  switch(_c_obj->_tag) {
  case cmd_config_tag_get: STATUS_CHECK(encode_cmd_config_get(_c_iter, &_c_obj->get)); break;
  case cmd_config_tag_set: STATUS_CHECK(encode_cmd_config_set(_c_iter, &_c_obj->set)); break;
  case cmd_config_tag_invalidate: /* No data for field invalidate with index 2. */ break;
  }

  return caut_status_ok;
}



/* type decoders */
R decode_string(DI * const _c_iter, struct string * const _c_obj) {
  STATUS_CHECK(decode_tag8(_c_iter, &_c_obj->_length));

  if (_c_obj->_length > VECTOR_MAX_LEN_string) {
    return caut_status_invalid_length;
  }

  for (size_t _c_i = 0; _c_i < _c_obj->_length; _c_i++) {
    STATUS_CHECK(decode_u8(_c_iter, &_c_obj->elems[_c_i]));
  }

  return caut_status_ok;
}

R decode_value(DI * const _c_iter, struct value * const _c_obj) {
  caut_tag8_t _temp_tag;
  STATUS_CHECK(decode_tag8(_c_iter, &_temp_tag));

  if (_temp_tag >= UNION_NUM_FIELDS_value) {
    return caut_status_invalid_tag;
  } else {
    _c_obj->_tag = (enum value_tag)_temp_tag;
  }

  switch(_c_obj->_tag) {
  case value_tag_b: STATUS_CHECK(decode_bool(_c_iter, &_c_obj->b)); break;
  case value_tag_i: STATUS_CHECK(decode_s32(_c_iter, &_c_obj->i)); break;
  case value_tag_f: STATUS_CHECK(decode_f32(_c_iter, &_c_obj->f)); break;
  case value_tag_s: STATUS_CHECK(decode_string(_c_iter, &_c_obj->s)); break;
  case value_tag_n: /* No data for field i"n" with index 4. */ break;
  }

  return caut_status_ok;
}

R decode_key(DI * const _c_iter, struct key * const _c_obj) {
  STATUS_CHECK(decode_tag8(_c_iter, &_c_obj->_length));

  if (_c_obj->_length > VECTOR_MAX_LEN_key) {
    return caut_status_invalid_length;
  }

  for (size_t _c_i = 0; _c_i < _c_obj->_length; _c_i++) {
    STATUS_CHECK(decode_u8(_c_iter, &_c_obj->elems[_c_i]));
  }

  return caut_status_ok;
}

R decode_assoc(DI * const _c_iter, struct assoc * const _c_obj) {
  STATUS_CHECK(decode_key(_c_iter, &_c_obj->k));
  STATUS_CHECK(decode_value(_c_iter, &_c_obj->v));

  return caut_status_ok;
}

R decode_arr_key(DI * const _c_iter, struct arr_key * const _c_obj) {
  STATUS_CHECK(decode_tag8(_c_iter, &_c_obj->_length));

  if (_c_obj->_length > VECTOR_MAX_LEN_arr_key) {
    return caut_status_invalid_length;
  }

  for (size_t _c_i = 0; _c_i < _c_obj->_length; _c_i++) {
    STATUS_CHECK(decode_key(_c_iter, &_c_obj->elems[_c_i]));
  }

  return caut_status_ok;
}

R decode_arr_assoc(DI * const _c_iter, struct arr_assoc * const _c_obj) {
  STATUS_CHECK(decode_tag8(_c_iter, &_c_obj->_length));

  if (_c_obj->_length > VECTOR_MAX_LEN_arr_assoc) {
    return caut_status_invalid_length;
  }

  for (size_t _c_i = 0; _c_i < _c_obj->_length; _c_i++) {
    STATUS_CHECK(decode_assoc(_c_iter, &_c_obj->elems[_c_i]));
  }

  return caut_status_ok;
}

R decode_cmd_config_get(DI * const _c_iter, struct cmd_config_get * const _c_obj) {
  caut_tag8_t _temp_tag;
  STATUS_CHECK(decode_tag8(_c_iter, &_temp_tag));

  if (_temp_tag >= UNION_NUM_FIELDS_cmd_config_get) {
    return caut_status_invalid_tag;
  } else {
    _c_obj->_tag = (enum cmd_config_get_tag)_temp_tag;
  }

  switch(_c_obj->_tag) {
  case cmd_config_get_tag_req: STATUS_CHECK(decode_arr_key(_c_iter, &_c_obj->req)); break;
  case cmd_config_get_tag_res: STATUS_CHECK(decode_arr_assoc(_c_iter, &_c_obj->res)); break;
  }

  return caut_status_ok;
}

R decode_cmd_config_set(DI * const _c_iter, struct cmd_config_set * const _c_obj) {
  caut_tag8_t _temp_tag;
  STATUS_CHECK(decode_tag8(_c_iter, &_temp_tag));

  if (_temp_tag >= UNION_NUM_FIELDS_cmd_config_set) {
    return caut_status_invalid_tag;
  } else {
    _c_obj->_tag = (enum cmd_config_set_tag)_temp_tag;
  }

  switch(_c_obj->_tag) {
  case cmd_config_set_tag_req: STATUS_CHECK(decode_arr_assoc(_c_iter, &_c_obj->req)); break;
  case cmd_config_set_tag_res: STATUS_CHECK(decode_u8(_c_iter, &_c_obj->res)); break;
  }

  return caut_status_ok;
}

R decode_cmd_config(DI * const _c_iter, struct cmd_config * const _c_obj) {
  caut_tag8_t _temp_tag;
  STATUS_CHECK(decode_tag8(_c_iter, &_temp_tag));

  if (_temp_tag >= UNION_NUM_FIELDS_cmd_config) {
    return caut_status_invalid_tag;
  } else {
    _c_obj->_tag = (enum cmd_config_tag)_temp_tag;
  }

  switch(_c_obj->_tag) {
  case cmd_config_tag_get: STATUS_CHECK(decode_cmd_config_get(_c_iter, &_c_obj->get)); break;
  case cmd_config_tag_set: STATUS_CHECK(decode_cmd_config_set(_c_iter, &_c_obj->set)); break;
  case cmd_config_tag_invalidate: /* No data for field i"invalidate" with index 2. */ break;
  }

  return caut_status_ok;
}



/* type initializers */
void init_string(struct string * _c_obj) {
  _c_obj->_length = 0;
}

void init_value(struct value * _c_obj) {
  _c_obj->_tag = (caut_tag8_t) value_tag_b;
  init_bool(&_c_obj->b);
}

void init_key(struct key * _c_obj) {
  _c_obj->_length = 0;
}

void init_assoc(struct assoc * _c_obj) {
  init_key(&_c_obj->k);
  init_value(&_c_obj->v);
}

void init_arr_key(struct arr_key * _c_obj) {
  _c_obj->_length = 0;
}

void init_arr_assoc(struct arr_assoc * _c_obj) {
  _c_obj->_length = 0;
}

void init_cmd_config_get(struct cmd_config_get * _c_obj) {
  _c_obj->_tag = (caut_tag8_t) cmd_config_get_tag_req;
  init_arr_key(&_c_obj->req);
}

void init_cmd_config_set(struct cmd_config_set * _c_obj) {
  _c_obj->_tag = (caut_tag8_t) cmd_config_set_tag_req;
  init_arr_assoc(&_c_obj->req);
}

void init_cmd_config(struct cmd_config * _c_obj) {
  _c_obj->_tag = (caut_tag8_t) cmd_config_tag_get;
  init_cmd_config_get(&_c_obj->get);
}



/* type comparators */
enum caut_ord compare_string(struct string const * const _c_a, struct string const * const _c_b) {
  for (size_t _c_i = 0; _c_i < _c_a->_length && _c_i < _c_b->_length ; _c_i++) {
    const enum caut_ord _c_o = compare_u8(&_c_a->elems[_c_i], &_c_b->elems[_c_i]);
    if (caut_ord_eq != _c_o) {
      return _c_o;
    }
  }

  return CAUT_ORDER(_c_a->_length, _c_b->_length);
}

enum caut_ord compare_value(struct value const * const _c_a, struct value const * const _c_b) {
  enum caut_ord _c_o;

  if (caut_ord_eq != (_c_o = CAUT_ORDER(_c_a->_tag, _c_b->_tag))) {
    return _c_o;
  }

  switch(_c_a->_tag) {
  case value_tag_b:
    _c_o = compare_bool(&_c_a->b, &_c_b->b);
    break;
  case value_tag_i:
    _c_o = compare_s32(&_c_a->i, &_c_b->i);
    break;
  case value_tag_f:
    _c_o = compare_f32(&_c_a->f, &_c_b->f);
    break;
  case value_tag_s:
    _c_o = compare_string(&_c_a->s, &_c_b->s);
    break;
  case value_tag_n:
    _c_o = caut_ord_eq; /* No comparison for empty field n */
    break;
  }

  return _c_o;
}

enum caut_ord compare_key(struct key const * const _c_a, struct key const * const _c_b) {
  for (size_t _c_i = 0; _c_i < _c_a->_length && _c_i < _c_b->_length ; _c_i++) {
    const enum caut_ord _c_o = compare_u8(&_c_a->elems[_c_i], &_c_b->elems[_c_i]);
    if (caut_ord_eq != _c_o) {
      return _c_o;
    }
  }

  return CAUT_ORDER(_c_a->_length, _c_b->_length);
}

enum caut_ord compare_assoc(struct assoc const * const _c_a, struct assoc const * const _c_b) {
  enum caut_ord _c_o;

  if (caut_ord_eq != (_c_o = compare_key(&_c_a->k, &_c_b->k))) { return _c_o; }
  if (caut_ord_eq != (_c_o = compare_value(&_c_a->v, &_c_b->v))) { return _c_o; }

  return caut_ord_eq;

}

enum caut_ord compare_arr_key(struct arr_key const * const _c_a, struct arr_key const * const _c_b) {
  for (size_t _c_i = 0; _c_i < _c_a->_length && _c_i < _c_b->_length ; _c_i++) {
    const enum caut_ord _c_o = compare_key(&_c_a->elems[_c_i], &_c_b->elems[_c_i]);
    if (caut_ord_eq != _c_o) {
      return _c_o;
    }
  }

  return CAUT_ORDER(_c_a->_length, _c_b->_length);
}

enum caut_ord compare_arr_assoc(struct arr_assoc const * const _c_a, struct arr_assoc const * const _c_b) {
  for (size_t _c_i = 0; _c_i < _c_a->_length && _c_i < _c_b->_length ; _c_i++) {
    const enum caut_ord _c_o = compare_assoc(&_c_a->elems[_c_i], &_c_b->elems[_c_i]);
    if (caut_ord_eq != _c_o) {
      return _c_o;
    }
  }

  return CAUT_ORDER(_c_a->_length, _c_b->_length);
}

enum caut_ord compare_cmd_config_get(struct cmd_config_get const * const _c_a, struct cmd_config_get const * const _c_b) {
  enum caut_ord _c_o;

  if (caut_ord_eq != (_c_o = CAUT_ORDER(_c_a->_tag, _c_b->_tag))) {
    return _c_o;
  }

  switch(_c_a->_tag) {
  case cmd_config_get_tag_req:
    _c_o = compare_arr_key(&_c_a->req, &_c_b->req);
    break;
  case cmd_config_get_tag_res:
    _c_o = compare_arr_assoc(&_c_a->res, &_c_b->res);
    break;
  }

  return _c_o;
}

enum caut_ord compare_cmd_config_set(struct cmd_config_set const * const _c_a, struct cmd_config_set const * const _c_b) {
  enum caut_ord _c_o;

  if (caut_ord_eq != (_c_o = CAUT_ORDER(_c_a->_tag, _c_b->_tag))) {
    return _c_o;
  }

  switch(_c_a->_tag) {
  case cmd_config_set_tag_req:
    _c_o = compare_arr_assoc(&_c_a->req, &_c_b->req);
    break;
  case cmd_config_set_tag_res:
    _c_o = compare_u8(&_c_a->res, &_c_b->res);
    break;
  }

  return _c_o;
}

enum caut_ord compare_cmd_config(struct cmd_config const * const _c_a, struct cmd_config const * const _c_b) {
  enum caut_ord _c_o;

  if (caut_ord_eq != (_c_o = CAUT_ORDER(_c_a->_tag, _c_b->_tag))) {
    return _c_o;
  }

  switch(_c_a->_tag) {
  case cmd_config_tag_get:
    _c_o = compare_cmd_config_get(&_c_a->get, &_c_b->get);
    break;
  case cmd_config_tag_set:
    _c_o = compare_cmd_config_set(&_c_a->set, &_c_b->set);
    break;
  case cmd_config_tag_invalidate:
    _c_o = caut_ord_eq; /* No comparison for empty field invalidate */
    break;
  }

  return _c_o;
}


#undef R
#undef I

