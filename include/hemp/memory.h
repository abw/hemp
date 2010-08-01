#ifndef HEMP_MEMORY_H
#define HEMP_MEMORY_H

#include <stdlib.h>
#include <string.h>
#include "hemp/types.h"
#include "hemp/debug.h"


#define HEMP_MEM_WILD     0    /* uninitialised (wild)                     */
#define HEMP_MEM_FAIL     1    /* malloc failed                            */
#define HEMP_MEM_MALLOC   2    /* malloc succeeded                         */
#define HEMP_MEM_FREE     3    /* memory free'd                            */

#define HEMP_MEM_BLKSIZ   8    /* # of mrec's to malloc at once            */
#define HEMP_MEM_PEEKLEN  32   /* # of bytes to peek into memory           */


#if  HAVE_memmove
#   define hemp_mem_copy_fn(src, dest, len) memmove(dest, src, len)
#elif HAVE_bcopy
#   define hemp_mem_copy_fn(src, dest, len) bcopy(src, dest, len)
#else
#   define HEMP_ADD_MEM_COPY 1
    hemp_ptr_t hemp_mem_copy_fn(
        hemp_mem_t  src,
        hemp_ptr_t  dest,
        hemp_size_t len
    );
#endif


/*--------------------------------------------------------------------------
 * memory debugging
 *--------------------------------------------------------------------------*/

#if DEBUG & DEBUG_MEM
    /* declare the extra data structure and functions for memory tracing */
    struct hemp_mem_trace {
        int          id;
        int          status;
        char        *file;
        unsigned int line;
        hemp_ptr_t   ptr;
        hemp_size_t  size;
    };

    typedef struct hemp_mem_trace *hemp_mem_trace_t;

    hemp_mem_trace_t hemp_mem_new_trace();
    hemp_mem_trace_t hemp_mem_get_trace(void *ptr);
    void hemp_mem_debug_report(hemp_bool_t verbose);
    void hemp_mem_debug_cleanup();

    void *hemp_mem_debug_malloc(size_t size, char *file, unsigned int line);
    void *hemp_mem_debug_realloc(void *ptr, size_t size, char *file, unsigned int line);
    char *hemp_mem_debug_copy(char *src, char *dest, size_t size, char *file, unsigned int line);
    char *hemp_mem_debug_strdup(char *ptr, char *file, unsigned int line);
    void hemp_mem_debug_free(void *ptr);

    #define hemp_mem_init(size)             hemp_mem_debug_malloc(size, __FILE__, __LINE__)
    #define hemp_mem_free(memory)           hemp_mem_debug_free(memory)
    #define hemp_mem_size(memory, size)     hemp_mem_debug_realloc(memory, size, __FILE__, __LINE__)
    #define hemp_mem_copy(src, dest, len)   hemp_mem_debug_copy(src, dest, len, __FILE__, __LINE__)
    #define strdup(src)                     hemp_mem_debug_strdup(src, __FILE__, __LINE__)
#else
    #define hemp_mem_init(size)             malloc(size)
    #define hemp_mem_free(memory)           free(memory)
    #define hemp_mem_size(memory, size)     realloc(memory, size)
    #define hemp_mem_copy(src, dest, size)  hemp_mem_copy_fn(src, dest, size)
#endif

#endif /* HEMP_MEMORY_H */
