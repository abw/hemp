#ifndef HEMP_DEBUG_H
#define HEMP_DEBUG_H

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <hemp/types.h>

void debug       (char *format, ...);
void debug_col   (char *col, char *format, ...);
void debug_on();
void debug_off();
void debug_token(hemp_cstr_p type, hemp_cstr_p str, hemp_pos_t len);


#define DEBUG_MEM       0x0002
#define DEBUG_CALL      0x0004
#define DEBUG_LOAD      0x0008
#define DEBUG_FILE      0x0010
#define DEBUG_SCAN      0x0020
#define DEBUG_PARSE     0x0040
#define DEBUG_TYPE      0x0080
#define DEBUG_HUB       0x0100
#define DEBUG_ALL       0xFFFF

#define ANSI_RED        "\e[31m"
#define ANSI_GREEN      "\e[32m"
#define ANSI_YELLOW     "\e[33m"
#define ANSI_BLUE       "\e[34m"
#define ANSI_MAGENTA    "\e[35m"
#define ANSI_CYAN       "\e[36m"
#define ANSI_RESET      "\e[0m"

#ifndef DEBUG
#  define DEBUG 0
#elif (DEBUG == 1)
#  undef  DEBUG
#  define DEBUG DEBUG_ALL
#endif

#if DEBUG
#  define debug_red(format, ...)        debug_col(ANSI_RED,     format, ##__VA_ARGS__)
#  define debug_green(format, ...)      debug_col(ANSI_GREEN,   format, ##__VA_ARGS__)
#  define debug_yellow(format, ...)     debug_col(ANSI_YELLOW,  format, ##__VA_ARGS__)
#  define debug_blue(format, ...)       debug_col(ANSI_BLUE,    format, ##__VA_ARGS__)
#  define debug_magenta(format, ...)    debug_col(ANSI_MAGENTA, format, ##__VA_ARGS__)
#  define debug_cyan(format, ...)       debug_col(ANSI_CYAN,    format, ##__VA_ARGS__)
#
#  if DEBUG & DEBUG_MEM
#    define debug_mem(format, ...)      debug_col(ANSI_CYAN, format, ##__VA_ARGS__)
#  else 
#    define debug_mem(format, ...)
#  endif
#
#  if DEBUG & DEBUG_CALL
#    define debug_call(format, ...)     debug_col(ANSI_BLUE, format, ##__VA_ARGS__)
#  else 
#    define debug_call(format, ...)
#  endif
#
#  if DEBUG & DEBUG_LOAD
#    define debug_load(format, ...)     debug_col(ANSI_MAGENTA, format, ##__VA_ARGS__)
#  else 
#    define debug_load(format, ...)
#  endif
#
#  if DEBUG & DEBUG_FILE
#    define debug_file(format, ...)     debug_yellow(format, ##__VA_ARGS__)
#  else 
#    define debug_file(format, ...)
#  endif
#
#  if DEBUG & DEBUG_SCAN
#    define debug_scan(format, ...)     debug_col(ANSI_GREEN, format, ##__VA_ARGS__)
#  else 
#    define debug_scan(format, ...)
#  endif
#
#  if DEBUG & DEBUG_PARSE
#    define debug_parse(format, ...)    debug_col(ANSI_GREEN, format, ##__VA_ARGS__)
#  else 
#    define debug_parse(format, ...)
#  endif
#
#  if DEBUG & DEBUG_TYPE
#    define debug_type(format, ...)     debug_col(ANSI_MAGENTA, format, ##__VA_ARGS__)
#  else 
#    define debug_type(format, ...)
#  endif
#
#  if DEBUG & DEBUG_HUB
#    define debug_hub(format, ...)     debug_col(ANSI_CYAN, format, ##__VA_ARGS__)
#  else 
#    define debug_hub(format, ...)
#  endif
#
#
#else
#  define debug_red(format, ...)      
#  define debug_green(format, ...)    
#  define debug_yellow(format, ...)   
#  define debug_blue(format, ...)     
#  define debug_magenta(format, ...)  
#  define debug_cyan(format, ...)     
#  define debug_mem(format, ...)      
#  define debug_call(format, ...)
#  define debug_load(format, ...)
#  define debug_file(format, ...)
#  define debug_scan(format, ...)
#  define debug_parse(format, ...)
#  define debug_type(format, ...)
#  define debug_hub(format, ...)
#endif

#endif /* HEMP_DEBUG_H */


