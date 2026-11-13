#ifndef APP_POOL_H
#define APP_POOL_H

#include <stdint.h>

#define POOL_BLOCK_SIZE  64u
#define POOL_BLOCK_COUNT 8u

void *pool_alloc(void);
void pool_release(void *block);

#endif // APP_POOL_H
