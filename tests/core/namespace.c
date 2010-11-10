#include <hemp/test.h>


void test_namespace();

int main(
    int argc, char **argv, char **env
) {
    plan(15);
    test_namespace();
    return done();
}



void test_namespace() {
    hemp_p hemp = hemp_init();
    ok( hemp, "created hemp object" );

    hemp_namespace_p foo = hemp_namespace("foo");
    ok( foo, "created foo namespace" );
    is( foo->name, "foo", "name is set: foo" );

    hemp_namespace_p bar = hemp_namespace_child(foo, "bar");
    ok( bar, "created foo.bar namespace" );
    is( bar->name, "bar", "name is set: bar" );
    ok( bar->parent == foo, "bar parent is foo" );
    is( bar->parent->name, "foo", "bar's parent is foo" );

    hemp_namespace_p foobar = hemp_namespace("foo.bar");
    ok( foobar, "fetched foo.bar namespace" );
    ok( foobar->id == bar->id, "foo.bar id is correct" );

    hemp_namespace_p wam = hemp_namespace("foo.bar.baz.wam");
    ok( wam, "fetched foo.bar.baz.wam namespace" );
    ok( 
        wam->parent->parent->id == bar->id, 
        "foo.bar.baz.wam has correct grandparent" 
    );

    hemp_namespace_p bam = hemp_namespace("foo.bar.baz.bam");
    ok( bam, "fetched foo.bar.baz.bam namespace" );
    ok( 
        bam->parent->parent->id == bar->id, 
        "foo.bar.baz.bam has correct grandparent" 
    );
    ok( 
        wam->parent->id == bam->parent->id, 
        "wam and bam have same parent" 
    );
    is( 
        wam->parent->name, "baz",
        "wam and bam's parent is baz"
    );

    hemp_free(hemp);
}


