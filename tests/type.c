#include <stdio.h>
#include "hemp.h"
#include "hemp/type.h"
#include "tap.h"


struct foo {
    int n;
    char *ptr;
};

typedef struct foo *foo_ptr;

#define ANSWER "The answer"
void test_type();
hemp_ptr_t foo_init(hemp_ptr_t foo, va_list *args);
void    foo_wipe(hemp_ptr_t foo);


static struct hemp_type
    _hemp_foo_ = { 
        "HempFoo",
        NULL,                       /* base */
        sizeof(struct foo),         /* size */
        &foo_init,                  /* init */
        &foo_wipe                   /* wipe */
    };


#define HEMP_TYPE_BASE      \
    hemp_name_t  name;      \
    hemp_type_t  base;      \
    hemp_size_t  size;      \
    hemp_init_fn init;      \
    hemp_wipe_fn wipe;

hemp_type_t HempFoo = &_hemp_foo_;


int
main(int argc, char **argv, char **env)
{
    plan_tests(5);
    test_type();
    return exit_status();
}


void test_type() {
    foo_ptr foo = hemp_new(HempFoo, 42, ANSWER);
    ok( foo, "created foo object" );
    ok( foo->n == 42, "n is 42" );
    ok( hemp_cstr_eq(foo->ptr, ANSWER), "%s is 42", foo->ptr );
    ok( hemp_cstr_eq(hemp_type_name(foo), "HempFoo"), "foo is a HempFoo object" );
    ok( hemp_cstr_eq(hemp_type_of(foo)->name, "HempFoo"), "foo is a HempFoo object" );
    hemp_old(foo);
}


hemp_ptr_t foo_init(
    hemp_ptr_t p, 
    HEMP_ARGS
) {
    foo_ptr foo = (foo_ptr) p;
    int n     = HEMP_ARG(int);
    char *ptr = HEMP_ARG(char *);
    debug("foo_init(%p, %d, %s)\n", foo, n, ptr);
    foo->n = n;
    foo->ptr = strdup(ptr);
    return foo;
}

void foo_wipe(hemp_ptr_t p) {
    foo_ptr foo = (foo_ptr) p;
    debug("foo_wipe(%p) %d: %s\n", foo, foo->n, foo->ptr);
    hemp_mem_free(foo->ptr);
}