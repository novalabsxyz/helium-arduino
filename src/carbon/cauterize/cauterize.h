#ifndef _CAUTERIZE_H_
#define _CAUTERIZE_H_

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum caut_status {
  caut_status_ok,
  caut_status_invalid_tag,
  caut_status_invalid_flags,
  caut_status_invalid_length,
  caut_status_would_overflow,
  caut_status_would_underflow,
  caut_status_enumeration_out_of_range,
  caut_status_range_out_of_bounds,
};

enum caut_ord {
  caut_ord_lt = -1,
  caut_ord_eq = 0,
  caut_ord_gt = 1,
};

/* Array type for a SHA1 hash. */
typedef uint8_t hashtype_t[20];

/* An alias for bool. Must be 1 byte. */
typedef bool caut_bool;

/* Aliases for catuerize tags. */
typedef uint8_t  caut_tag8_t;
typedef uint16_t caut_tag16_t;
typedef uint32_t caut_tag32_t;
typedef uint64_t caut_tag64_t;

/* Returns the status if it's not `caut_status_ok`. */
#define STATUS_CHECK(S) \
  do { \
    enum caut_status __check_stat = (S); \
    if (caut_status_ok != __check_stat) { \
      return __check_stat; \
    } \
  } while (0)

#define ARR_LEN(A) \
  (sizeof(A) / sizeof(A[0]))

