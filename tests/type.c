#include <stdio.h>
#include "hemp.h"
#include "hemp/type.h"
#include "tap.h"


struct foo {
    int  n;
    char *ptr;
};

typedef struct foo *foo_ptr;

#define ANSWER "The answer"
void test_type();
hemp_item_t foo_prepare(hemp_item_t foo, HEMP_ARGS);
void        foo_cleanup(hemp_ptr_t foo);


int
main(int argc, char **argv, char **env)
{
    plan_tests(6);
    test_type();
    return exit_status();
}


void test_type() {
    /* create a new Foo type */
    hemp_type_t Foo = hemp_type_init("Foo", sizeof(struct foo));
    ok( Foo, "created %s type", Foo->name );

    /* bind our custom prepare/cleanup methods */
    Foo->prepare = &foo_prepare;
    Foo->cleanup = &foo_cleanup;

    foo_ptr foo = hemp_new(Foo, 42, ANSWER);
    ok( foo, "created foo object" );
    ok( foo->n == 42, "n is 42" );
    ok( hemp_cstr_eq(foo->ptr, ANSWER), "%s is 42", foo->ptr );
    ok( hemp_cstr_eq(hemp_type_name(foo), "Foo"), "foo is a Foo object" );
    ok( hemp_cstr_eq(hemp_type_of(foo)->name, "Foo"), "foo is a Foo object" );

    hemp_old(foo);
    hemp_type_free(Foo);
}


hemp_item_t 
foo_prepare(
    hemp_item_t item,
    HEMP_ARGS
) {
    foo_ptr foo = (foo_ptr) item;
    int n     = HEMP_ARG(int);
    char *ptr = HEMP_ARG(char *);
    hemp_debug("foo_init(%p, %d, %s)\n", foo, n, ptr);
    foo->n = n;
    foo->ptr = strdup(ptr);
    return foo;
}


void 
foo_cleanup(
    hemp_item_t item
) {
    foo_ptr foo = (foo_ptr) item;
    hemp_debug("foo_cleanup(%p) %d: %s\n", foo, foo->n, foo->ptr);
    hemp_mem_free(foo->ptr);
}
