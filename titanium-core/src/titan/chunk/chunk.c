
#include "chunk.h"

static ATTR_FORCE_INLINE chunk_size_t grow_capacity(chunk_size_t current)
{
    ASSERT(current < (UMAX_OF(chunk_size_t) / 2), "Cannot grow capacity, would pass maximum allowed size!");

    return current < 8 ? 8 : current * 2;
}

void chunk_init(Chunk *chunk)
{
    chunk->capacity = 0;
    chunk->size = 0;
    chunk->instructions = NULL;
    chunk->dbg_lines = NULL;
    datapool_init(&chunk->datapool);
}

void chunk_free(Chunk *chunk)
{
    reallocate(chunk->instructions, 0);
    reallocate(chunk->dbg_lines, 0);
    chunk->instructions = NULL;
    chunk->dbg_lines = NULL;
    chunk->size = 0;
    chunk->capacity = 0;
    datapool_free(&chunk->datapool);
}

chunk_size_t chunk_write_code(Chunk *chunk, uint8_t value, uint16_t loc)
{
    if (chunk->capacity == chunk->size)
    {
        if (chunk->capacity == UMAX_OF(chunk_size_t))
            // OOPS! We have reached max possible instruction capacity for this chunk
            exit(TI_ERR_CHUNK_MAX_CAPACITY);

        uint16_t old_capacity = chunk->capacity;
        chunk->capacity = grow_capacity(old_capacity);

        chunk->instructions = reallocate(
            chunk->instructions,
            chunk->capacity * sizeof(uint8_t));

        chunk->dbg_lines = reallocate(
            chunk->dbg_lines,
            chunk->capacity * sizeof(uint16_t));
    }

    chunk->instructions[chunk->size] = value;
    chunk->dbg_lines[chunk->size] = loc;
    return chunk->size++;
}

pool_size_t chunk_write_pool(Chunk *chunk, uint8_t value)
{
    datapool_write_pool(&chunk->datapool, value);
    return chunk->datapool.size - 1;
}

uint8_t chunk_read_code(const Chunk *chunk, chunk_size_t idx)
{
    return chunk->instructions[idx];
}

uint8_t chunk_read_pool(const Chunk *chunk, pool_size_t idx)
{
    return chunk->datapool.values[idx];
}

uint16_t chunk_get_loc(const Chunk *chunk, chunk_size_t index)
{
    return chunk->dbg_lines[index];
}
