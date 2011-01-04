#include <hemp/test.h>


void test_namespace();

int main(
    int argc, char **argv, char **env
) {
    plan(17);
    test_namespace();
    return done();
}



void test_namespace() {
    Hemp hemp = hemp_new();
    ok( hemp, "created hemp object" );

    HempNamespace foo = hemp_namespace("foo");
    ok( foo, "created foo namespace" );
    is( foo->name, "foo", "name is set: foo" );

    HempNamespace bar = hemp_namespace_child(foo, "bar");
    ok( bar, "created foo.bar namespace" );
    is( bar->path, "foo.bar", "path is set: foo.bar" );
    is( bar->name, "bar", "name is set: bar" );
    ok( bar->parent == foo, "bar parent is foo" );
    is( bar->parent->name, "foo", "bar's parent is foo" );

    HempNamespace foobar = hemp_namespace("foo.bar");
    ok( foobar, "fetched foo.bar namespace" );
    ok( foobar->id == bar->id, "foo.bar id is correct" );

    HempNamespace wam = hemp_namespace("foo.bar.baz.wam");
    ok( wam, "fetched foo.bar.baz.wam namespace" );
    is( wam->path, "foo.bar.baz.wam", "wam path");
    ok( 
        wam->parent->parent->id == bar->id, 
        "foo.bar.baz.wam has correct grandparent" 
    );

    HempNamespace bam = hemp_namespace("foo.bar.baz.bam");
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


