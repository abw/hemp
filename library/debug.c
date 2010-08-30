#include "hemp/core.h"

#ifdef HEMP_DEBUG
int hemp_debugging = 1;
#else
int hemp_debugging = 0;
#endif


void hemp_debug_on() {
#ifdef HEMP_DEBUG
    hemp_debugging = 1;
#endif
}


void hemp_debug_off() {
#ifdef HEMP_DEBUG
    hemp_debugging = 0;
#endif
}


void hemp_debug(char *format, ...)
{
#ifdef HEMP_DEBUG
    if (hemp_debugging) {
        va_list args;
        va_start(args, format);
        vfprintf(stderr, format, args);
        va_end(args);
    }
#endif
}


void hemp_debug_col(char *col, char *format, ...) 
{
#ifdef HEMP_DEBUG
    if (hemp_debugging) {
        va_list args;
        va_start(args, format);
        fprintf(stderr, "%s", col);
        vfprintf(stderr, format, args);
        fprintf(stderr, HEMP_ANSI_RESET);
        va_end(args);
    }
#endif
}

void 
hemp_debug_token(
    hemp_str_p type,
    hemp_str_p str,
    hemp_pos_t len
) {
    static char buffer[80];
    if (len > 79)
        len = 79;
    strncpy(buffer, str, len);
    buffer[len] = '\0';
    hemp_debug_scan("[%s:%s]\n", type, buffer);
} 

