#ifndef HEMP_MEMORY_H
#define HEMP_MEMORY_H

#include <hemp/core.h>


/*--------------------------------------------------------------------------
 * Function prototypes - more below in "Memory Debugging"
 *--------------------------------------------------------------------------*/

void        hemp_mem_fail(HempString);
HempSize   hemp_mem_trace_report(HempBool);


/*--------------------------------------------------------------------------
 * Macro to patch in memmove(), bcopy() or a hand-rolled memory copy function
 *--------------------------------------------------------------------------*/

/* gcc 4.4 doesn't like empty values in #elif */
#ifdef  HEMP_HAVE_BCOPY
#undef  HEMP_HAVE_BCOPY
#define HEMP_HAVE_BCOPY 1
#endif

#ifdef  HEMP_HAVE_MEMMOVE
#   define hemp_mem_copy(src, dest, len) memmove(dest, src, len)
#elif   HEMP_HAVE_BCOPY
#   define hemp_mem_copy(src, dest, len) bcopy(src, dest, len)
#else
#   define HEMP_ADD_MEM_COPY 1
    HempMemory hemp_mem_copy(
        HempMemory src,
        HempMemory dest,
        HempSize   len
    );
#endif


/*--------------------------------------------------------------------------
 * memory debugging
 *--------------------------------------------------------------------------*/

#define HEMP_MEM_DASHES \
    "-----------------------------------------------------------------"

#if HEMP_DEBUG & HEMP_DEBUG_MEM
    #define HEMP_MEM_WILD     0    /* uninitialised (wild)                  */
    #define HEMP_MEM_FAIL     1    /* malloc failed                         */
    #define HEMP_MEM_MALLOC   2    /* malloc succeeded                      */
    #define HEMP_MEM_MOVED    3    /* realloc relocated                     */
    #define HEMP_MEM_EXTERNAL 4    /* memory allocated by external function */
    #define HEMP_MEM_FREE     5    /* memory free'd                         */

    #define HEMP_MEM_BLKSIZ   8    /* # of mrec's to malloc at once         */
    #define HEMP_MEM_PEEKLEN  32   /* # of bytes to peek into memory        */

    struct hemp_mem_trace {
        HempInt        id;
        HempInt        status;
        HempString     file;
        HempPos        line;
        HempMemory     ptr;
        HempSize       size;
    };
    typedef struct hemp_mem_trace *hemp_mem_trace;

    hemp_mem_trace  hemp_mem_new_trace();
    hemp_mem_trace  hemp_mem_get_trace(HempMemory, HempString, HempPos);
    void            hemp_mem_trace_reset();

    HempMemory     hemp_mem_trace_malloc(HempSize, HempString, HempPos);
    HempMemory     hemp_mem_trace_realloc(HempMemory, HempSize, HempString, HempPos);
    HempString     hemp_mem_trace_strdup(HempString, HempString, HempPos);
    HempMemory     hemp_mem_trace_external(HempMemory, HempSize, HempString, HempPos);

    void            hemp_mem_trace_free(HempMemory, HempString, HempPos);

    #define hemp_mem_alloc(size)            hemp_mem_trace_malloc(size, __FILE__, __LINE__)
    #define hemp_mem_resize(memory, size)   hemp_mem_trace_realloc(memory, size, __FILE__, __LINE__)
    #define hemp_mem_free(memory)           hemp_mem_trace_free(memory, __FILE__, __LINE__)
    /* See!  This is why it's a bad idea to use abbreviations... lack of consistency */
    #define hemp_string_copy(src)           hemp_mem_trace_strdup(src, __FILE__, __LINE__)
#else
    #define hemp_mem_alloc(size)            malloc(size)
    #define hemp_mem_resize(memory, size)   realloc(memory, size)
    #define hemp_mem_free(memory)           free(memory)
    #define hemp_string_copy(src)           strdup(src)
    #define hemp_mem_trace_external(m,s,f,l)
    #define hemp_mem_trace_reset()
#endif

#endif /* HEMP_MEMORY_H */
