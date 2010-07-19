#include <stdio.h>
#include "hemp.h"
#include "tap.h"

void test_pool();
void test_pool_type();

int
main(int argc, char **argv, char **env)
{
    plan_tests(10);
//    test_pool();
    test_pool_type();
    return exit_status();
}


void test_pool() {
    hemp_pool_t pool = hemp_pool_init(4, 2);
    hemp_ptr_t item;

    ok( 
        pool, 
        "created pool" 
    );
    ok( 
        pool->capacity == 2, 
        "pool capacity is %d", pool->capacity 
    );
    ok( 
        (item = hemp_pool_take(pool)), 
        "took a pool item at %p", item 
    );
    ok( 
        (item = hemp_pool_take(pool)),
        "took another pool item at %p", item 
    );
    ok( 
        pool->capacity == 2, 
        "pool capacity is still %d", pool->capacity 
    );
    ok( 
        pool->used == 2, 
        "pool has used %d items", pool->used 
    );
    ok( 
        (item = hemp_pool_take(pool)), 
        "took a third item at %p", item 
    );
    ok( 
        pool->capacity == 4, 
        "pool capacity has grown to %d", pool->capacity
    );

    hemp_pool_grow(pool);

    ok( 
        pool->capacity == 8, 
        "pool capacity forced to grow to %d", pool->capacity
    );

    hemp_pool_free(pool);
    pass("released pool");
}



void test_pool_type() {
    hemp_type_t Pool = hemp_pool_type();
    ok( Pool, "got %s type", Pool->name );

    hemp_pool_t pool = hemp_new(Pool, 4, 2);
    hemp_ptr_t item;

    ok( 
        pool, 
        "created pool from type" 
    );
    ok( 
        pool->capacity == 2, 
        "pool capacity is %d", pool->capacity 
    );

    hemp_old(pool);
}
