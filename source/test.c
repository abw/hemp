#include "hemp/test.h"


void test_msg(
    char *col, 
    char *type, 
    char *format, 
    ...
) {
    va_list args;
    va_start(args, format);
    fprintf(stderr, "%s%s - ", col, type);
    vfprintf(stderr, format, args);
    fprintf(stderr, "%s\n", ANSI_RESET);
    va_end(args);
}

