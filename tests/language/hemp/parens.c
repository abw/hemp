#include <hemp/test.h>


int main(
    int argc, char **argv, char **env
) {
    hemp_hemp       hemp    = hemp_new();
    hemp_context    context = hemp_context_instance(hemp);
    int             result  = 0;

    hemp_context_set_number(context, "a", 2);
    hemp_context_set_number(context, "b", 4);

    HEMP_TRY;
        result = hemp_test_expect_script(
            HEMP_TT3, HEMP_TT3, 
            HEMP_TESTDIR, "parens", 
            context
            );
    HEMP_CATCH_ALL;
        printf("FAILED: %s\n", hemp_error_text(hemp->error)->string);
    HEMP_END;

    hemp_context_free(context);
    hemp_free(hemp);
    hemp_mem_trace_ok();

    return result;
}
