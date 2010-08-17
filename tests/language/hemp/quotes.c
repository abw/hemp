#include <hemp/test.h>


int main(
    int argc, char **argv, char **env
) {
    return hemp_test_expect_script(
        HEMP_TT3, HEMP_TT3, 
        HEMP_TESTDIR, "quotes"
    );
}
