#include <stdio.h>
#include "hemp.h"
#include "hemp/debug.h"
#include "tap.h"

void test_debug();


int
main(int argc, char **argv, char **env)
{
    plan_tests(1);
    test_debug();
    return exit_status();
}


void test_debug() {
    debug_red("This is red\n");
    debug_green("This is green\n");
    debug_yellow("This is yellow\n");
    debug_blue("This is blue\n");
    debug_magenta("This is magenta\n");
    debug_cyan("This is cyan\n");
    debug("Turning debugging off for a moment\n");
    debug_off();
    debug("!!! FAIL !!! - You shouldn't see this message\n");
    debug_on();
    debug("Turned debugging back on\n");
    debug_mem("Memory debugging\n");
    debug_load("Module loading debugging\n");
    pass("ran debugging tests");
}
