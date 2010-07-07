#include "hemp/debug.h"


void debug(char *format, ...)
{
#ifdef DEBUG
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
#endif /* DEBUG */
}

void debug_col(char *col, char *format, ...) 
{
#ifdef DEBUG
   va_list args;
   va_start(args, format);
   fprintf(stderr, "%s", col);
   vfprintf(stderr, format, args);
   fprintf(stderr, ANSI_RESET);
   va_end(args);
#endif /* DEBUG */
}
