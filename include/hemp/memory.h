#ifndef HEMP_MEMORY_H
#define HEMP_MEMORY_H

#include <stdlib.h>
#include "hemp/types.h"

#define hemp_mem_init(size)         malloc(size)
#define hemp_mem_free(memory)       free(memory)
#define hemp_mem_size(memory, size) realloc(memory, size)

/* define macro to copy memory non-destructively, using memmove() or bcopy(),
 * if available, or use our own version if not.
 */
#ifdef HAVE_MEMMOVE
#  define hemp_mem_copy(src, dest, len) memmove(dest, src, len)
#elif HAVE_BCOPY
#  define hemp_mem_copy(src, dest, len) bcopy(src, dest, len)
#else
hemp_ptr_t hemp_mem_copy(
    hemp_mem_t  src,
    hemp_ptr_t  dest,
    hemp_size_t len
);
#endif

#endif /* HEMP_MEMORY_H */