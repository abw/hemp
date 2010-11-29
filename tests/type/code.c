#include <hemp/test.h>

void test_code();


int
main(
    int argc, char **argv, char **env
) {
    plan(1);
    test_code();
    return done();
}


void test_code() {
    hemp_hemp      hemp = hemp_new();
    hemp_code code = hemp_code_new();
    
    ok(code, "created code");

    hemp_code_free(code);
    hemp_free(hemp);
}

