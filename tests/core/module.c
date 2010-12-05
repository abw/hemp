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

    hemp_module foo1 = hemp_use_module(hemp, "modules/foo.so");
    ok( foo1, "created foo module" );

    hemp_module foo2 = hemp_use_module(hemp, "modules/foo.so");

    ok( foo2, "loaded foo module again" );

//    hemp_module_free(foo);

//    hemp_module_use(hemp, "foo");

    hemp_free(hemp);
}


