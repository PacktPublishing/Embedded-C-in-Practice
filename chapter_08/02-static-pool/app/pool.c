#include <stdbool.h>
#include <stddef.h>

#include "pool.h"

static uint8_t pool_data[POOL_BLOCK_COUNT][POOL_BLOCK_SIZE];
static bool pool_used[POOL_BLOCK_COUNT];

void *pool_alloc(void)
{
    for (uint32_t i = 0u; i < POOL_BLOCK_COUNT; i++)
    {
        if (!pool_used[i])
        {
            pool_used[i] = true;
            return pool_data[i];
        }
    }

    return NULL;
}

void pool_release(void *block)
{
    for (uint32_t i = 0u; i < POOL_BLOCK_COUNT; i++)
    {
        if (block == pool_data[i])
        {
            pool_used[i] = false;
            return;
        }
    }
}
