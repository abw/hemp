#include <hemp/test.h>

void test_pool();
void test_pool_type();

int main(
    int argc, char **argv, char **env
) {
    plan(10);
    test_pool();
    return done();
}


void test_pool() {
    hemp_pool pool = hemp_pool_new(4, 2, NULL);
    hemp_memory item;

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

