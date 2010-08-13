#include <hemp.h>
#include <tap.h>

#define TEST_START    "-- start"
#define TEST_STOP     "-- stop"
#define TEST_MARKER   "-- test"
#define EXPECT_MARKER "-- expect"
#define ERROR_MARKER  "-- error"


void test_scanner();
void test_script(hemp_cstr_p);

int
main(
    int  argc, 
    char **argv, 
    char **env
) {
    if (argc > 1) {
        test_script(argv[1]);
    }
    else {
        plan_tests(180);
        test_scanner();
    }
    
    return exit_status();
}

void test_scanner() {
    test_script( "hello" );
    test_script( "comments" );
    test_script( "numbers" );
    test_script( "quotes" );
    test_script( "boolops" );
    test_script( "textops" );
    test_script( "numops" );
}

hemp_cstr_p find_marker(
    hemp_cstr_p src,
    hemp_cstr_p marker
) {
    hemp_cstr_p mark;

    if ((mark = strstr(src, marker))) {
        /* skip to the end of the line */
        hemp_cstr_to_next_line(&mark);
    }
    return mark;
}


void check_output(
    hemp_cstr_p name,
    hemp_text_p output,
    hemp_cstr_p expect
) {
    hemp_cstr_chomp(output->string);
    hemp_cstr_chomp(expect);

    if (hemp_cstr_eq(output->string, expect)) {
//      printf("EXPECT: [%s%s%s]\n", HEMP_ANSI_YELLOW, expect, HEMP_ANSI_RESET);
//      printf("OUTPUT: [%s%s%s]\n", HEMP_ANSI_GREEN, output->string, HEMP_ANSI_RESET);
        ok(1, "%s output matches expected", name);
    }
    else {
        printf("EXPECT: [%s%s%s]\n", HEMP_ANSI_YELLOW, expect, HEMP_ANSI_RESET);
        printf("OUTPUT: [%s%s%s]\n", HEMP_ANSI_RED, output->string, HEMP_ANSI_RESET);
        ok(0, "%s output does not match expected", name);
    }
}


void check_error(
    hemp_cstr_p name,
    hemp_cstr_p error,
    hemp_cstr_p expect
) {
    hemp_cstr_chomp(error);
    hemp_cstr_chomp(expect);

    if (hemp_cstr_eq(error, expect)) {
        ok(1, "%s error matches expected", name);
    }
    else {
        printf("EXPECT ERROR: [%s%s%s]\n", HEMP_ANSI_YELLOW, expect, HEMP_ANSI_RESET);
        printf("ACTUAL ERROR: [%s%s%s]\n", HEMP_ANSI_RED, error, HEMP_ANSI_RESET);
        ok(0, "%s error does not match expected", name);
    }
}


void test_script(
    hemp_cstr_p script
) {
    hemp_mem_trace_reset();
    hemp_p          hemp = hemp_init();
    hemp_cstr_p     dir  = hemp_filesystem_join_path(TESTDIR, "scripts");
    hemp_cstr_p     path = hemp_filesystem_join_path(dir, script);
    hemp_cstr_p     text = hemp_filesystem_read_file(path);
    hemp_cstr_p     test, name, expect, error, end;
    hemp_list_p     list;
    hemp_template_p tmpl;
    hemp_text_p     output;
    hemp_size_t     n;

    HEMP_TRY;
    hemp_language(hemp, "tt3");

    if (! text) {
        fail("could not read test script: %s", path);
        return;
    }

    if ((test = strstr(text, TEST_START))) {
        hemp_cstr_to_next_line(&test);
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
    hemp_debug("found %d tests in %s\n", list->length, script);

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
            hemp_cstr_to_next_line(&expect);
        }

        if ( (error = strstr(test, ERROR_MARKER))
          || (expect && (error = strstr(expect, ERROR_MARKER )))) {
            *error = '\0';
            error += strlen(ERROR_MARKER);
            hemp_cstr_to_next_line(&error);
        }

        hemp_cstr_chomp(test);

//        printf(">> test %u: %s\n", n, name);
//        if (expect)
//            printf(">> expect [%s]\n", expect);
//        if (error)
//            printf(">> error [%s]\n", error);

        HEMP_TRY;

            tmpl = hemp_template(
                hemp,
                HEMP_TT3,
                HEMP_TEXT, 
                test
            );
            output = hemp_template_render(tmpl);
            ok( output, "%s rendered", name);

            if (expect)
                check_output(name, output, expect);
            else if (error)
                fail("expected error but got output");

        HEMP_CATCH_ALL;
            output = hemp_error_text(hemp->error);

            if (error)
                check_error(name, output->string, error);
            else
                fail("error: %s", output->string);

        HEMP_END;

        hemp_text_free(output);
    }

    HEMP_CATCH_ALL;
        hemp_debug("CATCH-ALL: %s\n", hemp->error->message);
    HEMP_END;

    hemp_mem_free(dir);
    hemp_mem_free(path);
    hemp_mem_free(text);
    
    hemp_list_free(list);
    hemp_free(hemp);

    hemp_mem_trace_ok();
}

    

