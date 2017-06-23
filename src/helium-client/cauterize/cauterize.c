#include "cauterize.h"

#include <string.h>

#define P struct caut_encode_iter
#define U struct caut_decode_iter

static void caut_encode_iter_advance(P * iter, size_t adv);
static void caut_decode_iter_advance(U * iter, size_t adv);

void caut_encode_iter_init(P * const iter, void * const buffer, size_t const length) {
  CAUT_ASSERT(iter);
  CAUT_ASSERT(buffer);

  iter->buffer = buffer;
  iter->length = length;
  iter->position = 0;
}

void * caut_encode_iter_buffer(const P * const iter) {
  CAUT_ASSERT(iter);

  return iter->buffer;
}

size_t caut_encode_iter_remaining(const P * const iter) {
  CAUT_ASSERT(iter);
  CAUT_ASSERT(iter->position <= iter->length);

  return iter->length - iter->position;
}

size_t caut_encode_iter_used(const P * const iter) {
  return iter->position;
}

static void caut_encode_iter_advance(P * iter, size_t adv) {
  CAUT_ASSERT(iter);

  size_t rem = caut_encode_iter_remaining(iter);

  if (rem >= adv) {
    iter->position += adv;
  }
}

void caut_decode_iter_init(U * iter, void * buffer, size_t length) {
  CAUT_ASSERT(iter);
  CAUT_ASSERT(buffer);

  iter->buffer = buffer;
  iter->length = length;
  iter->position = 0;
}

void * caut_decode_iter_buffer(const U * const iter) {
  CAUT_ASSERT(iter);

  return iter->buffer;
}
size_t caut_decode_iter_remaining(const U * const iter) {
  CAUT_ASSERT(iter);
  CAUT_ASSERT(iter->position <= iter->length);

  return iter->length - iter->position;
}

static void caut_decode_iter_advance(U * const iter, size_t adv) {
  CAUT_ASSERT(iter);

  size_t rem = caut_decode_iter_remaining(iter);

  if (rem >= adv) {
    iter->position += adv;
  }
}

#define ITER_FOCUS_PTR(ITER) (&((ITER)->buffer[(ITER)->position]))

#define MEMMOVE_PACK(ITER, OBJ_PTR) \
  do { \
    if (caut_encode_iter_remaining(ITER) >= sizeof(*OBJ_PTR)) { \
      memmove(ITER_FOCUS_PTR(ITER), OBJ_PTR, sizeof(*OBJ_PTR)); \
      caut_encode_iter_advance(ITER, sizeof(*OBJ_PTR)); \
    } else { \
      return caut_status_would_overflow; \
    } \
    \
    return caut_status_ok; \
  } while (0)

#define MEMMOVE_UNPACK(ITER, OBJ_PTR) \
  do { \
    if (caut_decode_iter_remaining(ITER) >= sizeof(*OBJ_PTR)) { \
      memmove(OBJ_PTR, ITER_FOCUS_PTR(ITER), sizeof(*OBJ_PTR)); \
      caut_decode_iter_advance(ITER, sizeof(*OBJ_PTR)); \
    } else { \
      return caut_status_would_underflow; \
    } \
    \
    return caut_status_ok; \
  } while (0)

#define GENERIC_PACK(CAUT_TYPE, C_TYPE) \
enum caut_status encode_##CAUT_TYPE(P * const iter, C_TYPE const * const obj) { \
  CAUT_ASSERT(iter); \
  CAUT_ASSERT(obj); \
  MEMMOVE_PACK(iter, obj); \
}

#define GENERIC_UNPACK(CAUT_TYPE, C_TYPE) \
enum caut_status decode_##CAUT_TYPE(U * const iter, C_TYPE * const obj) { \
  CAUT_ASSERT(iter); \
  CAUT_ASSERT(obj); \
  MEMMOVE_UNPACK(iter, obj); \
}

GENERIC_PACK(s8,  int8_t)
GENERIC_PACK(s16, int16_t)
GENERIC_PACK(s32, int32_t)
GENERIC_PACK(s64, int64_t)

GENERIC_PACK(u8,  uint8_t)
GENERIC_PACK(u16, uint16_t)
GENERIC_PACK(u32, uint32_t)
GENERIC_PACK(u64, uint64_t)

GENERIC_PACK(f32, float)
GENERIC_PACK(f64, double)

enum caut_status encode_bool(P * const iter, bool const * const obj) {
  CAUT_ASSERT(iter);
  CAUT_ASSERT(obj);
  MEMMOVE_PACK(iter, (uint8_t*)obj);
}


GENERIC_UNPACK(s8,  int8_t)
GENERIC_UNPACK(s16, int16_t)
GENERIC_UNPACK(s32, int32_t)
GENERIC_UNPACK(s64, int64_t)

GENERIC_UNPACK(u8,  uint8_t)
GENERIC_UNPACK(u16, uint16_t)
GENERIC_UNPACK(u32, uint32_t)
GENERIC_UNPACK(u64, uint64_t)

GENERIC_UNPACK(f32, float)
GENERIC_UNPACK(f64, double)

