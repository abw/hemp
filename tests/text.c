#include <hemp.h>
#include "tap.h"

void test_text();


int
main(
    int argc, 
    char **argv, 
    char **env
) {
    plan_tests(36);
    test_text();
    hemp_mem_trace_ok();
    return exit_status();
}



void test_text() {
    hemp_text_p message = hemp_text_init(25);
    hemp_text_p badger, copycat;

    ok( message, "created message" );
    ok( message->length   == 0, "length is %d", message->length );
    ok( message->capacity == 25, "capacity is %d", message->capacity );

    ok( hemp_text_append_cstr(message, "This is some text..."),
        "added some text"
    );
    ok( message->length   == 20, "length is %d", message->length );
    ok( message->capacity == 25, "capacity is still %d", message->capacity );

    ok( hemp_text_append_cstr(message, "and this is some more"),
        "added more text"
    );

    ok( message->length   == 41, "length is %d", message->length );
    ok( message->capacity == 41, "capacity is now %d", message->capacity );
    
//  printf("[%s] (%d)\n", message->string, strlen(message->string));

    ok( hemp_text_append_cstr(message, "!"),
        "added exclamation"
    );

    ok( message->length   == 42, "length is %d", message->length );
    ok( message->capacity == 42, "capacity is now %d", message->capacity );

    hemp_text_free(message);


    message = hemp_text_from_cstr("Hello ");
    ok( message->length == 6, message->string );

    hemp_text_append_cstr(message, "World");
    ok( message->length == 11, message->string );

    copycat = hemp_text_from_text(message);
    ok( copycat->length == 11, "copied: %s", copycat->string );

    hemp_text_append_cstr(copycat, " Hello");
    ok( copycat->length == 17, "appended: %s", copycat->string );

    badger  = hemp_text_from_cstr(" Badger");
    ok( badger->length == 7, badger->string );

    hemp_text_append_text(copycat, badger);
    ok( copycat->length == 24, "appended: %s", copycat->string );

    hemp_text_insert_cstr(copycat, 0, "START ");
    is( 
        copycat->string, "START Hello World Hello Badger",
        "inserted at start"
    );

    hemp_text_insert_cstr(copycat, 12, "MID ");
    is( 
        copycat->string, "START Hello MID World Hello Badger",
        "inserted at offset from start"
    );

    hemp_text_insert_cstr(copycat, -6, "END ");
    is( 
        copycat->string, "START Hello MID World Hello END Badger",
        "inserted at offset from end"
    );

    hemp_text_insert_cstr(copycat, 500, " AFTER");
    is( 
        copycat->string, "START Hello MID World Hello END Badger AFTER",
        "inserted at offset past the end"
    );

    hemp_text_insert_cstr(copycat, -500, "BEFORE ");
    is( 
        copycat->string, "BEFORE START Hello MID World Hello END Badger AFTER",
        "inserted at offset from end past the beginning"
    );

    hemp_text_free(message);
    hemp_text_free(badger);
    hemp_text_free(copycat);

    message = hemp_text_init(3);
    ok( message->capacity == 3, "capacity is %d", message->capacity );
    hemp_text_append_cstrn(message, "foobar", 3);
    ok( message->capacity == 3, "capacity is still %d", message->capacity );
    ok( hemp_cstr_eq(message->string, "foo"), "added foo" );

    hemp_text_append_cstrn(message, "pow", 1);
    ok( message->capacity == 4, "capacity is now %d", message->capacity );
    ok( hemp_cstr_eq(message->string, "foop"), message->string );

    hemp_text_append_cstrn(message, "wow", 0);
    ok( message->capacity == 4, "added nothing, capacity is %d", message->capacity );

    hemp_text_append_cstr(message, "");
    ok( message->length   == 4, "added nothing again, length is %d", message->length );
    ok( message->capacity == 4, "added nothing again, capacity is %d", message->capacity );

    hemp_text_free(message);


    message = hemp_text_init_format(
        "The %s sat on the %s and %s.", 
        "cat", "mat", "shat"
    );
    ok( message, "created text from format" );
    is( message->string, "The cat sat on the mat and shat.", "message is ok" );
    ok( message->length == 32, "length is %d, expected 32", message->length );
    ok( message->capacity == 32, "capacity is %d, expected 32", message->capacity );
    hemp_text_free(message);
}
