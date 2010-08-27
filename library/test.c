#include <hemp/test.h>

/*--------------------------------------------------------------------------
 * This file defines functions for testing hemp using the Test Anything 
 * Prototcol (TAP).  It is based on the C TAP library by Nik Clayton:
 *--------------------------------------------------------------------------
 * Copyright (c) 2004 Nik Clayton
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *--------------------------------------------------------------------------
 * Modifications for hemp include:
 * - renaming some of the functions to bring them in line with other hemp_*
 * - additional testing functions for hemp (e.g. hemp_test_expect())
 * - colour output
 *--------------------------------------------------------------------------*/

#include <hemp/test.h>


hemp_test_plan_p hemp_global_test_plan = NULL;


/*--------------------------------------------------------------------------
 * Global initialisation and cleanup
 *--------------------------------------------------------------------------*/

void
hemp_test_global_setup() {
    static int run_once = 0;

    HEMP_MUTEX_LOCK;

    if (! run_once) {
        atexit(hemp_test_global_cleanup);
        setbuf(stdout, 0);
        run_once = 1;
    }

    HEMP_MUTEX_UNLOCK;
}


void
hemp_test_global_cleanup() {
    HEMP_MUTEX_LOCK;

    if (hemp_global_test_plan) {
        hemp_test_plan_cleanup(hemp_global_test_plan);
        hemp_global_test_plan = NULL;
    }

    HEMP_MUTEX_UNLOCK;
}


hemp_test_plan_p 
hemp_test_global_plan(
    hemp_uint_t planned
) {
    HEMP_MUTEX_LOCK;

    if (hemp_global_test_plan)
        hemp_fatal("A global test plan is already defined");

    hemp_global_test_plan = hemp_test_plan_setup(planned);

    HEMP_MUTEX_UNLOCK;
    
    return hemp_global_test_plan;
}



/*--------------------------------------------------------------------------
 * Test plan initialisation and cleanup
 *--------------------------------------------------------------------------*/

hemp_test_plan_p 
hemp_test_plan_setup(
    hemp_uint_t planned
) {
    hemp_test_global_setup();

    hemp_test_plan_p plan = (hemp_test_plan_p) hemp_mem_alloc(
        sizeof(struct hemp_test_plan_s)
    );
    
    if (! plan)
        hemp_mem_fail("test plan");
    
    plan->planned  = planned;
    plan->tested   =
    plan->failed   =
    plan->skipped  =
    plan->verbose  =
    plan->todo     = 0;
    plan->todo_msg = NULL;

    printf("1..%lu\n", plan->planned);

    return plan;
}


void 
hemp_test_plan_summary(
    hemp_test_plan_p plan
) {
    if (plan->planned < plan->tested) {
        hemp_test_warning(
            "Looks like you planned %d tests but ran %d extra.",
            plan->planned, plan->tested - plan->planned
        );
        return;
    }

    if (plan->planned > plan->tested) {
        hemp_test_warning(
            "Looks like you planned %d tests but only ran %d.",
            plan->planned, plan->tested
        );
        return;
    }

    if (plan->failed) {
        hemp_test_warning(
            "Looks like you failed %d tests of %d.", 
            plan->failed, plan->tested
        );
    }
}



hemp_uint_t
hemp_test_plan_result(
    hemp_test_plan_p plan
) {
    hemp_uint_t r;

    /* Ran too many tests?  Return the number of tests that were run
       that shouldn't have been */
    if (plan->planned < plan->tested) {
        r = plan->tested - plan->planned;
        return r;
    }

    /* Return the number of tests that failed + the number of tests 
       that weren't run */
    r = plan->failed + plan->planned - plan->tested;

    return r;
}


void 
hemp_test_plan_cleanup(
    hemp_test_plan_p plan
) {
    hemp_mem_free(plan);

    if (plan == hemp_global_test_plan)
        hemp_global_test_plan = NULL;
}


/*--------------------------------------------------------------------------
 * hemp_test_result(plan, ok, function, file, line, name, ...)
 *
 * Generate a test result.
 *--------------------------------------------------------------------------*/

