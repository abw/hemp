#include "hemp/debug.h"

#ifdef DEBUG
int debugging = 1;
#else
int debugging = 0;
#endif


void debug_on() {
#ifdef DEBUG
    debugging = 1;
#endif
}


void debug_off() {
#ifdef DEBUG
    debugging = 0;
#endif
}


void debug(char *format, ...)
{
#ifdef DEBUG
    if (debugging) {
        va_list args;
        va_start(args, format);
        vfprintf(stderr, format, args);
        va_end(args);
    }
#endif
}


void debug_col(char *col, char *format, ...) 
{
#ifdef DEBUG
    if (debugging) {
        va_list args;
        va_start(args, format);
        fprintf(stderr, "%s", col);
        vfprintf(stderr, format, args);
        fprintf(stderr, ANSI_RESET);
        va_end(args);
    }
#endif
}

void 
debug_token(
    hemp_cstr_t type,
    hemp_cstr_t str,
    hemp_pos_t  len
) {
    static char buffer[80];
    if (len > 79)
        len = 79;
    strncpy(buffer, str, len);
    buffer[len] = '\0';
    debug_scan("[%s:%s]\n", type, buffer);
} 