#define CAUT_ASSERT(EXP) \
  do { \
    if (!(EXP)) { \
      printf("ASSERT FAIL on %s:%u\n\t" #EXP, __FILE__, __LINE__); \
      exit(__LINE__); \
    } \
  } while(0)

#define CAUT_ORDER(A,B) \
  ((A) < (B) ? caut_ord_lt : ((A) > (B) ? caut_ord_gt : caut_ord_eq))

#define CHECK_BIT(F,IX) \
  (((F) >> (IX)) & 1)

struct caut_encode_iter;
struct caut_decode_iter;

/* Generalized encode and decode function pointers */
typedef enum caut_status (gen_encode)(struct caut_encode_iter * const iter, void const * const obj);
typedef enum caut_status (gen_decode)(struct caut_decode_iter * const iter, void const * const obj);

struct caut_type_descriptor {
  char * name;
  hashtype_t hash;
  gen_encode * encode;
  gen_decode * decode;
  size_t min_size;
  size_t max_size;
};

struct caut_encode_iter {
  uint8_t * buffer;
  size_t length;
  size_t position;
};

struct caut_decode_iter {
  uint8_t * buffer;
  size_t length;
  size_t position;
};

void caut_encode_iter_init(struct caut_encode_iter * const iter, void * const buffer, size_t const length);
void * caut_encode_iter_buffer(const struct caut_encode_iter * const iter);
size_t caut_encode_iter_remaining(const struct caut_encode_iter * const iter);
size_t caut_encode_iter_used(const struct caut_encode_iter * const iter);


void caut_decode_iter_init(struct caut_decode_iter * const iter, void * const buffer, size_t const length);
void * caut_decode_iter_buffer(const struct caut_decode_iter * const iter);
size_t caut_decode_iter_remaining(const struct caut_decode_iter * const iter);



/* Encoders for all fundamental C types. */
enum caut_status encode_s8(struct caut_encode_iter * const iter, int8_t const * const obj);
enum caut_status encode_s16(struct caut_encode_iter * const iter, int16_t const * const obj);
enum caut_status encode_s32(struct caut_encode_iter * const iter, int32_t const * const obj);
enum caut_status encode_s64(struct caut_encode_iter * const iter, int64_t const * const obj);

enum caut_status encode_u8(struct caut_encode_iter * const iter, uint8_t const * const obj);
enum caut_status encode_u16(struct caut_encode_iter * const iter, uint16_t const * const obj);
enum caut_status encode_u32(struct caut_encode_iter * const iter, uint32_t const * const obj);
enum caut_status encode_u64(struct caut_encode_iter * const iter, uint64_t const * const obj);

enum caut_status encode_f32(struct caut_encode_iter * const iter, float const * const obj);
enum caut_status encode_f64(struct caut_encode_iter * const iter, double const * const obj);

enum caut_status encode_bool(struct caut_encode_iter * const iter, bool const * const obj);

enum caut_status __caut_encode_null_bytes(struct caut_encode_iter * const iter, size_t count);

enum caut_status __caut_encode_reserve(struct caut_encode_iter * const iter, size_t reserve_bytes, void ** ptr);

enum caut_status __caut_encode_raw_bytes(struct caut_encode_iter * const iter, uint8_t const * const bytes, size_t len);

enum caut_status encode_tag8(struct caut_encode_iter * const iter, uint8_t const * const obj);
enum caut_status encode_tag16(struct caut_encode_iter * const iter, uint16_t const * const obj);
enum caut_status encode_tag32(struct caut_encode_iter * const iter, uint32_t const * const obj);
enum caut_status encode_tag64(struct caut_encode_iter * const iter, uint64_t const * const obj);

/* Decoders for all fundamental C types. */
enum caut_status decode_s8(struct caut_decode_iter * const iter, int8_t * const obj);
enum caut_status decode_s16(struct caut_decode_iter * const iter, int16_t * const obj);
enum caut_status decode_s32(struct caut_decode_iter * const iter, int32_t * const obj);
enum caut_status decode_s64(struct caut_decode_iter * const iter, int64_t * const obj);

enum caut_status decode_u8(struct caut_decode_iter * const iter, uint8_t * const obj);
enum caut_status decode_u16(struct caut_decode_iter * const iter, uint16_t * const obj);
enum caut_status decode_u32(struct caut_decode_iter * const iter, uint32_t * const obj);
enum caut_status decode_u64(struct caut_decode_iter * const iter, uint64_t * const obj);

enum caut_status decode_f32(struct caut_decode_iter * const iter, float * const obj);
enum caut_status decode_f64(struct caut_decode_iter * const iter, double * const obj);

enum caut_status decode_bool(struct caut_decode_iter * const iter, bool * const obj);

enum caut_status __caut_decode_and_ignore_bytes(struct caut_decode_iter * const iter, size_t count);

enum caut_status __caut_decode_raw_bytes(struct caut_decode_iter * const iter, uint8_t * const bytes, size_t len);
enum caut_status decode_tag8(struct caut_decode_iter * const iter, uint8_t * const obj);
enum caut_status decode_tag16(struct caut_decode_iter * const iter, uint16_t * const obj);
enum caut_status decode_tag32(struct caut_decode_iter * const iter, uint32_t * const obj);
enum caut_status decode_tag64(struct caut_decode_iter * const iter, uint64_t * const obj);

/* Comparators for all fundamental C types. */
enum caut_ord compare_u8(uint8_t const * a, uint8_t const * b);
enum caut_ord compare_u16(uint16_t const * a, uint16_t const * b);
enum caut_ord compare_u32(uint32_t const * a, uint32_t const * b);
enum caut_ord compare_u64(uint64_t const * a, uint64_t const * b);
enum caut_ord compare_s8(int8_t const * a, int8_t const * b);
enum caut_ord compare_s16(int16_t const * a, int16_t const * b);
enum caut_ord compare_s32(int32_t const * a, int32_t const * b);
enum caut_ord compare_s64(int64_t const * a, int64_t const * b);
enum caut_ord compare_f32(float const * a, float const * b);
enum caut_ord compare_f64(double const * a, double const * b);
enum caut_ord compare_bool(bool const * a, bool const * b);

/* Initializers for all fundamental C types. */
void init_u8(uint8_t * o);
void init_u16(uint16_t * o);
void init_u32(uint32_t * o);
void init_u64(uint64_t * o);
void init_s8(int8_t * o);
void init_s16(int16_t * o);
void init_s32(int32_t * o);
void init_s64(int64_t * o);
void init_f32(float * o);
void init_f64(double * o);
void init_bool(bool * o);

#endif /* _CAUTERIZE_H_ */
