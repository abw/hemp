#include <hemp/test.h>

void test_params();


int
main(int argc, char **argv, char **env) {
    plan(2);
    test_params();
    return done();
}


void test_params() {
    Hemp   hemp    = hemp_new();
    HempParams params  = hemp_params_new();
    
    pass("created params");

    /* test methods */
    HempContext context = hemp_context(hemp);
    HempValue   value   = hemp_params_val(params);
    ok( hemp_is_params(value), "value is params" );
    
//    HempValue   length  = hemp_send(value, "length", context);
//    ok( hemp_is_defined(length), "got define length" );
//    ok( hemp_is_integer(length), "got an integer length" );
//    eq( hemp_val_int(length), 2, "list length is 2" );
    
    hemp_context_free(context);
    hemp_params_free(params);
    hemp_free(hemp);
}

