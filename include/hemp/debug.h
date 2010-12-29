#ifndef HEMP_DEBUG_H
#define HEMP_DEBUG_H

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include <hemp/types.h>

void hemp_debug(
    char *format, 
    ...
);
void hemp_debug_col(
    hemp_string file, 
    hemp_pos    line,
    hemp_string colour,
    hemp_string format, 
    ...
);
void hemp_debug_token(
    hemp_string type, 
    hemp_string str, 
    hemp_pos    len
);
void hemp_debug_on();
void hemp_debug_off();

#define HEMP_DEBUG_INIT         0x0002
#define HEMP_DEBUG_MEM          0x0004
#define HEMP_DEBUG_CALL         0x0008
#define HEMP_DEBUG_FILE         0x0010
#define HEMP_DEBUG_SCAN         0x0020
#define HEMP_DEBUG_PARSE        0x0040
#define HEMP_DEBUG_FACTORY      0x0080
#define HEMP_DEBUG_PTREE        0x0100
#define HEMP_DEBUG_ALL          0xFFFF

#define HEMP_ANSI_RED           "\e[31m"
#define HEMP_ANSI_GREEN         "\e[32m"
#define HEMP_ANSI_YELLOW        "\e[33m"
#define HEMP_ANSI_BLUE          "\e[34m"
#define HEMP_ANSI_MAGENTA       "\e[35m"
#define HEMP_ANSI_CYAN          "\e[36m"
#define HEMP_ANSI_WHITE         "\e[37m"
#define HEMP_ANSI_RESET         "\e[0m"
#define HEMP_ANSI_NONE          ""

#define HEMP_DEBUG_LINE_COL     HEMP_ANSI_CYAN
#define HEMP_DEBUG_FILE_COL     HEMP_ANSI_YELLOW

#ifndef HEMP_DEBUG
    #define HEMP_DEBUG 0
#elif (HEMP_DEBUG == 1)
    #undef  HEMP_DEBUG
    #define HEMP_DEBUG HEMP_DEBUG_ALL
#endif

#if HEMP_DEBUG
    #define hemp_assert                                                 \
            assert
    #define hemp_debug_at(...)                                          \
            hemp_debug_col(__FILE__, __LINE__, __VA_ARGS__)
    #define hemp_debug_msg(...)                                         \
            hemp_debug_at(HEMP_ANSI_RESET,   __VA_ARGS__)
    #define hemp_debug_red(...)                                         \
            hemp_debug_at(HEMP_ANSI_RED,     __VA_ARGS__)
    #define hemp_debug_green(...)                                       \
            hemp_debug_at(HEMP_ANSI_GREEN,   __VA_ARGS__)
    #define hemp_debug_yellow(...)                                      \
            hemp_debug_at(HEMP_ANSI_YELLOW,  __VA_ARGS__)
    #define hemp_debug_blue(...)                                        \
            hemp_debug_at(HEMP_ANSI_BLUE,    __VA_ARGS__)
    #define hemp_debug_magenta(...)                                     \
            hemp_debug_at(HEMP_ANSI_MAGENTA, __VA_ARGS__)
    #define hemp_debug_cyan(...)                                        \
            hemp_debug_at(HEMP_ANSI_CYAN,    __VA_ARGS__)

    #if HEMP_DEBUG & HEMP_DEBUG_INIT
        #define hemp_debug_init(...)    hemp_debug_yellow(__VA_ARGS__)
    #else 
        #define hemp_debug_init(format, ...)
    #endif

    #if HEMP_DEBUG & HEMP_DEBUG_MEM
        #define hemp_debug_mem(...)     hemp_debug_cyan(__VA_ARGS__)
    #else 
        #define hemp_debug_mem(format, ...)
    #endif

    #if HEMP_DEBUG & HEMP_DEBUG_CALL
        #define hemp_debug_call(...)    hemp_debug_blue(__VA_ARGS__)
    #else 
        #define hemp_debug_call(format, ...)
    #endif

    #if HEMP_DEBUG & HEMP_DEBUG_FILE
        #define hemp_debug_file(...)    hemp_debug_magenta(__VA_ARGS__)
    #else 
        #define hemp_debug_file(format, ...)
    #endif

    #if HEMP_DEBUG & HEMP_DEBUG_FACTORY
        #define hemp_debug_factory(...) hemp_debug_cyan(__VA_ARGS__)
    #else 
        #define hemp_debug_factory(format, ...)
    #endif

    #if HEMP_DEBUG & HEMP_DEBUG_SCAN
        #define hemp_debug_scan(...)    hemp_debug_green(__VA_ARGS__)
    #else 
        #define hemp_debug_scan(...)
    #endif

    #if HEMP_DEBUG & HEMP_DEBUG_PARSE
        #define hemp_debug_parse(...)   hemp_debug_yellow(__VA_ARGS__)
    #else 
        #define hemp_debug_parse(format, ...)
    #endif
#else
    #define hemp_assert             (void)
    #define hemp_debug_msg(...)
    #define hemp_debug_red(...)
    #define hemp_debug_green(...)
    #define hemp_debug_yellow(...)
    #define hemp_debug_blue(...)
    #define hemp_debug_magenta(...)
    #define hemp_debug_cyan(...)
    #define hemp_debug_init(...)
    #define hemp_debug_mem(...)
    #define hemp_debug_call(...)
    #define hemp_debug_file(...)
    #define hemp_debug_scan(...)
    #define hemp_debug_parse(...)
    #define hemp_debug_factory(...)
#endif

#endif /* HEMP_DEBUG_H */


