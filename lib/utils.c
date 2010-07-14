#include "hemp/utils.h"


void hemp_todo(char *format, ...) {
    va_list args;
    va_start(args, format);
    fprintf(stderr, "%sTODO: ", ANSI_RED);
    vfprintf(stderr, format, args);
    fprintf(stderr, "%s\n", ANSI_RESET);
    va_end(args);
}


void hemp_fatal(char *format, ...) {
    va_list args;
    va_start(args, format);
    fprintf(stderr, "%shemp fatal error: %s", ANSI_RED, ANSI_YELLOW);
    vfprintf(stderr, format, args);
    fprintf(stderr, "%s\n", ANSI_RESET);
    va_end(args);
    exit(1);
}


