#include <hemp/test.h>

jmp_buf state;


void test_error_jump();
void test_error_macros();
void test_error4();
void test_error_object();
void test_hemp_throw();
void test_error_finally();
void thrower(hemp_errno, char *msg);


int main(
    int argc, char **argv, char **env
) {
    plan(13);

    test_error_jump();
    /* test_error2() has been deprecated */
    test_error_macros();
    test_error_object();
    test_hemp_throw();

    test_error_finally();

    return done();
}


void test_error_jump() {
    int exception;
    
    if((exception = setjmp(state)) == 0) {              // try
        thrower(HEMP_ERROR_MEMORY, "hello world");
        fail("returned from thrower (this should not happen)\n");
    }
    else {
        switch (exception) {             
            case HEMP_ERROR_MEMORY:
                pass("caught HEMP_ERROR_MEMORY");
                break;
            case HEMP_ERROR_MISSING:
                fail("caught HEMP_ERROR_MISSING");
                break;
            default:
                fail("caught unknown exception: %d", exception);
        }
    }
}



void test_error_macros() {
    hemp_hemp hemp = hemp_new();

    /* better version with error stack local to hemp interpreter, 
     * but depends on 'hemp' local symbol being visible
     */
    HEMP_TRY
        HEMP_THROW(HEMP_ERROR_INVALID);
        fail("returned from thrower (this should not happen)");

    HEMP_CATCH(HEMP_ERROR_MEMORY);
        fail("caught HEMP_ERROR_MEMORY");

    HEMP_CATCH(HEMP_ERROR_MISSING);
        fail("caught HEMP_ERROR_MISSING");

    HEMP_CATCH(HEMP_ERROR_INVALID);
        pass("caught HEMP_ERROR_INVALID");

    HEMP_END;

    HEMP_TRY
        HEMP_THROW(HEMP_ERROR_INVALID);
        fail("returned from thrower (this should not happen)");

    HEMP_CATCH_ALL;
        pass("caught hemp error");

    HEMP_END;

    hemp_free(hemp);
}


void test_error4() {
    hemp_hemp hemp = hemp_new();

    HEMP_THROW(HEMP_ERROR_INVALID);
    fail("after throw - this should not happen");

    hemp_free(hemp);
}


void test_error_object() {
    hemp_error error = hemp_error_init(
        HEMP_ERROR_INVALID, "Bad badger!"
    );
    ok( error, "got error object" );
    ok( 
        hemp_string_eq(error->message, "Bad badger!"),
        "bad badger message"
    );
    hemp_error_free(error);
    return;

    error = hemp_error_initf(
        HEMP_ERROR_INVALID, "Bad %s: %s", "cheese", "badger"
    );
    ok( error, "got error object" );
    ok( 
        hemp_string_eq(error->message, "Bad cheese: badger"),
        "bad cheese message"
    );

    hemp_error_free(error);
}


void test_hemp_throw() {
    hemp_hemp hemp = hemp_new();

    HEMP_TRY;
        hemp_throw(
            hemp,
            HEMP_ERROR_INVALID, "cheese", "badger"
        );

    HEMP_CATCH_ALL;
        ok( hemp->error, "caught hemp error" );
        ok( hemp_string_eq(hemp->error->message, "Invalid cheese specified: badger"),
            "got bad cheese message"
        );

    HEMP_END;

    /* hemp error messages can be localised, customised, etc */
    hemp->errmsg[HEMP_ERROR_INVALID] = "You silly arse! '%2$s' is not a valid %1$s";

    HEMP_TRY;
        hemp_throw(
            hemp,
            HEMP_ERROR_INVALID, "cheese", "badger"
        );

    HEMP_CATCH_ALL;
        ok( hemp->error, "caught hemp error again" );
        ok( hemp_string_eq(hemp->error->message, "You silly arse! 'badger' is not a valid cheese"),
            "got silly arse bad cheese message"
        );

    HEMP_END;

    hemp_free(hemp);
}


void thrower(hemp_errno type, char *msg) {
    // hemp_debug("throwing %d: %s\n", type, msg);
    longjmp(state, type);
    hemp_debug("not reachhed\n");
}


void test_error_finally() {
    hemp_hemp hemp = hemp_new();
    int n = 0;

    HEMP_TRY
        HEMP_THROW(HEMP_ERROR_INVALID);
        fail("returned from thrower (this should not happen)");
    HEMP_CATCH(HEMP_ERROR_INVALID);
        pass("caught HEMP_ERROR_INVALID");
    HEMP_FINALLY;
        n++;
    HEMP_END;
    
    ok( n == 1, "finally block was run" );

    HEMP_TRY
        pass("not throwing an error");
    HEMP_CATCH_ALL;
        fail("caught hemp error");
    HEMP_FINALLY;
        n++;
    HEMP_END;

    ok( n == 2, "finally block was run again" );

    hemp_free(hemp);
}
