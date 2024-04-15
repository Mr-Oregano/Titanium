
#include "datapool.h"

#include <util/util.h>

static ATTR_FORCE_INLINE pool_size_t grow_capacity(pool_size_t current)
{
    ASSERT(current < (UMAX_OF(pool_size_t) / 2), "Cannot grow capacity, would pass maximum allowed size!");

    return current < 8 ? 8 : current * 2;
}

void datapool_init(DataPool *pool)
{
    pool->capacity = 0;
    pool->size = 0;
    pool->values = NULL;
}

void datapool_free(DataPool *pool)
{
    reallocate(pool->values, 0);
    pool->values = NULL;
    pool->size = 0;
    pool->capacity = 0;
}

void datapool_write_pool(DataPool *pool, uint8_t value)
{
    if (pool->capacity == pool->size)
    {
        if (pool->capacity == UMAX_OF(pool_size_t))
            // OOPS! We have reached max possible value capacity for this pool
            exit(TI_ERR_DATAPOOL_MAX_CAPACITY);

        pool_size_t old_capacity = pool->capacity;
        pool->capacity = grow_capacity(old_capacity);

        pool->values = reallocate(
            pool->values,
            pool->capacity * sizeof(uint8_t));
    }

    pool->values[pool->size] = value;
    ++pool->size;
}
