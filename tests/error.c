#include <setjmp.h>
#include "hemp.h"
#include "hemp/error.h"
#include "tap.h"

jmp_buf state;


void test_error1();
void test_error2();
void test_error3();
void thrower(hemp_error_t type, char *msg);


int
main(int argc, char **argv, char **env) {
    plan_tests(3);
    test_error1();
    test_error2();
    test_error3();
    return exit_status();
}


void test_error1() {
    int exception;
    
    if((exception = setjmp(state)) == 0) {              // try
        thrower(HEMP_ERROR_ONE, "hello world");
        debug_red("returned from thrower (this should not happen)\n");
    }
    else {
        switch (exception) {             
            case HEMP_ERROR_ONE:
                pass("caught HEMP_ERROR_ONE");
                break;
            case HEMP_ERROR_TWO:
                fail("caught HEMP_ERROR_TWO");
                break;
            default:
                fail("caught unknown exception: %d", exception);
        }
    }
}

void test_error2() {
    /* tmp hack - global error buffer */
    _HEMP_TRY
        _HEMP_THROW(HEMP_ERROR_TWO);
        fail("returned from thrower (this should not happen)");

    _HEMP_CATCH(HEMP_ERROR_ONE)
        fail("caught HEMP_ERROR_ONE");

    _HEMP_CATCH(HEMP_ERROR_TWO)
        pass("caught HEMP_ERROR_TWO");

    _HEMP_END
}


void test_error3() {
    hemp_t hemp = hemp_init();

    /* better version with error stack local to hemp interpreter, 
     * but depends on 'hemp' local symbol being visible
     */
    HEMP_TRY
        HEMP_THROW(HEMP_ERROR_THREE);
        fail("returned from thrower (this should not happen)");

    HEMP_CATCH(HEMP_ERROR_ONE)
        fail("caught HEMP_ERROR_ONE");

    HEMP_CATCH(HEMP_ERROR_TWO)
        fail("caught HEMP_ERROR_TWO");

    HEMP_CATCH(HEMP_ERROR_THREE)
        pass("caught HEMP_ERROR_THREE");

    HEMP_END;
}


void thrower(hemp_error_t type, char *msg) {
    // debug("throwing %d: %s\n", type, msg);
    longjmp(state, type);
    debug("not reachhed\n");
}
