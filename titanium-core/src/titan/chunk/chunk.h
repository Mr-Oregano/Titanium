#ifndef TITAN_CHUNK
#define TITAN_CHUNK

#include <string.h>

#include <common.h>
#include <titan/datapool/datapool.h>
#include <titan/opcodes.h>
#include <util/util.h>

typedef uint16_t chunk_size_t;

typedef struct
{
    chunk_size_t capacity;
    chunk_size_t size;
    DataPool datapool;
    uint8_t *instructions;

    // 1-1 correspondence with instructions for now
    // TODO: use run-length encoding: https://en.wikipedia.org/wiki/Run-length_encoding
    uint16_t *dbg_lines;
}
Chunk;

void chunk_init(Chunk *chunk);
void chunk_free(Chunk *chunk);

chunk_size_t chunk_write_code(Chunk *chunk, uint8_t value, uint16_t loc);
pool_size_t  chunk_write_pool(Chunk *chunk, uint8_t value);

uint8_t chunk_read_code(const Chunk *chunk, chunk_size_t idx);
uint8_t chunk_read_pool(const Chunk *chunk, pool_size_t idx);

uint16_t chunk_get_loc(const Chunk *chunk, chunk_size_t index);

// Utility functions for writing multiple bytes to chunk/data pool

static ATTR_FORCE_INLINE chunk_size_t chunk_write_code_t(Chunk *chunk, const uint64_t value, uint16_t loc, size_t size)
{
    chunk_size_t idx = chunk_write_code(chunk, value & UMAX_OF(uint8_t), loc);

    for (size_t i = 1; i < size; ++i)
    {
        uint8_t v = (value >> BYTES_TO_BITS(i)) & UMAX_OF(uint8_t);
        chunk_write_code(chunk, v, loc);
    }

    return idx;
}

static ATTR_FORCE_INLINE pool_size_t chunk_write_pool_t(Chunk *chunk, const uint64_t value, size_t size)
{
    pool_size_t idx = chunk_write_pool(chunk, value & UMAX_OF(uint8_t));

    for (size_t i = 1; i < size; ++i)
    {
        uint8_t v = (value >> BYTES_TO_BITS(i)) & UMAX_OF(uint8_t);
        chunk_write_pool(chunk, v);
    }

    return idx;
}

static ATTR_FORCE_INLINE uint64_t chunk_read_code_t(const Chunk *chunk, const chunk_size_t idx, size_t size)
{
    ASSERT(size <= UMAX_OF(chunk_size_t), "Size of value too large for chunk!");
    uint64_t result = 0;

    for (size_t i = 0; i < size; ++i)
    {
        chunk_size_t offset = (chunk_size_t)i;

        uint64_t v = chunk_read_code(chunk, idx + offset);
        result |= v << BYTES_TO_BITS(i);
    }

    return result;
}

static ATTR_FORCE_INLINE uint64_t chunk_read_pool_t(const Chunk *chunk, const pool_size_t idx, size_t size)
{
    ASSERT(size <= UMAX_OF(pool_size_t), "Size of value too large for pool!");
    uint64_t result = 0;

    for (size_t i = 0; i < size; ++i)
    {
        pool_size_t offset = (pool_size_t)i;

        uint64_t v = chunk_read_pool(chunk, idx + offset);
        result |= v << BYTES_TO_BITS(i);
    }

    return result;
}

static ATTR_FORCE_INLINE uint64_t chunk_next_inst_t(const uint8_t **const ip, size_t size)
{
    ASSERT(ip != NULL, "Cannot read instruction from NULL instruction pointer!");
    uint64_t result = 0;

    for (size_t i = 0; i < size; ++i)
    {
        uint64_t v = **ip;
        ++(*ip);
        result |= v << BYTES_TO_BITS(i);
    }

    return result;
}

#define CHUNK_WRITE_CODE_FUNC_T(NAME, TYPE) \
    static ATTR_FORCE_INLINE chunk_size_t NAME(Chunk *chunk, TYPE value, uint16_t loc) { return chunk_write_code_t(chunk, (uint64_t) value, loc, sizeof(TYPE)); }

#define CHUNK_WRITE_POOL_FUNC_T(NAME, TYPE) \
    static ATTR_FORCE_INLINE pool_size_t NAME(Chunk *chunk, TYPE value) { return chunk_write_pool_t(chunk, (uint64_t) value, sizeof(TYPE)); }

