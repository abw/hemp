#include <hemp/test.h>


void test_namespace();

int main(
    int argc, char **argv, char **env
) {
    plan(9);
    test_namespace();
    return done();
}



void test_namespace() {
    hemp_p hemp = hemp_init();
    ok( hemp, "created hemp object" );

    hemp_namespace_p foo = hemp_namespace(hemp, "foo");
    ok( foo, "created foo namespace" );
    is( foo->name, "foo", "name is set: foo" );
    eq( foo->id, 1, "foo id is 1" );

    hemp_namespace_p bar = hemp_namespace_child(foo, "bar");
    ok( bar, "created foo.bar namespace" );
    is( bar->name, "bar", "name is set: bar" );
    ok( bar->parent == foo, "bar parent is foo" );
    is( bar->parent->name, "foo", "bar's parent is foo" );
    eq( bar->id, 2, "bar id is 1" );
    
    hemp_free(hemp);
}


