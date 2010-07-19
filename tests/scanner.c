#include <stdio.h>
#include "hemp.h"
#include "tap.h"

#define TEST_MARKER "-- test"
#define EXPECT_MARKER "-- expect"


void test_scanner();
void test_script(hemp_cstr_t, hemp_cstr_t);


int
main(int argc, char **argv, char **env)
{
    plan_tests(20);
    test_scanner();
    return exit_status();
}

void test_scanner() {
    hemp_cstr_t scripts = hemp_filesystem_join_path(TESTDIR, "scripts");
    
//    test_script( scripts, "numbers" );
//    test_script( scripts, "quotes" );
    test_script( scripts, "comments" );
    
    hemp_mem_free(scripts);
}


void test_script(
    hemp_cstr_t scripts,
    hemp_cstr_t script
) {
    hemp_cstr_t     path = hemp_filesystem_join_path(scripts, script);
    hemp_cstr_t     text = hemp_filesystem_read_file(path);
    hemp_cstr_t     test, name, expect;
    hemp_list_t     list;
    hemp_template_t tmpl;
    hemp_size_t     n;

    if (! text) {
        fail("could not read test script: %s", path);
        return;
    }
    
    test = strstr(text, TEST_MARKER);

    if (! test)
        fail("no tests found in %s", script);
    
    /* skip over first -- test */
    test += strlen(TEST_MARKER);
    list = hemp_cstr_split(test, TEST_MARKER);
    debug("found %d tests in %s\n", list->length, script);
    
    for (n = 0; n < list->length; n++) {
        test = hemp_list_item(list, n);
        
        /* skip over leading whitespace */
        while (isspace(*test)) {
            test++;
        }
        
        name = test;
        do      { test++; }
        while   (*test != LF && *test != CR);

        /* NUL terminate test name */
        *test = '\0';
        
        do      { test++; }
        while   (*test == LF || *test == CR);
        
        if ((expect = strstr(test, EXPECT_MARKER))) {
            *expect = '\0';
            expect += strlen(EXPECT_MARKER);
            while   (*expect != LF && *expect != CR)
                    { expect++; }
            do      { expect++; }
            while   (*expect == LF || *expect == CR);
//            expect++;
        }

/*
        printf(">> test %d: %s\n", n, name);

        if (expect)
            printf(">> expect [%s]\n", expect);
*/
        
        tmpl = hemp_template_init(
            HEMP_TEXT, 
            test,
            NULL
        );
        hemp_text_t output = hemp_template_render(tmpl);
        ok( output, "%s rendered", name);
        debug_cyan("OUTPUT: [%s]\n", output->string);
        if (expect) {
            if (hemp_cstr_eq(output->string, expect)) {
                ok(1, "%s output matches expected", name);
            }
            else {
                ok(0, "%s output does not match expected", name);
            }
        }
                
    }
        
    //printf("TESTS: %s\n", hemp_list_dump(list));
}

    

