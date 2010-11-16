#include <hemp/test.h>

void test_params();

int main(
    int argc, char **argv, char **env
) {
    plan(1);
    test_params();
    return done();
}


void test_params() {
    hemp_params_p params = hemp_params_new();
    ok( params, "got params" );
    hemp_params_free(params);
}