#ifndef HEMP_CORE_H
#define HEMP_CORE_H

/*--------------------------------------------------------------------------
/* This header file includes all the other core header files that are 
 * required by most, if not all of the library source files.
 *--------------------------------------------------------------------------*/
 
#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <setjmp.h>
#include <sys/types.h>

#include <hemp/config.h>            /* local system configuration           */
#include <hemp/os.h>                /* operating system specific items      */
#include <hemp/defines.h>           /* various data definitions             */
#include <hemp/types.h>             /* type definitions                     */
#include <hemp/debug.h>             /* debugging functions/definitions      */
#include <hemp/error.h>             /* error handling                       */
#include <hemp/memory.h>            /* memory managment                     */
#include <hemp/global.h>            /* global hemp configuration            */
#include <hemp/utils.h>             /* misc utils                           */
#include <hemp/macros.h>            /* macros sprinkling syntactic sugar    */
#include <hemp/type/string.h>       /* functions for handling C strings     */

#endif /* HEMP_CORE_H */