hemp_uint_t
hemp_test_result(
    hemp_test_plan_p    plan,
    hemp_bool_t         ok,         /* true=pass false=fail */
    hemp_name_p         func,        
    hemp_name_p         file,       /* source file          */
    hemp_uint_t         line,       /* line number          */
    hemp_name_p         name,       /* test name            */
    ...
) {
    va_list ap;
    hemp_cstr_p fullname = NULL;
    hemp_bool_t badname  = HEMP_TRUE;   /* assume the worst */
    char *c;
    int name_is_digits;

    HEMP_MUTEX_LOCK;
    
    if (! plan)
        plan = hemp_global_test_plan;

    if (! plan)
        hemp_fatal("No test plan");
        
    
    plan->tested++;

    /* Start by taking the test name and performing any printf()
       expansions on it */
    if (name) {
        va_start(ap, name);
        vasprintf(&fullname, name, ap);
        va_end(ap);

        /* Make sure the test name contains more than digits
           and spaces.  Emit an error message and exit if it
           does */
        if (! fullname)
            hemp_mem_fail("test name");
        
        for(c = fullname; *c != HEMP_NUL; c++) {
            if(!isdigit(*c) && !isspace(*c)) {
                badname = HEMP_FALSE;
                break;
            }
        }

        if (badname) {
            hemp_fatal(
                "    You named your test '%s'.  You shouldn't use numbers for test names.", 
                fullname
            );
        }
    }

    if (ok) {
        printf(
            "%sok %lu", 
            HEMP_COL_PASS, 
            plan->tested
        );
        plan->passed++;
    }
    else {
        printf(
            "%snot ok %lu",
            plan->todo ? HEMP_COL_SKIP : HEMP_COL_FAIL, 
            plan->tested
        );
        plan->failed++;
    }

    if (name) {
        /* Print the test name, escaping any '#' characters */
        printf(" - ");
        flockfile(stdout);
            
        for(c = fullname; *c != HEMP_NUL; c++) {
            if (*c == '#')
                fputc('\\', stdout);
            fputc((int) *c, stdout);
        }
        funlockfile(stdout);
    }

    if (plan->todo) {
        printf(" # TODO %s", plan->todo_msg);

        /* TODO items aren't considered failures, so decrement the counter */
        if (! ok)
            plan->failed--;
    }

    printf("%s\n", HEMP_COL_TERM);

    if (! ok && plan->verbose)
        hemp_debug_cyan(
            "    Failed %stest (%s:%s() at line %d)", 
            plan->todo ? "(TODO) " : "", file, func, line
        );

    if (fullname)
        free(fullname);

    if (plan->todo) {
        if (--plan->todo == 0)
            free(plan->todo_msg);
    }


    HEMP_MUTEX_UNLOCK;

    /* We only care (when testing) that ok is positive, but here we
       specifically only want to return 1 or 0 */
    return ok ? 1 : 0;
}


/*--------------------------------------------------------------------------
 * 
 *--------------------------------------------------------------------------*/

hemp_test_plan_p
hemp_test_plan(
    hemp_uint_t planned
) {
    return hemp_test_global_plan(planned);
}


int
hemp_test_done(
    hemp_test_plan_p plan
) {
    if (! plan)
        plan = hemp_global_test_plan;

    if (! plan)
        hemp_fatal("No test plan defined");

    int r = hemp_test_plan_result(plan);
    hemp_test_plan_summary(plan);
    hemp_test_plan_cleanup(plan);

    return r;
}



void
hemp_test_warning(
    char *fmt, ...
) {
    va_list ap;
    fprintf(stderr, "%s# ", HEMP_COL_DIAG);
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);
    fprintf(stderr, "%s\n", HEMP_COL_TERM);
}


void
hemp_mem_trace_ok(void)
{
    char *debug = getenv("HEMP_MEMORY_TRACE");
    hemp_size_t size = hemp_mem_trace_report(
        debug && hemp_cstr_eq(debug, "1")
    );

    if (size == 0) {
        printf("%sall memory freed%s\n", HEMP_COL_PASS, HEMP_COL_TERM);
    }
    else if (size == -1) {
        printf("%smemory checks disabled%s\n", HEMP_COL_SKIP, HEMP_COL_TERM);
    }
    else {
        printf("%smemory unfreed: %lu bytes%s\n", HEMP_COL_FAIL, size, HEMP_COL_TERM);
        hemp_mem_trace_report(HEMP_TRUE);
        hemp_fatal("Fix the memory leaks!");
    }
}


/*--------------------------------------------------------------------------
 * hemp_test_expect_text(language, dialect, text)
 *
 * e.g. hemp_test_expect_text("tt3", "tt3", "...test script text...")
 *--------------------------------------------------------------------------*/

