#include <stdio.h>
#include "hemp.h"
#include "tap.h"

#define TEST_MARKER "-- test"


void test_scanner();
void test_script(hemp_cstr_t, hemp_cstr_t);


int
main(int argc, char **argv, char **env)
{
    plan_tests(10);
    test_scanner();
    return exit_status();
}

void test_scanner() {
    hemp_cstr_t scripts = hemp_filesystem_join_path(TESTDIR, "scripts");
    
    test_script( scripts, "numbers" );
    test_script( scripts, "quotes" );
    
    hemp_mem_free(scripts);
}


void test_script(
    hemp_cstr_t scripts,
    hemp_cstr_t script
) {
    hemp_cstr_t     path = hemp_filesystem_join_path(scripts, script);
    hemp_cstr_t     text = hemp_filesystem_read_file(path);
    hemp_cstr_t     test, name;
    hemp_list_t     list;
    hemp_template_t tmpl;
    hemp_pos_t      n;

    if (! text) {
        fail("could not read test script: %s", path);
        return;
    }
    
    test = strstr(text, TEST_MARKER);

    if (! test)
        fail("no tests found in %s", script);
    
    /* skip over first -- test */
    test += strlen(TEST_MARKER);
    list = hemp_cstr_split(test, "-- test");
    debug("found %d tests in %s\n", list->length, script);
    
    for (n = 0; n < list->length; n++) {
        test = hemp_list_item(list, n);
        
        /* skip over leading whitespace */
        while (isspace(*test)) {
            test++;
        }
        
        name = test;
        do {
            test++;
        }
        while (*test != LF && *test != CR);

        /* NUL terminate test name */
        *test = '\0';
        
        do {
            test++;
        }
        while (*test == LF || *test == CR);

        printf(">> test %d: %s\n", n, name);
        
        tmpl = hemp_template_init(
            HEMP_TEXT, 
            test,
            NULL
        );
        ok( tmpl , "created template" );
        ok( hemp_template_compile(tmpl), "compiled template" );
    }
        
    //printf("TESTS: %s\n", hemp_list_dump(list));
}

    

