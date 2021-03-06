#include <hemp/test.h>


void test_factory();
void test_factory_wildcard();
char result[255];
int  n = 0;

int main(
    int argc, char **argv, char **env
) {
    plan(20);

    test_factory();
    test_factory_wildcard();

    return done();
}


HempMemory
constructor(
    HempMemory msg,
    HempMemory name
) {
    sprintf(result, "[%s] constructor for %s", (char *) msg, (char *) name);
    n++;
    return result;
}


HempAction
wildcard(
    HempMemory msg,
    HempString name
) {
    hemp_debug_cyan("wildcard action for %s\n", name);
    return hemp_action_new(
        (HempActor) constructor, msg
    );
}


void test_factory() {
    Hemp hemp = hemp_new();
    HempFactory factory = hemp_factory_new(hemp, "test");
    ok( factory, "created factory" );

    hemp_factory_register(
        factory, "foo", (HempActor) &constructor, "FOO"
    );
    HempMemory instance = hemp_factory_instance(
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
    hemp_free(hemp);
}


void test_factory_wildcard() {
    Hemp hemp = hemp_new();
    HempFactory factory = hemp_factory_new(hemp, "test");
    ok( factory, "created wildcard factory" );

    hemp_factory_register(
        factory, "foo.*", (HempActor) &wildcard, "FOO"
    );
    hemp_factory_register(
        factory, "bar.*", (HempActor) &wildcard, "BAR"
    );
    hemp_factory_register(
        factory, "foo.bar.*", (HempActor) &wildcard, "FOO.BAR"
    );

    HempMemory instance = hemp_factory_instance(factory, "foo.splat");
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
    hemp_free(hemp);
}
