#ifndef TITANIUM_COMMON
#define TITANIUM_COMMON

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <assert.h>

#define STATIC_ASSERT(cond, msg) _Static_assert(cond, msg)
#define ASSERT(cond, msg) assert(cond && msg)

#ifdef __GNUC__
#define ATTR_PACKED __attribute__((__packed__))
#define ATTR_FORCE_INLINE __attribute__((__always_inline__)) inline
#elif
#define ATTR_PACKED
#define ATTR_FORCE_INLINE inline
#endif

#endif
