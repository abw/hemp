#include <hemp/test.h>

void test_proto();

int main(
    int argc, char **argv, char **env
) {
    plan(6);
    test_proto();
    return done();
}


void test_proto() {
    HempProto proto = hemp_proto_new();
    ok( proto, "got proto" );

    hemp_proto_add_item(proto, hemp_string_copy("foo"));
    hemp_proto_add_item(proto, hemp_string_copy("bar"));
    hemp_proto_add_list(proto, hemp_string_copy("baz"));
    hemp_proto_add_hash(proto, hemp_string_copy("bop"));

    HempString i = hemp_proto_item(proto, "foo");
    HEMP_UNUSED(i);

    is( hemp_proto_item(proto, "foo"), "item", "foo is an item" );
    is( hemp_proto_item(proto, "bar"), "item", "bar is an item" );
    is( hemp_proto_item(proto, "baz"), "list", "baz is a list" );
    is( hemp_proto_item(proto, "bop"), "hash", "bop is a hash" );

    ok( ! hemp_proto_item(proto, "floop"), "no floop" );

    hemp_proto_free(proto);
}
