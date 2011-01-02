#include <hemp/test.h>


void test_module();


int main(
    int argc, char **argv, char **env
) {
    plan(1);
    test_module();
    return done();
}


void test_module() {
    Hemp hemp = hemp_new();
    ok( hemp, "created hemp object" );

    //    HempModule foo1 = hemp_use_module(hemp, "test", "foo");
    //    ok( foo1, "created foo module" );

    //    HempModule foo2 = hemp_use_module(hemp, "test", "foo");

    //    ok( foo2, "loaded foo module again" );

//    hemp_module_free(foo);

//    hemp_module_use(hemp, "foo");

    hemp_free(hemp);
}


