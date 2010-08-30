#include <hemp/test.h>

jmp_buf state;


void test_error1();
void test_error2();
void test_error3();
void test_error4();
void test_error_object();
void test_hemp_throw();
void thrower(hemp_errno_e, char *msg);


int main(
    int argc, char **argv, char **env
) {
    plan(10);

    test_error1();
    test_error2();
    test_error3();
    test_error_object();
    test_hemp_throw();

    return done();
}


void test_error1() {
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


void test_error2() {
    /* tmp hack - global error buffer */
    _HEMP_TRY
        _HEMP_THROW(HEMP_ERROR_INVALID);
        fail("returned from thrower (this should not happen)");

    _HEMP_CATCH(HEMP_ERROR_MISSING)
        fail("caught HEMP_ERROR_MISSING");

    _HEMP_CATCH(HEMP_ERROR_INVALID)
        pass("caught HEMP_ERROR_INVALID");

    _HEMP_END
}


void test_error3() {
    hemp_p hemp = hemp_init();

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
    hemp_p hemp = hemp_init();

    HEMP_THROW(HEMP_ERROR_INVALID);
    fail("after throw - this should not happen");

    hemp_free(hemp);
}


void test_error_object() {
    hemp_error_p error = hemp_error_init(
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
    hemp_p hemp = hemp_init();

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


void thrower(hemp_errno_e type, char *msg) {
    // hemp_debug("throwing %d: %s\n", type, msg);
    longjmp(state, type);
    hemp_debug("not reachhed\n");
}
