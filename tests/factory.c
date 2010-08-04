#include <hemp.h>
#include <tap.h>

void test_factory();
void test_factory_wildcard();
char result[255];
int  n = 0;

int
main(
    int  argc, 
    char **argv, 
    char **env
) {
    plan_tests(22);

    test_factory();
    hemp_mem_trace_ok();
    hemp_mem_trace_reset();

    test_factory_wildcard();
    hemp_mem_trace_ok();
    hemp_mem_trace_reset();

    return exit_status();
}


hemp_mem_p
constructor(
    hemp_mem_p msg,
    hemp_mem_p name
) {
    sprintf(result, "[%s] constructor for %s", (char *) msg, (char *) name);
    n++;
    return result;
}


void test_factory() {
    hemp_factory_p factory = hemp_factory_init();
    ok( factory, "created factory" );

    hemp_factory_constructor(
        factory, "foo", (hemp_actor_f) &constructor, "FOO"
    );
    hemp_mem_p instance = hemp_factory_instance(
        factory, "foo"
    );
    ok( instance, "got a foo instance" );
    ok( hemp_cstr_eq(instance, "[FOO] constructor for foo"), "constructed OK" );
    ok( n == 1, "made one constructor call" );

    instance = hemp_factory_instance(
        factory, "foo"
    );
    ok( instance, "got another foo instance" );
    ok( hemp_cstr_eq(instance, "[FOO] constructor for foo"), "constructed OK again" );
    ok( n == 1, "still made only one constructor call" );

    hemp_factory_free(factory);
}


void test_factory_wildcard() {
    hemp_factory_p factory = hemp_factory_init();
    ok( factory, "created wildcard factory" );

    hemp_factory_constructor(
        factory, "foo.*", (hemp_actor_f) &constructor, "FOO"
    );
    hemp_factory_constructor(
        factory, "bar.*", (hemp_actor_f) &constructor, "BAR"
    );
    hemp_factory_constructor(
        factory, "foo.bar.*", (hemp_actor_f) &constructor, "FOO.BAR"
    );

    hemp_mem_p instance = hemp_factory_instance(factory, "foo.splat");
    ok( instance, "got foo.splat instance" );
    ok( hemp_cstr_eq(instance, "[FOO] constructor for splat"), "constructed foo.splat" );
    ok( n == 2, "made two constructor calls" );

    instance = hemp_factory_instance(factory, "foo.splat");
    ok( instance, "got foo.splat instance again" );
    ok( hemp_cstr_eq(instance, "[FOO] constructor for splat"), "fetched foo.splat again" );
    ok( n == 2, "still made only two constructor calls" );

    instance = hemp_factory_instance(factory, "foo.splat.blam");
    ok( instance, "got foo.splat.blam instance" );
    ok( hemp_cstr_eq(instance, "[FOO] constructor for splat.blam"), "fetched foo.splat.blam" );
    ok( n == 3, "now made three constructor calls" );

    instance = hemp_factory_instance(factory, "foo.bar.baz");
    ok( instance, "got foo.bar.baz instance" );
    ok( hemp_cstr_eq(instance, "[FOO.BAR] constructor for baz"), "fetched foo.bar.baz" );
    ok( n == 4, "now made four constructor calls" );

    hemp_factory_free(factory);
}
