#include <hemp/test.h>

void test_uri();

int main(
    int argc, char **argv, char **env
) {
    plan(9);
    test_uri();
    return done();
}


void test_uri() {
    hemp_uri uri = hemp_uri_new();
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

    hemp_string s = hemp_hash_fetch_string(uri->params, "animal");
    is( s, "badger", "matched URI parameter" );

    hemp_uri_free(uri);
}
