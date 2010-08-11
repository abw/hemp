#include <hemp.h>
#include <tap.h>

#define TEST_START    "-- start"
#define TEST_STOP     "-- stop"
#define TEST_MARKER   "-- test"
#define EXPECT_MARKER "-- expect"


void test_scanner();
void test_script(hemp_cstr_p);

int
main(
    int  argc, 
    char **argv, 
    char **env
) {
    plan_tests(86);
    test_scanner();
    return exit_status();
}

void test_scanner() {
    test_script( "hello" );
    test_script( "comments" );
    test_script( "numbers" );
    test_script( "quotes" );
    test_script( "numops" );
    test_script( "boolops" );
}


void test_script(
    hemp_cstr_p script
) {
    hemp_mem_trace_reset();
    hemp_p          hemp = hemp_init();
    hemp_cstr_p     dir  = hemp_filesystem_join_path(TESTDIR, "scripts");
    hemp_cstr_p     path = hemp_filesystem_join_path(dir, script);
    hemp_cstr_p     text = hemp_filesystem_read_file(path);
    hemp_cstr_p     test, name, expect, end;
    hemp_list_p     list;
    hemp_template_p tmpl;
    hemp_size_t     n;

    HEMP_TRY;
    hemp_language(hemp, "tt3");

    if (! text) {
        fail("could not read test script: %s", path);
        return;
    }

    if ((test = strstr(text, TEST_START))) {
        test += strlen(TEST_START);
    }
    else {
        test = text;
    }
        
    if ((end = strstr(text, TEST_STOP)))
        *end = HEMP_NUL;

    test = strstr(test, TEST_MARKER);

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
        while   (*test != HEMP_LF && *test != HEMP_CR);

        /* NUL terminate test name */
        *test = '\0';
        
        do      { test++; }
        while   (*test == HEMP_LF || *test == HEMP_CR);
        
        if ((expect = strstr(test, EXPECT_MARKER))) {
            *expect = '\0';
            expect += strlen(EXPECT_MARKER);
            while   (*expect != HEMP_LF && *expect != HEMP_CR)
                    { expect++; }
            do      { expect++; }
            while   (*expect == HEMP_LF || *expect == HEMP_CR);
//            expect++;
        }

        hemp_cstr_chomp(test);

//      printf(">> test %d: %s\n", n, name);

//        if (expect)
//            printf(">> expect [%s]\n", expect);

        HEMP_TRY;
            tmpl = hemp_template(
                hemp,
                HEMP_TT3,
                HEMP_TEXT, 
                test
            );
        HEMP_CATCH_ALL;
            hemp_fatal("error: %s", hemp->error->message);
        HEMP_END;

//      printf("rendering...\n");
        hemp_text_p output = hemp_template_render(tmpl);
        ok( output, "%s rendered", name);

        if (expect) {
            hemp_cstr_chomp(output->string);
            hemp_cstr_chomp(expect);

            if (hemp_cstr_eq(output->string, expect)) {
//              printf("EXPECT: [%s%s%s]\n", ANSI_YELLOW, expect, ANSI_RESET);
//              printf("OUTPUT: [%s%s%s]\n", ANSI_GREEN, output->string, ANSI_RESET);
                ok(1, "%s output matches expected", name);
            }
            else {
                printf("EXPECT: [%s%s%s]\n", ANSI_YELLOW, expect, ANSI_RESET);
                printf("OUTPUT: [%s%s%s]\n", ANSI_RED, output->string, ANSI_RESET);
                ok(0, "%s output does not match expected", name);
            }
        }
        
        hemp_text_free(output);
    }

    HEMP_CATCH_ALL;
        debug("CATCH-ALL: %s\n", hemp->error->message);
    HEMP_END;

    hemp_mem_free(dir);
    hemp_mem_free(path);
    hemp_mem_free(text);
    
    hemp_list_free(list);
    hemp_free(hemp);

    hemp_mem_trace_ok();
}

    

