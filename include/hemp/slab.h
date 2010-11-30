#ifndef HEMP_SLAB_H
#define HEMP_SLAB_H

#include <hemp/core.h>


/*--------------------------------------------------------------------------
 * type definitions
 *--------------------------------------------------------------------------*/

struct hemp_slab {
    hemp_size       size;               /* size of allocated memory         */
    hemp_memory     data;               /* pointer to memory                */
    hemp_slab       next;               /* next slab in linked list         */
};


/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

hemp_slab
hemp_slab_new(
    hemp_size       size
);

void
hemp_slab_free(
    hemp_slab       slab
);


#endif /* HEMP_SLAB_H */

