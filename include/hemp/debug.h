#ifndef HEMP_DEBUG_H
#define HEMP_DEBUG_H

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include <hemp/types.h>

void hemp_debug(char *format, ...);
void hemp_debug_col(char *col, char *format, ...);
void hemp_debug_on();
void hemp_debug_off();
void hemp_debug_token(hemp_cstr_p type, hemp_cstr_p str, hemp_pos_t len);

#define HEMP_DEBUG_INIT      0x0002
#define HEMP_DEBUG_MEM       0x0004
#define HEMP_DEBUG_CALL      0x0008
#define HEMP_DEBUG_FILE      0x0010
#define HEMP_DEBUG_SCAN      0x0020
#define HEMP_DEBUG_PARSE     0x0040
#define HEMP_DEBUG_PTREE     0x0100
#define HEMP_DEBUG_ALL       0xFFFF

#define HEMP_ANSI_RED        "\e[31m"
#define HEMP_ANSI_GREEN      "\e[32m"
#define HEMP_ANSI_YELLOW     "\e[33m"
#define HEMP_ANSI_BLUE       "\e[34m"
#define HEMP_ANSI_MAGENTA    "\e[35m"
#define HEMP_ANSI_CYAN       "\e[36m"
#define HEMP_ANSI_RESET      "\e[0m"

#ifndef HEMP_DEBUG
#  define HEMP_DEBUG 0
#elif (HEMP_DEBUG == 1)
#  undef  HEMP_DEBUG
#  define HEMP_DEBUG HEMP_DEBUG_ALL
#endif

#if HEMP_DEBUG
#  define hemp_assert                       assert
#  define hemp_debug_red(format, ...)       hemp_debug_col(HEMP_ANSI_RED,     format, ##__VA_ARGS__)
#  define hemp_debug_green(format, ...)     hemp_debug_col(HEMP_ANSI_GREEN,   format, ##__VA_ARGS__)
#  define hemp_debug_yellow(format, ...)    hemp_debug_col(HEMP_ANSI_YELLOW,  format, ##__VA_ARGS__)
#  define hemp_debug_blue(format, ...)      hemp_debug_col(HEMP_ANSI_BLUE,    format, ##__VA_ARGS__)
#  define hemp_debug_magenta(format, ...)   hemp_debug_col(HEMP_ANSI_MAGENTA, format, ##__VA_ARGS__)
#  define hemp_debug_cyan(format, ...)      hemp_debug_col(HEMP_ANSI_CYAN,    format, ##__VA_ARGS__)
#
#  if HEMP_DEBUG & HEMP_DEBUG_INIT
#    define hemp_debug_init(format, ...)    hemp_debug_col(HEMP_ANSI_YELLOW, format, ##__VA_ARGS__)
#  else 
#    define hemp_debug_init(format, ...)
#  endif
#
#  if HEMP_DEBUG & HEMP_DEBUG_MEM
#    define hemp_debug_mem(format, ...)     hemp_debug_col(HEMP_ANSI_CYAN, format, ##__VA_ARGS__)
#  else 
#    define hemp_debug_mem(format, ...)
#  endif
#
#  if HEMP_DEBUG & HEMP_DEBUG_CALL
#    define hemp_debug_call(format, ...)    hemp_debug_col(HEMP_ANSI_BLUE, format, ##__VA_ARGS__)
#  else 
#    define hemp_debug_call(format, ...)
#  endif
#
#  if HEMP_DEBUG & HEMP_DEBUG_FILE
#    define hemp_debug_file(format, ...)    hemp_debug_col(HEMP_ANSI_MAGENTA, format, ##__VA_ARGS__)
#  else 
#    define hemp_debug_file(format, ...)
#  endif
#
#  if HEMP_DEBUG & HEMP_DEBUG_SCAN
#    define hemp_debug_scan(format, ...)    hemp_debug_col(HEMP_ANSI_GREEN, format, ##__VA_ARGS__)
#  else 
#    define hemp_debug_scan(format, ...)
#  endif
#
#  if HEMP_DEBUG & HEMP_DEBUG_PARSE
#    define hemp_debug_parse(format, ...)   hemp_debug_col(HEMP_ANSI_YELLOW, format, ##__VA_ARGS__)
#  else 
#    define hemp_debug_parse(format, ...)
#  endif
#
#
#else
#  define hemp_assert
#  define hemp_debug_red(format, ...)
#  define hemp_debug_green(format, ...)
#  define hemp_debug_yellow(format, ...)
#  define hemp_debug_blue(format, ...)
#  define hemp_debug_magenta(format, ...)
#  define hemp_debug_cyan(format, ...)
#  define hemp_debug_init(format, ...)
#  define hemp_debug_mem(format, ...)
#  define hemp_debug_call(format, ...)
#  define hemp_debug_file(format, ...)
#  define hemp_debug_scan(format, ...)
#  define hemp_debug_parse(format, ...)
#endif

#endif /* HEMP_DEBUG_H */