hemp_uint_t
hemp_test_expect_text(
    hemp_cstr_p     language,
    hemp_cstr_p     dialect,
    hemp_cstr_p     text,
    hemp_cstr_p     alias,
    hemp_context_p  context
) {
    hemp_p          hemp = hemp_init();
    hemp_cstr_p     test, name, expect, error, end;
    hemp_list_p     list;
    hemp_template_p tmpl;
    hemp_text_p     output;
    hemp_size_t     n;

    HEMP_TRY;
        hemp_language(hemp, language);
    HEMP_CATCH_ALL;
        hemp_fatal("Failed to load hemp language '%s': %s", language, hemp->error->message);
    HEMP_END;

    if ((test = strstr(text, HEMP_TEST_START))) {
        hemp_cstr_to_next_line(&test);
    }
    else {
        test = text;
    }
        
    if ((end = strstr(text, HEMP_TEST_STOP)))
        *end = HEMP_NUL;

    test = strstr(test, HEMP_TEST_MARKER);

    if (! test)
        hemp_fatal("no tests found in %s", alias);
    
    /* skip over first -- test */
    test += strlen(HEMP_TEST_MARKER);
    list = hemp_cstr_split(test, HEMP_TEST_MARKER);

    hemp_debug("found %d tests in %s\n", list->length, alias);

    hemp_test_plan(list->length * 2);

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
        
        if ((expect = strstr(test, HEMP_EXPECT_MARKER))) {
            *expect = '\0';
            expect += strlen(HEMP_EXPECT_MARKER);
            hemp_cstr_to_next_line(&expect);
        }

        if ( (error = strstr(test, HEMP_ERROR_MARKER))
          || (expect && (error = strstr(expect, HEMP_ERROR_MARKER )))) {
            *error = '\0';
            error += strlen(HEMP_ERROR_MARKER);
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
            output = hemp_template_render(tmpl, context);
            ok( output, "%s rendered", name);

            if (expect)
                hemp_test_output(name, output, expect);
            else if (error)
                fail("expected error but got output");

        HEMP_CATCH_ALL;
            output = hemp_error_text(hemp->error);

            if (error) {
                pass("%s raised error", name);
                hemp_test_error(name, output->string, error);
            }
            else {
                fail("%s raised error", name);
                fail("error: %s", output->string);
            }
        HEMP_END;

        hemp_text_free(output);
    }

    hemp_list_free(list);
    hemp_free(hemp);

    return hemp_test_done(NULL);
}


/*--------------------------------------------------------------------------
 * hemp_test_expect_file(language, dialect, file)
 *
 * e.g. hemp_test_expect_text("tt3", "tt3", "/path/to/test/script")
 *--------------------------------------------------------------------------*/

hemp_uint_t
hemp_test_expect_file(
    hemp_cstr_p     language,
    hemp_cstr_p     dialect,
    hemp_cstr_p     file,
    hemp_context_p  context
) {
    hemp_cstr_p text   = hemp_filesystem_read_file(file);
    hemp_uint_t result = hemp_test_expect_text(language, dialect, text, file, context);
    hemp_mem_free(text);
    return result;
}


/*--------------------------------------------------------------------------
 * hemp_test_expect_script(language, dialect, name)
 *
 * Looks for a script in the hemp distribution tests/scripts directory.
 * e.g. hemp_test_expect_text("tt3", "tt3", HEMP_TESTDIR, "numops")
 *--------------------------------------------------------------------------*/

hemp_uint_t
hemp_test_expect_script(
    hemp_cstr_p     language,
    hemp_cstr_p     dialect,
    hemp_cstr_p     testdir,
    hemp_cstr_p     name,
    hemp_context_p  context
) {
//    hemp_mem_trace_reset();
    hemp_cstr_p dir    = hemp_filesystem_join_path(testdir, "scripts");
    hemp_cstr_p file   = hemp_filesystem_join_path(dir, name);
    hemp_cstr_p text   = hemp_filesystem_read_file(file);
    hemp_uint_t result = hemp_test_expect_text(language, dialect, text, name, context);
    hemp_mem_free(text);
    hemp_mem_free(file);
    hemp_mem_free(dir);

    return result;
}


void 
hemp_test_output(
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


void 
hemp_test_error(
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


