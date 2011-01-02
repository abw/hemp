#include <hemp/test.h>


int main(
    int argc, char **argv, char **env
) {
    Hemp      hemp    = hemp_new();
    HempContext context = hemp_context_instance(hemp);
    int         result;

    result = hemp_test_expect_script(
        HEMP_TT3, HEMP_TT3, 
        HEMP_TEST_DIR, "functions",
        context
    );

    hemp_context_free(context);
    hemp_free(hemp);
    hemp_mem_trace_ok();

    return result;
}