enum caut_status decode_bool(U * const iter, bool * const obj) {
  CAUT_ASSERT(iter);
  CAUT_ASSERT(obj);
  MEMMOVE_UNPACK(iter, (uint8_t*)obj);
}


enum caut_status __caut_encode_null_bytes(struct caut_encode_iter * const iter, size_t count) {
  if (caut_encode_iter_remaining(iter) >= count) {
    memset(ITER_FOCUS_PTR(iter), 0, count);
    caut_encode_iter_advance(iter, count);
  } else {
    return caut_status_would_overflow;
  }

  return caut_status_ok;
}

enum caut_status __caut_encode_reserve(P * const iter, size_t reserve_bytes, void ** ptr) {
  if (reserve_bytes < caut_encode_iter_remaining(iter)) {
    *ptr = ITER_FOCUS_PTR(iter);
    caut_encode_iter_advance(iter, reserve_bytes);
    return caut_status_ok;
  } else {
    return caut_status_would_overflow;
  }
}

enum caut_status __caut_encode_raw_bytes(struct caut_encode_iter * const iter, uint8_t const * const bytes, size_t len) {
  if (caut_encode_iter_remaining(iter) >= len) {
    memmove(ITER_FOCUS_PTR(iter), bytes, len);
    caut_encode_iter_advance(iter, len);
  } else {
    return caut_status_would_overflow;
  }

  return caut_status_ok;
}

enum caut_status __caut_decode_and_ignore_bytes(struct caut_decode_iter * const iter, size_t count) {
  if (caut_decode_iter_remaining(iter) >= count) {
    caut_decode_iter_advance(iter, count);
  } else {
    return caut_status_would_underflow;
  }

  return caut_status_ok;
}

enum caut_status __caut_decode_raw_bytes(struct caut_decode_iter * const iter, uint8_t * const bytes, size_t len) {
  if (caut_decode_iter_remaining(iter) >= len) {
    memmove(bytes, ITER_FOCUS_PTR(iter), len);
    caut_decode_iter_advance(iter, len);
  } else {
    return caut_status_would_underflow;
  }

  return caut_status_ok;
}

enum caut_status encode_tag8(struct caut_encode_iter * const iter, uint8_t const * const obj) {
  return encode_u8(iter, obj);
}

enum caut_status encode_tag16(struct caut_encode_iter * const iter, uint16_t const * const obj) {
  return encode_u16(iter, obj);
}

enum caut_status encode_tag32(struct caut_encode_iter * const iter, uint32_t const * const obj) {
  return encode_u32(iter, obj);
}

enum caut_status encode_tag64(struct caut_encode_iter * const iter, uint64_t const * const obj) {
  return encode_u64(iter, obj);
}

enum caut_status decode_tag8(struct caut_decode_iter * const iter, uint8_t * const obj) {
  return decode_u8(iter, obj);
}

enum caut_status decode_tag16(struct caut_decode_iter * const iter, uint16_t * const obj) {
  return decode_u16(iter, obj);
}

enum caut_status decode_tag32(struct caut_decode_iter * const iter, uint32_t * const obj) {
  return decode_u32(iter, obj);
}

enum caut_status decode_tag64(struct caut_decode_iter * const iter, uint64_t * const obj) {
  return decode_u64(iter, obj);
}


enum caut_ord compare_u8(uint8_t const * a, uint8_t const * b) { return CAUT_ORDER(*a, *b); }
enum caut_ord compare_u16(uint16_t const * a, uint16_t const * b) { return CAUT_ORDER(*a, *b); }
enum caut_ord compare_u32(uint32_t const * a, uint32_t const * b) { return CAUT_ORDER(*a, *b); }
enum caut_ord compare_u64(uint64_t const * a, uint64_t const * b) { return CAUT_ORDER(*a, *b); }
enum caut_ord compare_s8(int8_t const * a, int8_t const * b) { return CAUT_ORDER(*a, *b); }
enum caut_ord compare_s16(int16_t const * a, int16_t const * b) { return CAUT_ORDER(*a, *b); }
enum caut_ord compare_s32(int32_t const * a, int32_t const * b) { return CAUT_ORDER(*a, *b); }
enum caut_ord compare_s64(int64_t const * a, int64_t const * b) { return CAUT_ORDER(*a, *b); }
enum caut_ord compare_f32(float const * a, float const * b) { return CAUT_ORDER(*a, *b); }
enum caut_ord compare_f64(double const * a, double const * b) { return CAUT_ORDER(*a, *b); }
enum caut_ord compare_bool(bool const * a, bool const * b) { return CAUT_ORDER(*a, *b); }

void init_u8(uint8_t * o) { *o = 0; }
void init_u16(uint16_t * o) { *o = 0; }
void init_u32(uint32_t * o) { *o = 0; }
void init_u64(uint64_t * o) { *o = 0; }
void init_s8(int8_t * o) { *o = 0; }
void init_s16(int16_t * o) { *o = 0; }
void init_s32(int32_t * o) { *o = 0; }
void init_s64(int64_t * o) { *o = 0; }
void init_f32(float * o) { *o = 0; }
void init_f64(double * o) { *o = 0; }
void init_bool(bool * o) { *o = false; }
