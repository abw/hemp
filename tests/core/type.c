#include <hemp/test.h>
#include <math.h>

void test_types();
void test_type_registration();


int main(
    int argc, char **argv, char **env
) {
    plan(14);
    test_types();
    test_type_registration();
    return done();
}


void test_types() {
    hemp_hemp hemp            = hemp_init();
    hemp_context context = hemp_context_instance(hemp);

    hemp_value number = hemp_num_val(3.14159);
    ok( hemp_is_number(number), "created a number" );

    hemp_memory method;
    method = hemp_object_method(number, "name");
    ok( method, "got a pointer to name method");

    hemp_value value = hemp_send(number, "name", context);
    ok( hemp_is_defined(value), "got result" );
    is( hemp_val_str(value), "Number", "got number name" );

    value = hemp_send(number, "no_such_method", context);
    ok( hemp_is_missing(value), "no_such_method is missing" );

    hemp_text text = hemp_text_from_string("hello world!");
    value = hemp_send( hemp_text_val(text), "length", context );
    ok( hemp_is_defined(value), "got text length" );
    eq( hemp_val_int(value), 12, "length is 12" );
    hemp_text_free(text);
    
    /* try converting number to text */
    value = hemp_send(value, "text", context);
    if (hemp_is_missing(value)) {
        fail("No text method?");
        exit(1);
    }
    ok( hemp_is_defined(value), "got length as text" );
    is( hemp_val_text(value)->string, "12", "length text is 12" );

// Text value is now managed by context
//    hemp_text_free( hemp_val_text(value) );
    
    hemp_context_free(context);
    hemp_free(hemp);
}


hemp_type type1_constructor(
    hemp_int id,
    hemp_string name
) {
    return hemp_type_new(id, name);
}
    

void test_type_registration() {
    hemp_hemp hemp = hemp_init();
    ok( hemp, "created hemp" );

    hemp_type type1 = hemp_use_type("Type1", &type1_constructor);
    ok( type1, "created custom type 1" );

    hemp_type type2 = hemp_use_type("Type1", &type1_constructor);
    ok( type2, "created custom type 2" );
    ok( type1 == type2, "both the same type" );

    int n = type1->id;
    ok( n, "registered type: %d", n );

    hemp_free(hemp);
}

