#ifndef HEMP_MEMORY_H
#define HEMP_MEMORY_H

#include <stdlib.h>
#include "hemp/types.h"


/*--------------------------------------------------------------------------
 * macros
 *--------------------------------------------------------------------------*/

#define hemp_mem_init(size)         malloc(size)
#define hemp_mem_free(memory)       free(memory)
#define hemp_mem_size(memory, size) realloc(memory, size)

#if  HAVE_memmove
#   define hemp_mem_copy(src, dest, len) memmove(dest, src, len)
#elif HAVE_bcopy
#   define hemp_mem_copy(src, dest, len) bcopy(src, dest, len)
#else
#   define HEMP_ADD_MEM_COPY 1
    hemp_ptr_t hemp_mem_copy(
        hemp_mem_t  src,
        hemp_ptr_t  dest,
        hemp_size_t len
    );
#endif


#endif /* HEMP_MEMORY_H */