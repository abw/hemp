#include <stdio.h>
#include "hemp.h"
#include "hemp/text.h"
#include "tap.h"

void test_text();


int
main(int argc, char **argv, char **env)
{
    plan_tests(23);
    test_text();
    return exit_status();
}


void test_text() {
    hemp_text_t message = hemp_text_init(25);
    hemp_text_t badger, copycat;

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
    ok( 
        hemp_cstr_eq(copycat->string, "START Hello World Hello Badger"),
        "inserted at start"
    );

    hemp_text_insert_cstr(copycat, 12, "MID ");
    ok( 
        hemp_cstr_eq(copycat->string, "START Hello MID World Hello Badger"),
        "inserted at offset from start"
    );

    hemp_text_insert_cstr(copycat, -6, "END ");
    ok( 
        hemp_cstr_eq(copycat->string, "START Hello MID World Hello END Badger"),
        "inserted at offset from end"
    );

    hemp_text_insert_cstr(copycat, 500, " AFTER");
    ok( 
        hemp_cstr_eq(copycat->string, "START Hello MID World Hello END Badger AFTER"),
        "inserted at offset past the end"
    );

    hemp_text_insert_cstr(copycat, -500, "BEFORE ");
    ok( 
        hemp_cstr_eq(copycat->string, "BEFORE START Hello MID World Hello END Badger AFTER"),
        "inserted at offset from end past the beginning"
    );

    hemp_text_free(message);
    hemp_text_free(badger);
    hemp_text_free(copycat);

}
