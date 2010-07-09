/* A slab is a block of memory with a next pointer, allowing then to be 
 * chained together in a singly linked list
 */

#include "hemp/slab.h"
#include "hemp/debug.h"


hemp_slab_t
hemp_slab_init(
    hemp_size_t size
) {
    hemp_slab_t slab;
    
    if ((slab = (hemp_slab_t) hemp_mem_init(sizeof(struct hemp_slab)))) {
        if ((slab->data = hemp_mem_init(size))) {
            slab->size  = size;
            slab->next  = NULL;
            debug_mem(
                "Allocated slab of %d bytes at %p\n", 
                size, slab->data
            );
        }
        else {
            hemp_slab_null(slab);
        }
    }

    // TODO: handle slab == NULL

    return slab;
}


void
hemp_slab_free(
    hemp_slab_t slab
) {
    hemp_slab_t next_slab;
    
    while (slab) {
        next_slab = slab->next;

        if (slab->data) {
            debug_mem(
                "Releasing slab buffer of %d bytes at %p\n", 
                slab->size, slab->data
            );
            hemp_mem_free(slab->data);
        }
        hemp_mem_free(slab);
    
        slab = next_slab;
    }
}

