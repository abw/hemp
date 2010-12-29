#include <hemp/test.h>


int main(
    int argc, char **argv, char **env
) {
    int result = hemp_test_expect_script(
        HEMP_TT3, HEMP_TT3, 
        HEMP_TEST_DIR, "command/just",
        NULL
    );
    hemp_mem_trace_ok();
    return result;
}
