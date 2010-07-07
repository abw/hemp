#ifndef HEMP_TEST_H
#define HEMP_TEST_H

#include <stdio.h>
#include "hemp/debug.h"

#define PASS "PASS"
#define FAIL "FAIL"

void test_msg(
    char *col, 
    char *type, 
    char *format, 
    ...
);

#define pass(format, ...) test_msg(ANSI_GREEN, PASS, format, ##__VA_ARGS__)
#define fail(format, ...) debug_col(ANSI_RED,  FAIL, format, ##__VA_ARGS__)

#endif /* HEMP_TEST_H */


