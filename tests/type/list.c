#include <hemp/test.h>

void test_list();
void test_list_copy();
void test_list_resize();
void test_list_push();
HempString list_as_text(HempList);


int
main(int argc, char **argv, char **env) {
    plan(37);
    test_list();
    test_list_push();
    test_list_copy();
    test_list_resize();
    return done();
}


void test_list() {
    Hemp      hemp = hemp_new();
    HempList list = hemp_list_new();
    
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
    HempContext context = hemp_context(hemp);
    HempValue   value   = hemp_list_val(list);
    ok( hemp_is_list(value), "value is a list" );
    
    HempValue   length  = hemp_send(value, "length", context);
    ok( hemp_is_defined(length), "got define length" );
    ok( hemp_is_integer(length), "got an integer length" );
    eq( hemp_val_int(length), 2, "list length is 2" );

    hemp_list_push_integer(list, 10);
    eq( list->length, 3, "three items in list" );
    HempValue ten = hemp_list_pop(list);
    ok( hemp_is_integer(ten), "popped ten" );
    eq( hemp_val_int(ten), 10, "got correct value for ten" );
    
    hemp_context_free(context);
    hemp_list_free(list);
    hemp_free(hemp);
}

void test_list_push() {
    HempList list1 = hemp_list_new();
    hemp_list_push_integer(list1, 10);
    hemp_list_push_integer(list1, 20);
    eq( list1->length, 2, "first list has two items" );

    HempList list2 = hemp_list_new();
    hemp_list_push_integer(list2, 30);
    hemp_list_push_integer(list2, 40);
    hemp_list_push_integer(list2, 50);
    eq( list2->length, 3, "second list has three items" );

    hemp_list_push_list(list1, list2);
    eq( list1->length, 5, "combined list has five items" );

    HempValue value = hemp_list_item(list1, 0);
    eq( hemp_val_int(value), 10, "item zero is 10" );

    value = hemp_list_item(list1, 1);
    eq( hemp_val_int(value), 20, "item one is 20" );

    value = hemp_list_item(list1, 2);
    eq( hemp_val_int(value), 30, "item two is 30" );

    value = hemp_list_item(list1, 3);
    eq( hemp_val_int(value), 40, "item three is 40" );

    value = hemp_list_item(list1, 4);
    eq( hemp_val_int(value), 50, "item four is 40" );

    HempString foos = "Hello World";
    HempValue  foov = hemp_str_val(foos);
    hemp_list_push(list1, foov);
    HempValue  barv = hemp_list_item(list1, list1->length - 1);
    HempString bars = hemp_val_str(barv);
    is( foos, bars, "round-trip string value push" );
    
    

    hemp_list_free(list1);
    hemp_list_free(list2);
}


void test_list_copy() {
    HempList list = hemp_list_new();
    hemp_list_push_integer(list, 10);
    hemp_list_push_number(list, 1.23);
    hemp_list_push_string(list, "Hello World!");
    
    HempList copy = hemp_list_copy(list);
    ok( copy, "copied list: %p", copy );
    eq( copy->length, 3, "three items in copy" );
    
    HempValue value = hemp_list_item(copy, 0);
    ok( hemp_is_integer(value), "item zero is integer" );

    value = hemp_list_item(copy, 1);
    ok( hemp_is_number(value), "item one is number" );

    value = hemp_list_item(copy, 2);
    ok( hemp_is_string(value), "item two is string" );
    
    hemp_list_free(copy);
    hemp_list_free(list);
}


void test_list_resize() {
    HempList list = hemp_list_new();
    hemp_list_push_integer(list, 10);
    hemp_list_push_number(list, 1.23);
    hemp_list_push_string(list, "Hello World!");
    
    HempSize new_size = hemp_list_resize(list, 6);
    eq( new_size, 6, "expanded list to 6 items" );
    eq( list->length, 6, "six items in list" );
    
    HempValue value = hemp_list_item(list, 0);
    ok( hemp_is_integer(value), "item zero is integer" );

    value = hemp_list_item(list, 1);
    ok( hemp_is_number(value), "item one is number" );

    value = hemp_list_item(list, 2);
    ok( hemp_is_string(value), "item two is string" );

    value = hemp_list_item(list, 3);
    ok( hemp_is_missing(value), "item three is missing" );

    value = hemp_list_item(list, 4);
    ok( hemp_is_missing(value), "item four is missing" );

    value = hemp_list_item(list, 5);
    ok( hemp_is_missing(value), "item five is missing" );

    new_size = hemp_list_resize(list, 3);
    eq( new_size, 3, "reduced list back to 3 items" );
    eq( list->length, 3, "three items in list again" );

    hemp_list_free(list);
}



char buffer[255];

HempBool 
hemp_text_append(
    HempList  list, 
    HempPos   pos, 
    HempValue value
) {
    if (buffer[0])
        strcat(buffer, ", ");
    strcat(buffer, hemp_val_str(value) );
    return HEMP_TRUE;
}


HempString
list_as_text(
    HempList list
) {
    buffer[0] = '\0';
    hemp_list_each(list, &hemp_text_append);
    return buffer;
}
