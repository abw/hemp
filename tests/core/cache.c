#include <hemp/test.h>

hemp_char buffer[1024];

void test_cache();
void test_cache_lru();

int main(
    int argc, char **argv, char **env
) {
    plan(14);
    test_cache();
    test_cache_lru();
    return done();
}


void test_cache() {
    hemp_hemp hemp = hemp_new();
    ok( hemp, "created hemp object" );

    hemp_cache cache = hemp_cache_new(hemp);
    ok( cache, "created hemp cache" );

    hemp_cache_free(cache);
    hemp_free(hemp);
}


hemp_string lru_order(
    hemp_cache_lru lru
) {
    hemp_cache_lru_slot fresh = lru->fresh;
    hemp_string string = (hemp_string) buffer;
    *string = HEMP_NUL;

    while (fresh) {
        if (*string)
            strcat(string, ", ");
        strcat(string, fresh->key);
        fresh = fresh->staler;
    }
    return string;
}


void check_lru_order(
    hemp_cache  cache,
    hemp_string expect
) {
    hemp_string order = lru_order((hemp_cache_lru) cache->instance);
    is( order, expect, expect );
}


void test_cache_lru() {
    hemp_hemp hemp = hemp_new();
    ok( hemp, "created hemp object" );

    hemp_cache cache = hemp_cache_lru_new(hemp, 4);
    ok( cache, "created LRU cache" );
    
    cache->store(cache, "foo", hemp_int_val(42));
    cache->store(cache, "bar", hemp_int_val(43));
    check_lru_order(cache, "bar, foo");

    cache->fetch(cache, "foo");
    check_lru_order(cache, "foo, bar");

    cache->store(cache, "baz", hemp_int_val(56));
    cache->store(cache, "bam", hemp_int_val(57));
    check_lru_order(cache, "bam, baz, foo, bar");

    cache->store(cache, "wam", hemp_int_val(58));
    check_lru_order(cache, "wam, bam, baz, foo");

    cache->fetch(cache, "foo");
    check_lru_order(cache, "foo, wam, bam, baz");

    cache->fetch(cache, "bam");
    check_lru_order(cache, "bam, foo, wam, baz");

    cache->fetch(cache, "baz");
    check_lru_order(cache, "baz, bam, foo, wam");

    cache->fetch(cache, "bam");
    check_lru_order(cache, "bam, baz, foo, wam");

    cache->fetch(cache, "bam");
    check_lru_order(cache, "bam, baz, foo, wam");

    cache->store(cache, "wim", hemp_int_val(55));
    check_lru_order(cache, "wim, bam, baz, foo");

    hemp_cache_free(cache);
    hemp_free(hemp);
}
