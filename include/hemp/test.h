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


struct hemp_test_plan_s {
    hemp_uint_t     skip_all;
    hemp_uint_t     skip_some;
    hemp_uint_t     planned;
    hemp_uint_t     expected;
    hemp_uint_t     tested;
    hemp_uint_t     passed;
    hemp_uint_t     failed;
    hemp_uint_t     skipped;
    hemp_uint_t     verbose;
    hemp_uint_t     todo;
    hemp_cstr_p     todo_msg;
};

typedef struct hemp_test_plan_s *hemp_test_plan_p;

void
    hemp_test_global_setup();

void
    hemp_test_global_cleanup();

hemp_test_plan_p 
    hemp_test_global_plan(
        hemp_uint_t planned
    );

hemp_test_plan_p 
    hemp_test_plan_setup(
        hemp_uint_t planned
    );

void 
    hemp_test_plan_summary(
        hemp_test_plan_p plan
    );

hemp_uint_t
    hemp_test_plan_result(
        hemp_test_plan_p plan
    );

void    
    hemp_test_plan_cleanup(
        hemp_test_plan_p plan
    );

hemp_test_plan_p 
    hemp_test_plan(
        hemp_uint_t planned
    );

hemp_uint_t
    hemp_test_result(
        hemp_test_plan_p    plan,
        hemp_bool_t         ok,         /* true=pass false=fail */
        hemp_name_p         func,        
        hemp_name_p         file,       /* source file          */
        hemp_uint_t         line,       /* line number          */
        hemp_name_p         name,       /* test name            */
        ...
    );

void 
    hemp_test_output(
        hemp_cstr_p name,
        hemp_text_p output,
        hemp_cstr_p expect
    );


void 
    hemp_test_error(
        hemp_cstr_p name,
        hemp_cstr_p error,
        hemp_cstr_p expect
    );

void
    hemp_test_warning(
        char *fmt, ...
    );



void hemp_mem_trace_ok(void);

hemp_uint_t
    hemp_test_expect_text(
        hemp_cstr_p language,
        hemp_cstr_p dialect,
        hemp_cstr_p text,
        hemp_cstr_p alias
    );

hemp_uint_t
    hemp_test_expect_file(
        hemp_cstr_p language,
        hemp_cstr_p dialect,
        hemp_cstr_p file
    );

hemp_uint_t
    hemp_test_expect_script(
        hemp_cstr_p language,
        hemp_cstr_p dialect,
        hemp_cstr_p testdir,
        hemp_cstr_p name
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
            HEMP_ANSI_CYAN, HEMP_ANSI_YELLOW, (hemp_num_t) b,               \
            HEMP_ANSI_CYAN, HEMP_ANSI_RED, (hemp_num_t) a, HEMP_ANSI_CYAN,  \
            HEMP_ANSI_RESET)                                                \
)

#define plan(n)             \
    hemp_test_plan(n)

#define done()              \
    hemp_test_done(NULL);   \
    hemp_mem_trace_ok();


#endif /* HEMP_TEST_H */