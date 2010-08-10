#ifndef HEMP_MEMORY_H
#define HEMP_MEMORY_H

#include <hemp/core.h>


void hemp_mem_fail(hemp_cstr_p);


#define hemp_struct_alloc(type, name, label)            \
    type *name = (type *) hemp_mem_alloc(               \
        sizeof(type)                                    \
    );                                                  \
    if (! name) hemp_mem_fail(label);                   \
    debug_mem(                                          \
        "Allocated %d bytes at %p for new %s type\n",   \
        sizeof(type), name, label                       \
    );




/*--------------------------------------------------------------------------
 * patch in memmove(), bcopy() or a hand-rolled memory copy function
 *--------------------------------------------------------------------------*/

#if  HAVE_memmove
#   define hemp_mem_copy(src, dest, len) memmove(dest, src, len)
#elif HAVE_bcopy
#   define hemp_mem_copy(src, dest, len) bcopy(src, dest, len)
#else
#   define HEMP_ADD_MEM_COPY 1
    hemp_mem_p hemp_mem_copy(
        hemp_mem_p  src,
        hemp_mem_p  dest,
        hemp_size_t len
    );
#endif


/*--------------------------------------------------------------------------
 * memory debugging
 *--------------------------------------------------------------------------*/


#if DEBUG & DEBUG_MEM
    #define HEMP_MEM_WILD     0    /* uninitialised (wild)                     */
    #define HEMP_MEM_FAIL     1    /* malloc failed                            */
    #define HEMP_MEM_MALLOC   2    /* malloc succeeded                         */
    #define HEMP_MEM_MOVED    3    /* realloc relocated                        */
    #define HEMP_MEM_FREE     4    /* memory free'd                            */

    #define HEMP_MEM_BLKSIZ   8    /* # of mrec's to malloc at once            */
    #define HEMP_MEM_PEEKLEN  32   /* # of bytes to peek into memory           */

    struct hemp_mem_trace_s {
        int          id;
        int          status;
        hemp_cstr_p  file;
        hemp_pos_t   line;
        hemp_mem_p   ptr;
        hemp_size_t  size;
    };
    typedef struct hemp_mem_trace_s *hemp_mem_trace_p;

    hemp_mem_trace_p hemp_mem_new_trace();
    hemp_mem_trace_p hemp_mem_get_trace(hemp_mem_p, hemp_cstr_p, hemp_pos_t);
    hemp_size_t      hemp_mem_trace_report(hemp_bool_t);
    void             hemp_mem_trace_reset();

    hemp_mem_p       hemp_mem_trace_malloc(hemp_size_t, hemp_cstr_p, hemp_pos_t);
    hemp_mem_p       hemp_mem_trace_realloc(hemp_mem_p, hemp_size_t, hemp_cstr_p, hemp_pos_t);
    hemp_cstr_p      hemp_mem_trace_strdup(hemp_cstr_p, hemp_cstr_p, hemp_pos_t);
    void             hemp_mem_trace_free(hemp_mem_p, hemp_cstr_p, hemp_pos_t);

    #define hemp_mem_alloc(size)            hemp_mem_trace_malloc(size, __FILE__, __LINE__)
    #define hemp_mem_resize(memory, size)   hemp_mem_trace_realloc(memory, size, __FILE__, __LINE__)
    #define hemp_mem_free(memory)           hemp_mem_trace_free(memory, __FILE__, __LINE__)
    #define hemp_cstr_copy(src)             hemp_mem_trace_strdup(src, __FILE__, __LINE__)
#else
    #define hemp_mem_alloc(size)            malloc(size)
    #define hemp_mem_resize(memory, size)   realloc(memory, size)
    #define hemp_mem_free(memory)           free(memory)
    #define hemp_cstr_copy(src)             strdup(src)
//    #define hemp_mem_trace_report(verbose) -1
    #define hemp_mem_trace_reset()
#endif

#endif /* HEMP_MEMORY_H */
