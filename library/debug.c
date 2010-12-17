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


void hemp_debug_col(
    hemp_string file, 
    hemp_pos    line,
    hemp_string colour,
    hemp_string format, 
    ...
) {
#ifdef HEMP_DEBUG
    if (hemp_debugging) {
        /* attempt to shorten long file paths by skipping forward over
         * the compile time source directory which is encoded in __FILE__
         */
        hemp_char first = *file;
        hemp_size len   = strlen(HEMP_SOURCE_DIR);

        if (hemp_stringn_eq(file, HEMP_SOURCE_DIR, len))
            file += len + 1;

        len = strlen(file);
        if (len > 19) {
            file += len - 19;
            first = '~';
        }

        va_list args;
        va_start(args, format);
        fprintf(
            stderr, "%s%6ld %s%c%-20s %s", 
            HEMP_DEBUG_LINE_COL, line,
            HEMP_DEBUG_FILE_COL, first, file,
            colour
        );
        vfprintf(stderr, format, args);
        fprintf(stderr, HEMP_ANSI_RESET);
        va_end(args);
    }
#endif
}


void 
hemp_debug_token(
    hemp_string type,
    hemp_string str,
    hemp_pos    len
) {
    static char buffer[80];
    if (len > 79)
        len = 79;
    strncpy(buffer, str, len);
    buffer[len] = '\0';
    hemp_debug_scan("[%s:%s]\n", type, buffer);
} 

