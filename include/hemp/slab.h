#ifndef HEMP_SLAB_H
#define HEMP_SLAB_H

#include "hemp/memory.h"

typedef struct hemp_slab * hemp_slab_t;

struct hemp_slab {
    hemp_ptr_t  data;       /* pointer to memory            */
    hemp_size_t size;       /* size of allocated memory     */
    hemp_slab_t next;       /* next slab in linked list     */
};

hemp_slab_t hemp_slab_init(hemp_size_t size);
void        hemp_slab_free(hemp_slab_t slab);

#endif /* HEMP_SLAB_H */

