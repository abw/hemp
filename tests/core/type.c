#include <hemp/test.h>
#include <math.h>

void test_types();


int main(
    int argc, char **argv, char **env
) {
    plan(5);
    test_types();
    return done();
}


void test_types() {
    hemp_p hemp = hemp_init();

    hemp_value_t number = hemp_num_val(3.14159);
    ok( hemp_is_number(number), "created a number" );

    hemp_mem_p method;
    method = hemp_object_method(number, "name");
    ok( method, "got a pointer to name method");

    hemp_value_t value = hemp_send(number, "name");
    ok( hemp_is_defined(value), "got result" );
    is( hemp_val_str(value), "Number", "got number name" );

    value = hemp_send(number, "no_such_method");
    ok( hemp_is_missing(value), "no_such_method is missing" );
    
    hemp_free(hemp);
}

