#include <hemp.h>
#include <tap.h>

void test_hemp_debug();


int
main(
    int  argc, char **argv, char **env
) {
    plan_tests(1);
    test_hemp_debug();
    return exit_status();
}


void test_hemp_debug() {
    hemp_debug_red("This is red\n");
    hemp_debug_green("This is green\n");
    hemp_debug_yellow("This is yellow\n");
    hemp_debug_blue("This is blue\n");
    hemp_debug_magenta("This is magenta\n");
    hemp_debug_cyan("This is cyan\n");
    hemp_debug("Turning debugging off for a moment\n");
    hemp_debug_off();
    hemp_debug_red("!!! FAIL !!! - You shouldn't see this message\n");
    hemp_debug_on();
    hemp_debug("Turned debugging back on\n");
    hemp_debug_mem("Memory debugging\n");
    hemp_debug_load("Module loading debugging\n");
    pass("ran debugging tests");
}
