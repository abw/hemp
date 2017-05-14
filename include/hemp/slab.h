#ifndef HEMP_SLAB_H
#define HEMP_SLAB_H

#include <hemp/core.h>


/*--------------------------------------------------------------------------
 * type definitions
 *--------------------------------------------------------------------------*/

struct hemp_slab {
    HempSize        size;               /* size of allocated memory         */
    HempMemory      data;               /* pointer to memory                */
    HempSlab        next;               /* next slab in linked list         */
};


/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

HempSlab
hemp_slab_new(
    HempSize        size
);

void
hemp_slab_free(
    HempSlab        slab
);


#endif /* HEMP_SLAB_H */
