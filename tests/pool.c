#include <stdio.h>
#include <assert.h>
#include "hemp.h"

void test_pool();

int
main(int argc, char **argv, char **env)
{
    test_pool();
    return 0;
}

void test_pool() {
    hemp_pool_t pool = hemp_pool_init(4, 2);
    hemp_ptr_t item;
    assert(pool);

    pass("created pool");

    pool->capacity == 2
        ? pass("pool capacity is %d", 2)
        : fail("pool capacity is not %d", 2);

    (item = hemp_pool_take(pool))
        ? pass("took a pool item at %p", item)
        : fail("could not take a pool item");

    (item = hemp_pool_take(pool))
        ? pass("took another pool item at %p", item)
        : fail("could not take another pool item");

    pool->capacity == 2
        ? pass("pool capacity is still %d", 2)
        : fail("pool capacity is no longer %d", 2);

    pool->used == 2
        ? pass("pool has used %d items", 2)
        : fail("pool has not used %d items", 2);

    (item = hemp_pool_take(pool))
        ? pass("took a third pool item at %p", item)
        : fail("could not take a third pool item");

    pool->capacity == 4
        ? pass("pool capacity has grown to %d", 4)
        : fail("pool capacity has not grown to %d", 4);

    hemp_pool_grow(pool);

    pool->capacity == 8
        ? pass("pool capacity is %d", 8)
        : fail("pool capacity is not %d", 8);

    hemp_pool_free(pool);

    pass("released pool");
}

