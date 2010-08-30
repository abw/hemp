#include <hemp/test.h>

void test_list();
hemp_str_p list_as_text(hemp_list_p);


int
main(int argc, char **argv, char **env) {
    plan(6);
    test_list();
    return done();
}


void test_list() {
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
    
    hemp_list_free(list);
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
