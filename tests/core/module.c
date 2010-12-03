#include <hemp/test.h>


void test_module();


int main(
    int argc, char **argv, char **env
) {
    plan(3);
    test_module();
    return done();
}


void test_module() {
    hemp_hemp hemp = hemp_new();
    ok( hemp, "created hemp object" );

    hemp_module foo = hemp_module_new("foo");
    ok( foo, "created foo module" );

    ok( hemp_module_load(foo), "loaded foo module" );

    hemp_module_free(foo);

//    hemp_module_use(hemp, "foo");

    hemp_free(hemp);
}


