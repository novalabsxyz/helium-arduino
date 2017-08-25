#ifndef _CAUTERIZE_C11REF_config_api_
#define _CAUTERIZE_C11REF_config_api_

#include "cauterize.h"

/* library meta information */
#define NAME_config_api "config_api"
#define VERSION_config_api "1.0.0"
#define MIN_SIZE_config_api (1)
#define MAX_SIZE_config_api (553)
#define NUM_TYPES_config_api (9)

/* schema hash */
extern hashtype_t const SCHEMA_HASH_config_api;

/* type indicies */
enum type_index_config_api {
  type_index_config_api_string = 0,
  type_index_config_api_value = 1,
  type_index_config_api_key = 2,
  type_index_config_api_assoc = 3,
  type_index_config_api_arr_key = 4,
  type_index_config_api_arr_assoc = 5,
  type_index_config_api_cmd_config_get = 6,
  type_index_config_api_cmd_config_set = 7,
  type_index_config_api_cmd_config = 8,
};

/* type definitions */
#define VECTOR_MAX_LEN_string (32)
struct string {
  caut_tag8_t _length;
  uint8_t elems[VECTOR_MAX_LEN_string];
};

#define UNION_NUM_FIELDS_value (0x5ull)
struct value {
  enum value_tag {
    value_tag_b = 0,
    value_tag_i = 1,
    value_tag_f = 2,
    value_tag_s = 3,
    value_tag_n = 4,
  } _tag;


  union {
    bool b;
    int32_t i;
    float f;
    struct string s;
    /* no data for field i"n" with index 4 */
  };

};

#define VECTOR_MAX_LEN_key (20)
struct key {
  caut_tag8_t _length;
  uint8_t elems[VECTOR_MAX_LEN_key];
};

struct assoc {
  struct key k;
  struct value v;
};

#define VECTOR_MAX_LEN_arr_key (10)
struct arr_key {
  caut_tag8_t _length;
  struct key elems[VECTOR_MAX_LEN_arr_key];
};

#define VECTOR_MAX_LEN_arr_assoc (10)
struct arr_assoc {
  caut_tag8_t _length;
  struct assoc elems[VECTOR_MAX_LEN_arr_assoc];
};

#define UNION_NUM_FIELDS_cmd_config_get (0x2ull)
struct cmd_config_get {
  enum cmd_config_get_tag {
    cmd_config_get_tag_req = 0,
    cmd_config_get_tag_res = 1,
  } _tag;


  union {
    struct arr_key req;
    struct arr_assoc res;
  };

};

#define UNION_NUM_FIELDS_cmd_config_set (0x2ull)
struct cmd_config_set {
  enum cmd_config_set_tag {
    cmd_config_set_tag_req = 0,
    cmd_config_set_tag_res = 1,
  } _tag;


  union {
    struct arr_assoc req;
    uint8_t res;
  };

};

#define UNION_NUM_FIELDS_cmd_config (0x3ull)
struct cmd_config {
  enum cmd_config_tag {
    cmd_config_tag_get = 0,
    cmd_config_tag_set = 1,
    cmd_config_tag_invalidate = 2,
  } _tag;


  union {
    struct cmd_config_get get;
    struct cmd_config_set set;
    /* no data for field i"invalidate" with index 2 */
  };

};


/* function prototypes */
enum caut_status encode_string(struct caut_encode_iter * const _c_iter, struct string const * const _c_obj);
enum caut_status decode_string(struct caut_decode_iter * const _c_iter, struct string * const _c_obj);
void init_string(struct string * _c_obj);
enum caut_ord compare_string(struct string const * const _c_a, struct string const * const _c_b);

enum caut_status encode_value(struct caut_encode_iter * const _c_iter, struct value const * const _c_obj);
enum caut_status decode_value(struct caut_decode_iter * const _c_iter, struct value * const _c_obj);
void init_value(struct value * _c_obj);
enum caut_ord compare_value(struct value const * const _c_a, struct value const * const _c_b);

enum caut_status encode_key(struct caut_encode_iter * const _c_iter, struct key const * const _c_obj);
enum caut_status decode_key(struct caut_decode_iter * const _c_iter, struct key * const _c_obj);
void init_key(struct key * _c_obj);
enum caut_ord compare_key(struct key const * const _c_a, struct key const * const _c_b);

enum caut_status encode_assoc(struct caut_encode_iter * const _c_iter, struct assoc const * const _c_obj);
enum caut_status decode_assoc(struct caut_decode_iter * const _c_iter, struct assoc * const _c_obj);
void init_assoc(struct assoc * _c_obj);
enum caut_ord compare_assoc(struct assoc const * const _c_a, struct assoc const * const _c_b);

enum caut_status encode_arr_key(struct caut_encode_iter * const _c_iter, struct arr_key const * const _c_obj);
enum caut_status decode_arr_key(struct caut_decode_iter * const _c_iter, struct arr_key * const _c_obj);
void init_arr_key(struct arr_key * _c_obj);
enum caut_ord compare_arr_key(struct arr_key const * const _c_a, struct arr_key const * const _c_b);

enum caut_status encode_arr_assoc(struct caut_encode_iter * const _c_iter, struct arr_assoc const * const _c_obj);
enum caut_status decode_arr_assoc(struct caut_decode_iter * const _c_iter, struct arr_assoc * const _c_obj);
void init_arr_assoc(struct arr_assoc * _c_obj);
enum caut_ord compare_arr_assoc(struct arr_assoc const * const _c_a, struct arr_assoc const * const _c_b);

enum caut_status encode_cmd_config_get(struct caut_encode_iter * const _c_iter, struct cmd_config_get const * const _c_obj);
enum caut_status decode_cmd_config_get(struct caut_decode_iter * const _c_iter, struct cmd_config_get * const _c_obj);
void init_cmd_config_get(struct cmd_config_get * _c_obj);
enum caut_ord compare_cmd_config_get(struct cmd_config_get const * const _c_a, struct cmd_config_get const * const _c_b);

enum caut_status encode_cmd_config_set(struct caut_encode_iter * const _c_iter, struct cmd_config_set const * const _c_obj);
enum caut_status decode_cmd_config_set(struct caut_decode_iter * const _c_iter, struct cmd_config_set * const _c_obj);
void init_cmd_config_set(struct cmd_config_set * _c_obj);
enum caut_ord compare_cmd_config_set(struct cmd_config_set const * const _c_a, struct cmd_config_set const * const _c_b);

enum caut_status encode_cmd_config(struct caut_encode_iter * const _c_iter, struct cmd_config const * const _c_obj);
enum caut_status decode_cmd_config(struct caut_decode_iter * const _c_iter, struct cmd_config * const _c_obj);
void init_cmd_config(struct cmd_config * _c_obj);
enum caut_ord compare_cmd_config(struct cmd_config const * const _c_a, struct cmd_config const * const _c_b);


#endif /* _CAUTERIZE_C11REF_config_api_ */
