#include <hemp/test.h>

void test_list();
hemp_str_p list_as_text(hemp_list_p);


int
main(int argc, char **argv, char **env) {
    plan(10);
    test_list();
    return done();
}


void test_list() {
    hemp_p      hemp = hemp_init();
    hemp_list_p list = hemp_list_init();
    
    pass("created list");

    hemp_list_push(list, hemp_str_val("foo") );
    pass("appended foo");
    ok( 
        list->length == 1,
        "one item in list"
    );

    hemp_list_push_string(list, "bar");
    pass("appended bar");
    ok( 
        list->length == 2,
        "two items in list"
    );
    
    ok( 
        hemp_string_eq(list_as_text(list), "foo, bar"),
        "list text is foo, bar"
    );

    /* test methods */
    hemp_context_p context = hemp_context(hemp);
    hemp_value_t   value   = hemp_list_val(list);
    ok( hemp_is_list(value), "value is a list" );
    
    hemp_value_t   length  = hemp_send(value, "length", context);
    ok( hemp_is_defined(length), "got define length" );
    ok( hemp_is_integer(length), "got an integer length" );
    eq( hemp_val_int(length), 2, "list length is 2" );
    
    hemp_context_free(context);
    hemp_list_free(list);
    hemp_free(hemp);
}



char buffer[255];

hemp_bool_t 
hemp_text_append(
    hemp_list_p  list, 
    hemp_pos_t   pos, 
    hemp_value_t value
) {
    if (buffer[0])
        strcat(buffer, ", ");
    strcat(buffer, hemp_val_str(value) );
    return HEMP_TRUE;
}


hemp_str_p
list_as_text(
    hemp_list_p list
) {
    buffer[0] = '\0';
    hemp_list_each(list, &hemp_text_append);
    return buffer;
}
