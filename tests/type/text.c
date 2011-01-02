#include <hemp/test.h>

void test_text();


int main(
    int argc, char **argv, char **env
) {
    plan(46);
    test_text();
    return done();
}



void test_text() {
    HempText message = hemp_text_new_size(25);
    HempText badger, copycat;

    ok( message, "created message" );
    ok( message->length   == 0, "length is %d", message->length );
    ok( message->capacity == 25, "capacity is %d", message->capacity );

    ok( hemp_text_append_string(message, "This is some text..."),
        "added some text"
    );
    ok( message->length   == 20, "length is %d", message->length );
    ok( message->capacity == 25, "capacity is still %d", message->capacity );

    ok( hemp_text_append_string(message, "and this is some more"),
        "added more text"
    );

    ok( message->length   == 41, "length is %d", message->length );
    ok( message->capacity == 41, "capacity is now %d", message->capacity );
    
//  printf("[%s] (%d)\n", message->string, strlen(message->string));

    ok( hemp_text_append_string(message, "!"),
        "added exclamation"
    );

    ok( message->length   == 42, "length is %d", message->length );
    ok( message->capacity == 42, "capacity is now %d", message->capacity );

    hemp_text_free(message);


    message = hemp_text_from_string("Hello ");
    eq( message->length, 6, "length is 6" );
    is( message->string, "Hello ", message->string );

    hemp_text_append_string(message, "World");
    eq( message->length, 11, "length is 11" );
    is( message->string, "Hello World", message->string );

    copycat = hemp_text_from_text(message);
    eq( copycat->length, 11, "copycat length is 11" );
    is( copycat->string, "Hello World", copycat->string );

    hemp_text_append_string(copycat, " Hello");
    ok( copycat->length == 17, "appended: %s", copycat->string );
    is( copycat->string, "Hello World Hello", copycat->string );

    badger  = hemp_text_from_string(" Badger");
    eq( badger->length, 7, "new badger string is 7 long" );

    hemp_text_append_text(copycat, badger);
    eq( copycat->length,  24, "appended text is 24 long" );
    is( copycat->string, "Hello World Hello Badger", "combined text correct" );

    hemp_text_insert_string(copycat, 0, "START ");
    is( 
        copycat->string, "START Hello World Hello Badger",
        "inserted at start"
    );

    hemp_text_insert_string(copycat, 12, "MID ");
    is( 
        copycat->string, "START Hello MID World Hello Badger",
        "inserted at offset from start"
    );

    hemp_text_insert_string(copycat, -6, "END ");
    is( 
        copycat->string, "START Hello MID World Hello END Badger",
        "inserted at offset from end"
    );

    hemp_text_insert_string(copycat, 500, " AFTER");
    is( 
        copycat->string, "START Hello MID World Hello END Badger AFTER",
        "inserted at offset past the end"
    );

    hemp_text_insert_string(copycat, -500, "BEFORE ");
    is( 
        copycat->string, "BEFORE START Hello MID World Hello END Badger AFTER",
        "inserted at offset from end past the beginning"
    );

    hemp_text_free(message);
    hemp_text_free(badger);
    hemp_text_free(copycat);


    /* append */
    message = hemp_text_new_size(3);
    ok( message->capacity == 3, "capacity is %d", message->capacity );
    hemp_text_append_stringn(message, "foobar", 3);
    ok( message->capacity == 3, "capacity is still %d", message->capacity );
    ok( hemp_string_eq(message->string, "foo"), "added foo" );

    hemp_text_append_stringn(message, "pow", 1);
    ok( message->capacity == 4, "capacity is now %d", message->capacity );
    ok( hemp_string_eq(message->string, "foop"), message->string );

    hemp_text_append_stringn(message, "wow", 0);
    ok( message->capacity == 4, "added nothing, capacity is %d", message->capacity );

    hemp_text_append_string(message, "");
    ok( message->length   == 4, "added nothing again, length is %d", message->length );
    ok( message->capacity == 4, "added nothing again, capacity is %d", message->capacity );


    /* replace */
    hemp_text_replace_string(message, "replacement");
    is( message->string, "replacement", "replaced text with 'replacement'" );
    eq( message->length, 11, "increased length to 11" );
    eq( message->capacity, 11, "increased capacity to 11" );

    hemp_text_replace_string(message, "foo");
    is( message->string, "foo", "replaced text with 'foo'" );
    eq( message->length, 3, "reduced length to 3" );
    eq( message->capacity, 11, "capacity remains at 11" );

    hemp_text_free(message);


    message = hemp_text_new_format(
        "The %s sat on the %s and %s.", 
        "cat", "mat", "shat"
    );
    ok( message, "created text from format" );
    is( message->string, "The cat sat on the mat and shat.", "message is ok" );
    eq( message->length, 32, "length is 32" );
    eq( message->capacity, 32, "capacity is 32" );
    hemp_text_free(message);
}
