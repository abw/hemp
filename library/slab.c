/* A slab is a block of memory with a next pointer, allowing then to be 
 * chained together in a singly linked list
 */

#include "hemp/slab.h"


hemp_slab_p
hemp_slab_init(
    hemp_size_t size
) {
    /* TODO: perform a single memory allocation for the requested block size
     * plus the size of the housekeeping record.  Then set the data pointer
     * in the housekeeping record to reference the first byte after the header
     */
    hemp_slab_p slab = (hemp_slab_p) hemp_mem_alloc( 
        sizeof(struct hemp_slab_s) 
    );

    if (! slab) 
        hemp_mem_fail("slab");

    slab->data = hemp_mem_alloc(size);

    if (! slab->data)
        hemp_mem_fail("slab data");

    slab->size  = size;
    slab->next  = NULL;

    return slab;
}


void
hemp_slab_free(
    hemp_slab_p slab
) {
    hemp_slab_p next_slab;
    
    while (slab) {
        next_slab = slab->next;

        if (slab->data)
            hemp_mem_free(slab->data);

        hemp_mem_free(slab);

        slab = next_slab;
    }
}

