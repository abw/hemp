#include "hemp/slab.h"


hemp_slab
hemp_slab_new(
    hemp_size   size
) {
    /* 
    ** Perform a single memory allocation for the requested block size plus 
    ** the size of the housekeeping record.  Then set the data pointer in the 
    ** housekeeping record to reference the first byte after the header
    */
    hemp_slab slab = (hemp_slab) hemp_mem_alloc( 
        size + sizeof(struct hemp_slab) 
    );

    if (! slab) 
        hemp_mem_fail("slab");

    slab->size  = size;
    slab->next  = NULL;
    slab->data  = ((char *) slab) + sizeof(struct hemp_slab);

    return slab;
}


void
hemp_slab_free(
    hemp_slab slab
) {
    hemp_slab next_slab;
    while (slab) {
//      hemp_debug_mem("freeing slab at %p\n", slab);
        next_slab = slab->next;
        hemp_mem_free(slab);
        slab = next_slab;
    }
}

