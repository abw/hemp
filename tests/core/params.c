#include <hemp/test.h>

void test_params();

int main(
    int argc, char **argv, char **env
) {
    plan(6);
    test_params();
    return done();
}


void test_params() {
    hemp_proto params = hemp_proto_new();
    ok( params, "got params" );

    hemp_proto_add_item(params, hemp_string_copy("foo"));
    hemp_proto_add_item(params, hemp_string_copy("bar"));
    hemp_proto_add_list(params, hemp_string_copy("baz"));
    hemp_proto_add_hash(params, hemp_string_copy("bop"));

    hemp_string i = hemp_proto_item(params, "foo");

    is( hemp_proto_item(params, "foo"), "item", "foo is an item" );
    is( hemp_proto_item(params, "bar"), "item", "bar is an item" );
    is( hemp_proto_item(params, "baz"), "list", "baz is a list" );
    is( hemp_proto_item(params, "bop"), "hash", "bop is a hash" );

    ok( ! hemp_proto_item(params, "floop"), "no floop" );

    hemp_proto_free(params);
}