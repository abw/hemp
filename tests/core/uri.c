#include <hemp/test.h>

void test_uri();
void test_path();
void test_relative_path();
void test_relative_paths();

int main(
    int argc, char **argv, char **env
) {
    plan(63);
    test_path();
    test_uri();
    test_relative_path();
    test_relative_paths();
    return done();
}


void test_path_match(
    HempString base,
    HempString rel,
    HempString expect
) {
    HempString result = hemp_uri_path_relative(base, rel);
    ok( 
        hemp_string_eq(result, expect), 
        "path %s + %s = %s", 
        base, rel, result
    );
    hemp_mem_free(result);
}

void test_path() {
    HempString base = "/foo/bar/baz";
    test_path_match(base, "/wam/bam", "/wam/bam");
    test_path_match(base, "wam/bam", "/foo/bar/wam/bam");
    test_path_match(base, "./wam/bam", "/foo/bar/wam/bam");
    test_path_match(base, "../wam/bam", "/foo/wam/bam");
    test_path_match(base, "../../wam/bam", "/wam/bam");
    test_path_match(base, "../../../wam/bam", "/wam/bam");
    test_path_match(base, "../../../../wam/bam", "/wam/bam");
    test_path_match(base, "./wam/././bam", "/foo/bar/wam/bam");
    test_path_match(base, "./wam/oops/../mistake/../bam", "/foo/bar/wam/bam");
    test_path_match(base, ".wam/..bam/...mam", "/foo/bar/.wam/..bam/...mam");

    base = "/foo/bar/";
    test_path_match(base, "/wam/bam", "/wam/bam");
    test_path_match(base, "wam/bam", "/foo/bar/wam/bam");
    test_path_match(base, "./wam/bam", "/foo/bar/wam/bam");
    test_path_match(base, "../wam/bam", "/foo/wam/bam");
    test_path_match(base, "../../wam/bam", "/wam/bam");
    test_path_match(base, "../../../wam/bam", "/wam/bam");
    test_path_match(base, "../../../../wam/bam", "/wam/bam");
    test_path_match(base, "./wam/././bam", "/foo/bar/wam/bam");
    test_path_match(base, "./wam/oops/../mistake/../bam", "/foo/bar/wam/bam");
    test_path_match(base, ".wam/..bam/...mam", "/foo/bar/.wam/..bam/...mam");
}



void test_uri() {
    HempUri uri = hemp_uri_new();
    ok( uri, "constructed new uri" );

    hemp_uri_split(
        uri, 
        "hemp://abw@badgerpower.com:420/over/there?animal=badger#nose"
    );
    is( uri->scheme,    "hemp",             "matched URI scheme"    );
    is( uri->user,      "abw",              "matched URI user"      );
    is( uri->host,      "badgerpower.com",  "matched URI host"      );
    is( uri->port,      "420",              "matched URI port"      );
    is( uri->path,      "/over/there",      "matched URI path"      );
    is( uri->query,     "animal=badger",    "matched URI query"     );
    is( uri->fragment,  "nose",             "matched URI fragment"  );

    HempString s = hemp_hash_fetch_string(uri->params, "animal");
    is( s, "badger", "matched URI parameter" );

    HempValue v = hemp_list_item(uri->paths, 0);
    ok( hemp_is_empty(v), "matched empty zeroth URI path element" );
    
    s = hemp_list_item_string(uri->paths, 1);
    is( s, "over", "matched first URI path element" );
    s = hemp_list_item_string(uri->paths, 2);
    is( s, "there", "matched second URI path element" );

    hemp_uri_free(uri);
}


void test_relative_path() {
    HempUri base = hemp_uri_from_string(
        "hemp://abw@badgerpower.com:420/over/there?animal=badger#nose"
    );
    ok( base, "constructed new uri" );

    HempUri uri = hemp_uri_relative_string(base, "./here?cheese=cheddar#pickle");
    ok( uri, "got relative uri" );
    is( uri->scheme,    "hemp",             "matched rel URI scheme"    );
    is( uri->user,      "abw",              "matched rel URI user"      );
    is( uri->host,      "badgerpower.com",  "matched rel URI host"      );
    is( uri->port,      "420",              "matched rel URI port"      );
    is( uri->path,      "/over/here",       "matched rel URI path"      );
    is( uri->query,     "cheese=cheddar",   "matched rel URI query"     );
    is( uri->fragment,  "pickle",           "matched rel URI fragment"  );

    hemp_uri_free(uri);
    hemp_uri_free(base);
}


void test_relative_match(
    HempUri    base,
    HempString rels,
    HempString expect
) {
    HempUri relu = hemp_uri_relative_string(base, rels);
//    hemp_debug_msg("new uri: %p\n", relu->uri);
    ok( 
        hemp_string_eq(relu->path, expect), 
        "uri %s + %s = %s", 
        base->path, rels, relu->path
    );
    hemp_uri_free(relu);
}


void test_relative_paths() {
    HempUri base = hemp_uri_from_string("/foo/bar/baz");
    ok( base, "constructed new base uri" );

    test_relative_match(base, "/wam/bam", "/wam/bam");
    test_relative_match(base, "wam/bam", "/foo/bar/wam/bam");
    test_relative_match(base, "./wam/bam", "/foo/bar/wam/bam");
    test_relative_match(base, "../wam/bam", "/foo/wam/bam");
    test_relative_match(base, "../../wam/bam", "/wam/bam");
    test_relative_match(base, "../../../wam/bam", "/wam/bam");
    test_relative_match(base, "../../../../wam/bam", "/wam/bam");
    test_relative_match(base, "./wam/././bam", "/foo/bar/wam/bam");
    test_relative_match(base, "./wam/oops/../mistake/../bam", "/foo/bar/wam/bam");
    test_relative_match(base, ".wam/..bam/...mam", "/foo/bar/.wam/..bam/...mam");
    
    hemp_uri_free(base);

    base = hemp_uri_from_string("/foo/bar/");
    ok( base, "constructed new base uri with trailing slash" );

    test_relative_match(base, "/wam/bam", "/wam/bam");
    test_relative_match(base, "wam/bam", "/foo/bar/wam/bam");
    test_relative_match(base, "./wam/bam", "/foo/bar/wam/bam");
    test_relative_match(base, "../wam/bam", "/foo/wam/bam");
    test_relative_match(base, "../../wam/bam", "/wam/bam");
    test_relative_match(base, "../../../wam/bam", "/wam/bam");
    test_relative_match(base, "../../../../wam/bam", "/wam/bam");
    test_relative_match(base, "./wam/././bam", "/foo/bar/wam/bam");
    test_relative_match(base, "./wam/oops/../mistake/../bam", "/foo/bar/wam/bam");
    test_relative_match(base, ".wam/..bam/...mam", "/foo/bar/.wam/..bam/...mam");

    hemp_uri_free(base);

}
