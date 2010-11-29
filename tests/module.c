#include <stdio.h>
#include "hemp.h"
#include "tap.h"


void test_module();


int
main(int argc, char **argv, char **env)
{
    plan_tests(2);
    test_module();
    return exit_status();
}


void test_module() {
    hemp_t hemp = hemp_new();
    ok( hemp, "created hemp object" );
    
    skip_start(1, 1, "module loading doesn't work yet");
    ok( 
        // The path is hard-coded for now... it was a struggle to get this
        // far!
        hemp_module_load(hemp, "/Users/abw/projects/hemp/modules/foo.dylib"),
        "loaded module"
    );
    skip_end;
}