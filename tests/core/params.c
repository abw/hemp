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
    hemp_params_p params = hemp_params_new();
    ok( params, "got params" );

    hemp_params_add_item(params, hemp_string_copy("foo"));
    hemp_params_add_item(params, hemp_string_copy("bar"));
    hemp_params_add_list(params, hemp_string_copy("baz"));
    hemp_params_add_hash(params, hemp_string_copy("bop"));

    hemp_str_p i = hemp_params_item(params, "foo");

    is( hemp_params_item(params, "foo"), "item", "foo is an item" );
    is( hemp_params_item(params, "bar"), "item", "bar is an item" );
    is( hemp_params_item(params, "baz"), "list", "baz is a list" );
    is( hemp_params_item(params, "bop"), "hash", "bop is a hash" );

    ok( ! hemp_params_item(params, "floop"), "no floop" );

    hemp_params_free(params);
}