#define CHUNK_READ_CODE_FUNC_T(NAME, TYPE) \
    static ATTR_FORCE_INLINE TYPE NAME(const Chunk *chunk, chunk_size_t idx) { return chunk_read_code_t(chunk, idx, sizeof(TYPE)) & UMAX_OF(TYPE); }

#define CHUNK_READ_POOL_FUNC_T(NAME, TYPE) \
    static ATTR_FORCE_INLINE TYPE NAME(const Chunk *chunk, pool_size_t idx) { return chunk_read_pool_t(chunk, idx, sizeof(TYPE)) & UMAX_OF(TYPE); }

#define CHUNK_NEXT_INST_FUNC_T(NAME, TYPE) \
    static ATTR_FORCE_INLINE TYPE NAME(const uint8_t **const ip) { return chunk_next_inst_t(ip, sizeof(TYPE)) & UMAX_OF(TYPE); }

CHUNK_WRITE_CODE_FUNC_T(chunk_write_uint8_code, uint8_t)
CHUNK_WRITE_CODE_FUNC_T(chunk_write_uint16_code, uint16_t)
CHUNK_WRITE_CODE_FUNC_T(chunk_write_uint32_code, uint32_t)
CHUNK_WRITE_CODE_FUNC_T(chunk_write_uint64_code, uint64_t)
CHUNK_WRITE_CODE_FUNC_T(chunk_write_pool_idx, pool_size_t)
CHUNK_WRITE_CODE_FUNC_T(chunk_write_chunk_idx, chunk_size_t)
CHUNK_WRITE_CODE_FUNC_T(chunk_write_opcode, TiOpCode)

CHUNK_WRITE_POOL_FUNC_T(chunk_write_uint8_pool, uint8_t)
CHUNK_WRITE_POOL_FUNC_T(chunk_write_uint16_pool, uint16_t)
CHUNK_WRITE_POOL_FUNC_T(chunk_write_uint32_pool, uint32_t)
CHUNK_WRITE_POOL_FUNC_T(chunk_write_uint64_pool, uint64_t)

CHUNK_READ_CODE_FUNC_T(chunk_read_uint8_code, uint8_t)
CHUNK_READ_CODE_FUNC_T(chunk_read_uint16_code, uint16_t)
CHUNK_READ_CODE_FUNC_T(chunk_read_uint32_code, uint32_t)
CHUNK_READ_CODE_FUNC_T(chunk_read_uint64_code, uint64_t)
CHUNK_READ_CODE_FUNC_T(chunk_read_pool_idx, pool_size_t)
CHUNK_READ_CODE_FUNC_T(chunk_read_chunk_idx, chunk_size_t)
CHUNK_READ_CODE_FUNC_T(chunk_read_opcode, TiOpCode)

CHUNK_READ_POOL_FUNC_T(chunk_read_uint8_pool, uint8_t)
CHUNK_READ_POOL_FUNC_T(chunk_read_uint16_pool, uint16_t)
CHUNK_READ_POOL_FUNC_T(chunk_read_uint32_pool, uint32_t)
CHUNK_READ_POOL_FUNC_T(chunk_read_uint64_pool, uint64_t)

CHUNK_NEXT_INST_FUNC_T(chunk_next_inst_uint8, uint8_t)
CHUNK_NEXT_INST_FUNC_T(chunk_next_inst_uint16, uint16_t)
CHUNK_NEXT_INST_FUNC_T(chunk_next_inst_uint32, uint32_t)
CHUNK_NEXT_INST_FUNC_T(chunk_next_inst_uint64, uint64_t)
CHUNK_NEXT_INST_FUNC_T(chunk_next_inst_pool_idx, pool_size_t)
CHUNK_NEXT_INST_FUNC_T(chunk_next_inst_chunk_idx, chunk_size_t)
CHUNK_NEXT_INST_FUNC_T(chunk_next_inst_opcode, TiOpCode)

#undef CHUNK_WRITE_CODE_FUNC_T
#undef CHUNK_WRITE_POOL_FUNC_T
#undef CHUNK_READ_CODE_FUNC_T
#undef CHUNK_READ_POOL_FUNC_T
#undef CHUNK_NEXT_INST_FUNC_T

#endif
