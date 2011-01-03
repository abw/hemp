#ifndef HEMP_TEST_H
#define HEMP_TEST_H

#include <hemp.h>

/*--------------------------------------------------------------------------
 * This file defines macros and declares functions for testing hemp using 
 * the Test Anything Prototcol (TAP).  It is based on the C TAP library by 
 * Nik Clayton:
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
 * - defining a structure to hold test data
 * - additional testing functions for hemp (e.g. hemp_test_expect())
 * - colour output
 *--------------------------------------------------------------------------*/


struct hemp_test_plan {
//  HempUint        skip_all;
//  HempUint        skip_some;
    HempUint        planned;
//  HempUint        expected;
    HempUint        tested;
    HempUint        passed;
    HempUint        failed;
    HempUint        skipped;
    HempUint        verbose;
    HempUint        todo;
    HempString      todo_msg;
};

typedef struct hemp_test_plan   * HempTestPlan;

void
hemp_test_global_setup();

void
hemp_test_global_cleanup();

HempTestPlan
hemp_test_global_plan(
    HempUint        planned
);

HempTestPlan
hemp_test_plan_setup(
    HempUint        planned
);

void 
hemp_test_plan_summary(
    HempTestPlan    plan
);

HempUint
hemp_test_plan_result(
    HempTestPlan    plan
);

void    
hemp_test_plan_cleanup(
    HempTestPlan    plan
);

HempTestPlan 
hemp_test_plan(
    HempUint        planned
);

HempUint
hemp_test_result(
    HempTestPlan    plan,
    HempBool        ok,         /* true=pass false=fail */
    HempName        func,        
    HempName        file,       /* source file          */
    HempUint        line,       /* line number          */
    HempName        name,       /* test name            */
    ...
);

void 
hemp_test_output(
    HempString      name,
    HempText        output,
    HempString      expect
);

void 
hemp_test_error(
    HempString      name,
    HempString      error,
    HempString      expect
);

void
hemp_test_warning(
    HempString      format,
    ...
);

int
hemp_test_all_done(
    HempTestPlan    plan
);



void hemp_mem_trace_ok(void);

HempUint
hemp_test_expect_text(
    HempString      language,
    HempString      dialect,
    HempString      text,
    HempString      alias,
    HempContext     context
);

HempUint
hemp_test_expect_file(
    HempString      language,
    HempString      dialect,
    HempString      file,
    HempContext     context
);

HempUint
hemp_test_expect_script(
    HempString      language,
    HempString      dialect,
    HempString      testdir,
    HempString      name,
    HempContext     context
);


/*--------------------------------------------------------------------------
 * definitions and macros
 *--------------------------------------------------------------------------*/

// TODO: pass this in as an environment variable

#ifndef HEMP_TEST_COLOR
    #define HEMP_TEST_COLOR 1
#endif

#ifdef HEMP_TEST_COLOR
    #define HEMP_COL_PASS   HEMP_ANSI_GREEN
    #define HEMP_COL_FAIL   HEMP_ANSI_RED
    #define HEMP_COL_SKIP   HEMP_ANSI_YELLOW
    #define HEMP_COL_DIAG   HEMP_ANSI_CYAN
    #define HEMP_COL_TERM   HEMP_ANSI_RESET
#else
    #define HEMP_COL_PASS   ""
    #define HEMP_COL_FAIL   ""
    #define HEMP_COL_SKIP   ""
    #define HEMP_COL_DIAG   ""
    #define HEMP_COL_TERM   ""
#endif

#define HEMP_TEST_START    "-- start"
#define HEMP_TEST_STOP     "-- stop"
#define HEMP_TEST_MARKER   "-- test"
#define HEMP_EXPECT_MARKER "-- expect"
#define HEMP_ERROR_MARKER  "-- error"


#define FUNC_FILE_LINE  __func__, __FILE__, __LINE__

/* '## __VA_ARGS__' is a gcc'ism. C99 doesn't allow the token pasting
   and requires the caller to add the final comma if they've ommitted
   the optional arguments */

#ifdef __GNUC__
    #define ok(e, test, ...) (                                              \
      (e)                                                                   \
        ? hemp_test_result(NULL, 1, FUNC_FILE_LINE, test, ## __VA_ARGS__)   \
        : hemp_test_result(NULL, 0, FUNC_FILE_LINE, test, ## __VA_ARGS__)   \
    )

    #define ok1(e) (                                                        \
      (e)                                                                   \
        ? hemp_test_result(NULL, 1, FUNC_FILE_LINE, "%s", #e)               \
        : hemp_test_result(NULL, 0, FUNC_FILE_LINE, "%s", #e)               \
    )

    #define pass(test, ...) ok(1, test, ## __VA_ARGS__);
    #define fail(test, ...) ok(0, test, ## __VA_ARGS__);

#elif __STDC_VERSION__ >= 199901L
    #define ok(e, ...) (                                                    \
      (e)                                                                   \
        ? hemp_test_result(NULL, 1, FUNC_FILE_LINE, __VA_ARGS__)            \
        : hemp_test_result(NULL, 0, FUNC_FILE_LINE, __VA_ARGS__)            \
    )

    #define ok1(e) (                                                        \
        (e)                                                                 \
            ? hemp_test_result(NULL, 1, FUNC_FILE_LINE, "%s", #e)           \
            : hemp_test_result(NULL, 0, FUNC_FILE_LINE, "%s", #e)           \
    )

    #define pass(...) ok(1, __VA_ARGS__);
    #define fail(...) ok(0, __VA_ARGS__);

#else
    #error "Needs gcc or C99 compiler for variadic macros."

#endif

#define is(a, b, msg) (                                                     \
    (strcmp(a, b) == 0)                                                     \
        ? hemp_test_result(NULL, 1, FUNC_FILE_LINE, msg)                    \
        : hemp_test_result(NULL, 0, FUNC_FILE_LINE,                         \
            "%s\n%sEXPECT: [%s%s%s]\nOUTPUT: [%s%s%s]%s", msg,              \
            HEMP_ANSI_CYAN, HEMP_ANSI_YELLOW, b,                            \
            HEMP_ANSI_CYAN, HEMP_ANSI_RED, a, HEMP_ANSI_CYAN,               \
            HEMP_ANSI_RESET)                                                \
)

#define eq(a, b, msg) (                                                     \
    (a == b)                                                                \
        ? hemp_test_result(NULL, 1, FUNC_FILE_LINE, msg)                    \
        : hemp_test_result(NULL, 0, FUNC_FILE_LINE,                         \
            "%s\n%sEXPECT: [%s%g%s]\nOUTPUT: [%s%g%s]%s", msg,              \
            HEMP_ANSI_CYAN, HEMP_ANSI_YELLOW, (HempNum) b,               \
            HEMP_ANSI_CYAN, HEMP_ANSI_RED, (HempNum) a, HEMP_ANSI_CYAN,  \
            HEMP_ANSI_RESET)                                                \
)

#define plan(n)             \
    hemp_test_plan(n)

#define done()              \
    hemp_test_all_done(NULL)


#endif /* HEMP_TEST_H */