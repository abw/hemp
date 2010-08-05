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


hemp_action_p
wildcard(
    hemp_mem_p      msg,
    hemp_cstr_p     name
) {
    debug_cyan("wildcard action for %s\n", name);
    return hemp_action_init(
        (hemp_actor_f) constructor, msg
    );
}


void test_factory() {
    hemp_factory_p factory = hemp_factory_init();
    ok( factory, "created factory" );

    hemp_factory_register(
        factory, "foo", (hemp_actor_f) &constructor, "FOO"
    );
    hemp_mem_p instance = hemp_factory_instance(
        factory, "foo"
    );
    ok( instance, "got a foo instance" );
    is( instance, "[FOO] constructor for foo", "constructed OK" );
    ok( n == 1, "made one constructor call" );

    instance = hemp_factory_instance(
        factory, "foo"
    );
    ok( instance, "got another foo instance" );
    is( instance, "[FOO] constructor for foo", "constructed OK again" );
    ok( n == 1, "still made only one constructor call" );

    hemp_factory_free(factory);
}


void test_factory_wildcard() {
    hemp_factory_p factory = hemp_factory_init();
    ok( factory, "created wildcard factory" );

    hemp_factory_register(
        factory, "foo.*", (hemp_actor_f) &wildcard, "FOO"
    );
    hemp_factory_register(
        factory, "bar.*", (hemp_actor_f) &wildcard, "BAR"
    );
    hemp_factory_register(
        factory, "foo.bar.*", (hemp_actor_f) &wildcard, "FOO.BAR"
    );

    hemp_mem_p instance = hemp_factory_instance(factory, "foo.splat");
    ok( instance, "got foo.splat instance" );
    is( instance, "[FOO] constructor for foo.splat", "constructed foo.splat" );
    ok( n == 2, "made two constructor calls" );

    instance = hemp_factory_instance(factory, "foo.splat");
    ok( instance, "got foo.splat instance again" );
    is( instance, "[FOO] constructor for foo.splat", "fetched foo.splat again" );
    ok( n == 2, "still made only two constructor calls" );

    instance = hemp_factory_instance(factory, "foo.splat.blam");
    ok( instance, "got foo.splat.blam instance" );
    is( instance, "[FOO] constructor for foo.splat.blam", "fetched foo.splat.blam" );
    ok( n == 3, "now made three constructor calls" );

    instance = hemp_factory_instance(factory, "foo.bar.baz");
    ok( instance, "got foo.bar.baz instance" );
    is( instance, "[FOO.BAR] constructor for foo.bar.baz", "fetched foo.bar.baz" );
    ok( n == 4, "now made four constructor calls" );

    hemp_factory_free(factory);
}
