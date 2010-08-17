#include <hemp/test.h>

void test_list();
hemp_cstr_p list_as_text(hemp_list_p);


int
main(int argc, char **argv, char **env) {
    plan(6);
    test_list();
    return done();
}


void test_list() {
    hemp_list_p list = hemp_list_init();
    pass("created list");

    hemp_list_push(list, "foo");
    pass("appended foo");
    ok( 
        list->length == 1,
        "one item in list"
    );

    hemp_list_push(list, "bar");
    pass("appended bar");
    ok( 
        list->length == 2,
        "two items in list"
    );
    
    ok( 
        hemp_cstr_eq(list_as_text(list), "foo, bar"),
        "list text is foo, bar"
    );
    
    hemp_list_free(list);
}



char buffer[255];

hemp_bool_t 
hemp_text_append(
    hemp_list_p  list, 
    hemp_pos_t   pos, 
    hemp_mem_p   item
) {
    if (buffer[0])
        strcat(buffer, ", ");
    strcat(buffer, item);
    return HEMP_TRUE;
}


hemp_cstr_p
list_as_text(
    hemp_list_p list
) {
    buffer[0] = '\0';
    hemp_list_each(list, &hemp_text_append);
    return buffer;
}
