#include <stdio.h>
#include "hemp.h"
#include "tap.h"


void test_tests();


int
main(int argc, char **argv, char **env)
{
    plan_tests(5);
//  plan_skip_all("YAYAY");
    test_tests();
    return exit_status();
}


void test_tests() {
    pass("This passed");
    fail("This failed");
    skip_start(1, 2, "Skipping 2 tests for the hell of it");
    pass("This would have passed (but was skipped)");
    fail("This would have failed (but was skipped)");
    skip_end;
}

