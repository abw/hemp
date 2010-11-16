#ifndef HEMP_MEMORY_H
#define HEMP_MEMORY_H

#include <hemp/core.h>


void hemp_mem_fail(hemp_str_p);


#define hemp_struct_alloc(type, name, label)            \
    type *name = (type *) hemp_mem_alloc(               \
        sizeof(type)                                    \
    );                                                  \
    if (! name) hemp_mem_fail(label);                   \
    hemp_debug_mem(                                     \
        "Allocated %d bytes at %p for new %s type\n",   \
        sizeof(type), name, label                       \
    );

#define hemp_allocate_type_name(type, name) ({                      \
    if (! name) {                                                   \
        name = (hemp_##type##_p) hemp_mem_alloc(                    \
            sizeof(struct hemp_##type##_s)                          \
        );                                                          \
        if (! name)                                                 \
            hemp_mem_fail(#type);                                   \
    }                                                               \
})






/*--------------------------------------------------------------------------
 * patch in memmove(), bcopy() or a hand-rolled memory copy function
 *--------------------------------------------------------------------------*/

#if  HAVE_MEMMOVE
#   define hemp_mem_copy(src, dest, len) memmove(dest, src, len)
#elif HAVE_BCOPY
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


#if HEMP_DEBUG & HEMP_DEBUG_MEM
    #define HEMP_MEM_WILD     0    /* uninitialised (wild)                  */
    #define HEMP_MEM_FAIL     1    /* malloc failed                         */
    #define HEMP_MEM_MALLOC   2    /* malloc succeeded                      */
    #define HEMP_MEM_MOVED    3    /* realloc relocated                     */
    #define HEMP_MEM_EXTERNAL 4    /* memory allocated by external function */
    #define HEMP_MEM_FREE     5    /* memory free'd                         */

    #define HEMP_MEM_BLKSIZ   8    /* # of mrec's to malloc at once         */
    #define HEMP_MEM_PEEKLEN  32   /* # of bytes to peek into memory        */

    struct hemp_mem_trace_s {
        int          id;
        int          status;
        hemp_str_p   file;
        hemp_pos_t   line;
        hemp_mem_p   ptr;
        hemp_size_t  size;
    };
    typedef struct hemp_mem_trace_s *hemp_mem_trace_p;

    hemp_mem_trace_p hemp_mem_new_trace();
    hemp_mem_trace_p hemp_mem_get_trace(hemp_mem_p, hemp_str_p, hemp_pos_t);
    hemp_size_t      hemp_mem_trace_report(hemp_bool_t);
    void             hemp_mem_trace_reset();

    hemp_mem_p       hemp_mem_trace_malloc(hemp_size_t, hemp_str_p, hemp_pos_t);
    hemp_mem_p       hemp_mem_trace_realloc(hemp_mem_p, hemp_size_t, hemp_str_p, hemp_pos_t);
    hemp_str_p       hemp_mem_trace_strdup(hemp_str_p, hemp_str_p, hemp_pos_t);
    hemp_mem_p       hemp_mem_trace_external(hemp_mem_p, hemp_size_t, hemp_str_p, hemp_pos_t);

    void             hemp_mem_trace_free(hemp_mem_p, hemp_str_p, hemp_pos_t);

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
//    #define hemp_mem_trace_report(verbose) -1
    #define hemp_mem_trace_reset()
#endif

#endif /* HEMP_MEMORY_H */
