#ifndef TITAN_DATAPOOL
#define TITAN_DATAPOOL

#include <common.h>

typedef uint8_t pool_size_t;

typedef struct
{
    pool_size_t capacity;
    pool_size_t size;
    uint8_t *values;
}
DataPool;

void datapool_init(DataPool *pool);
void datapool_free(DataPool *pool);
void datapool_write_pool(DataPool *pool, uint8_t value);

#endif
