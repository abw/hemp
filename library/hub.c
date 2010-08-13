#include "hemp/hub.h"
#include "hemp/debug.h"


hemp_hub_t 
hemp_hub_init( ) {
    hemp_hub_t hub;
    
    if ((hub = (hemp_hub_t) hemp_mem_init(sizeof(struct hemp_hub)))) {
        hemp_debug_mem("Allocated new hub at %p\n", hub);
        
        hub->tags     = hemp_hash_init();
        hub->elements = hemp_hash_init();
        hub->vtypes   = hemp_hash_init();

        if (hub->tags && hub->elements && hub->vtypes) {
            hemp_debug("created hub with tags, elements and vtypes\n");
        }
        else {
            hemp_hub_null(hub);
        }
    }
    
    // TODO: handle hub == NULL

    return hub;
}


void
hemp_hub_free(
    hemp_hub_t hub
) {
    hemp_debug_mem("Releasing hub at %p\n", hub);

    if (hub->tags)
        hemp_hash_free(hub->tags);

    if (hub->elements) {
        // TODO: free each element types
        hemp_hash_free(hub->elements);
    }

    if (hub->vtypes) {
        // TODO: free each vtypes
        hemp_hash_each(hub->vtypes, &hemp_hub_free_vtypes);
        hemp_hash_free(hub->vtypes);
    }

    hemp_mem_free(hub);
}


void hemp_hub_free_vtypes(
    hemp_hash_entry_t   entry
) {
    hemp_vtypes_free(entry->value);
}


