#include <hemp/test.h>


int main(
    int  argc, char **argv, char **env
) {
    char *name;
    
    if (argc > 1)
        name = argv[1];
    else
        name = "hello";

    return hemp_test_expect_script(
        HEMP_TT3, HEMP_TT3, 
        HEMP_TESTDIR, "name"
    );
}

