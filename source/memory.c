#include <stdio.h>
#include "hemp/memory.h"
#include "hemp/debug.h"


/* hemp_memory_copy(src, dest, length) 
 *
 * Generic memory copy for systems (if there are any) that don't provide 
 * memmove() or bcopy().
 */

hemp_ptr_t
hemp_mem_copy(
    hemp_mem_t  src,
    hemp_ptr_t  dest,
    hemp_size_t len
) {
    hemp_size_t i;
    hemp_text_t s = (hemp_text_t) src;
    hemp_text_t d = (hemp_text_t) dest;

    debug_red("WARNING: using hemp_mem_copy() is a bad idea\n");
    if (d > s) {
        d += len;
        s += len;
        for (i = 0; i < len; ++i)
            *(--d) = *(--s);
        return d;
    }
    else {
        for (i = 0; i < len; ++i)
            *d++ = *s++;
        return d - len;
    }
}

