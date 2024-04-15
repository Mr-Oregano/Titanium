#ifndef TITANIUM_UTIL
#define TITANIUM_UTIL

#include <common.h>
#include <titan/debug/error_codes.h>
#include <string.h>

#define BYTES_TO_BITS(B) ((B) << 3)
#define SIZE_OF_BITS(T) (BYTES_TO_BITS(sizeof(T)))
#define UMAX_OF(TYPE) ((TYPE) ~(TYPE) 0)

#define BYTE_TO_BINARY_STR_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY_STR(byte)  \
  ((byte) & 0x80 ? '1' : '0'), \
  ((byte) & 0x40 ? '1' : '0'), \
  ((byte) & 0x20 ? '1' : '0'), \
  ((byte) & 0x10 ? '1' : '0'), \
  ((byte) & 0x08 ? '1' : '0'), \
  ((byte) & 0x04 ? '1' : '0'), \
  ((byte) & 0x02 ? '1' : '0'), \
  ((byte) & 0x01 ? '1' : '0') 

#define i2f(in) _Generic((in),        \
                    uint32_t: i2f_32, \
                    uint64_t: i2f_64)(in)

#define f2i(in) _Generic((in),        \
                    float:  f2i_32,   \
                    double: f2i_64)(in)

#define u2s(in) _Generic((in),        \
                    uint32_t: u2s_32, \
                    uint64_t: u2s_64)(in)

#define s2u(in) _Generic((in),        \
                    int32_t: s2u_32,  \
                    int64_t: s2u_64)(in)

static inline void *reallocate(void *ptr, size_t new_size)
{
    if (new_size == 0)
    {
        free(ptr);
        return NULL;
    }

    void *new_ptr = realloc(ptr, new_size);

    if (new_ptr == NULL)
        exit(TI_ERR_OUT_OF_MEMORY);

    return new_ptr;
}

// Floating-point to integer conversions

static ATTR_FORCE_INLINE float i2f_32(uint32_t in)
{
    float out;
    memcpy(&out, &in, sizeof(out));
    return out;
}

static ATTR_FORCE_INLINE double i2f_64(uint64_t in)
{
    double out;
    memcpy(&out, &in, sizeof(out));
    return out;
}

static ATTR_FORCE_INLINE uint32_t f2i_32(float in)
{
    uint32_t out;
    memcpy(&out, &in, sizeof(out));
    return out;
}

static ATTR_FORCE_INLINE uint64_t f2i_64(double in)
{
    uint64_t out;
    memcpy(&out, &in, sizeof(out));
    return out;
}

// Unsigned integer and floating-point demotions

static ATTR_FORCE_INLINE uint32_t l2i(uint64_t in)
{
    return in & UMAX_OF(uint32_t);
}

static ATTR_FORCE_INLINE uint16_t i2s(uint32_t in)
{
    return in & UMAX_OF(uint16_t);
}

static ATTR_FORCE_INLINE uint8_t i2b(uint32_t in)
{
    return in & UMAX_OF(uint8_t);
}

static ATTR_FORCE_INLINE float d2f(double in)
{
    return (float)in;
}

// Unsigned to signed integer conversions

static ATTR_FORCE_INLINE int32_t u2s_32(uint32_t in)
{
    return (int32_t)in;
}

static ATTR_FORCE_INLINE int64_t u2s_64(uint64_t in)
{
    return (int64_t)in;
}

static ATTR_FORCE_INLINE uint32_t s2u_32(int32_t in)
{
    return (uint32_t)in;
}

static ATTR_FORCE_INLINE uint64_t s2u_64(int64_t in)
{
    return (uint64_t)in;
}

#endif
