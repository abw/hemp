#include <hemp/test.h>


int main(
    int     argc, 
    char  **argv, 
    char  **env
) {
    Hemp       hemp    = hemp_new();
    HempContext    context = hemp_context(hemp);
    int             result;

    hemp_context_set_number(context, "a", 4);
    hemp_context_set_number(context, "b", 2);
    hemp_context_set_number(context, "c", 0);

    result = hemp_test_expect_script(
        HEMP_TT3, HEMP_TT3, 
        HEMP_TEST_DIR, "hash", 
        context
    );

    hemp_context_free(context);
    hemp_free(hemp);
    hemp_mem_trace_ok();

    return result;
}
