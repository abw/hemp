#include "hemp/slab.h"


hemp_slab_p
hemp_slab_init(
    hemp_size_t size
) {
    /* Perform a single memory allocation for the requested block size plus 
     * the size of the housekeeping record.  Then set the data pointer in the 
     * housekeeping record to reference the first byte after the header
     */
    hemp_slab_p slab = (hemp_slab_p) hemp_mem_alloc( 
        size + sizeof(struct hemp_slab_s) 
    );

    if (! slab) 
        hemp_mem_fail("slab");

    slab->size  = size;
    slab->next  = NULL;
    slab->data  = ((char *) slab) + sizeof(struct hemp_slab_s);
    
//    hemp_debug_mem(
//        "allocated slab (%d bytes) at %p and %d bytes of data at %p\n",
//        sizeof(struct hemp_slab_s), slab, size, slab->data
//    );

    return slab;
}


void
hemp_slab_free(
    hemp_slab_p slab
) {
    hemp_slab_p next_slab;

    while (slab) {
        next_slab = slab->next;
        hemp_mem_free(slab);
        slab = next_slab;
    }
}

