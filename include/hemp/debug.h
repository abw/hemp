#ifndef HEMP_DEBUG_H
#define HEMP_DEBUG_H

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

void    debug       (char *format, ...);
void    debug_col   (char *col, char *format, ...);

#define ANSI_RED        "\e[31m"
#define ANSI_GREEN      "\e[32m"
#define ANSI_YELLOW     "\e[33m"
#define ANSI_BLUE       "\e[34m"
#define ANSI_MAGENTA    "\e[35m"
#define ANSI_CYAN       "\e[36m"
#define ANSI_RESET      "\e[0m"

#define debug_red(format, ...)      debug_col(ANSI_RED,     format, ##__VA_ARGS__)
#define debug_green(format, ...)    debug_col(ANSI_GREEN,   format, ##__VA_ARGS__)
#define debug_yellow(format, ...)   debug_col(ANSI_YELLOW,  format, ##__VA_ARGS__)
#define debug_blue(format, ...)     debug_col(ANSI_BLUE,    format, ##__VA_ARGS__)
#define debug_magenta(format, ...)  debug_col(ANSI_MAGENTA, format, ##__VA_ARGS__)
#define debug_cyan(format, ...)     debug_col(ANSI_CYAN,    format, ##__VA_ARGS__)

#endif /* HEMP_DEBUG_H */


