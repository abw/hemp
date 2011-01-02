#include <hemp/test.h>

void test_string();


int main(
    int argc, char **argv, char **env
) {
    plan(2);
    test_string();
    return done();
}


void test_string() {
    HempString s = hemp_string_sprintf("Hello %s", "World");
    ok( s, "hemp_string_sprintf() created string" );
    is( s, "Hello World", "string is 'Hello World'" );
    hemp_string_free(s);
}

