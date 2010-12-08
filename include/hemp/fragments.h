#ifndef HEMP_FRAGMENTS_H
#define HEMP_FRAGMENTS_H

#include <hemp/fragment.h>
#include <hemp/pool.h>


/*--------------------------------------------------------------------------
 * type definitions
 *--------------------------------------------------------------------------*/

struct hemp_fragments {
    hemp_hemp       hemp;
    hemp_pool       pool;
    hemp_fragment   head;
    hemp_fragment   tail;
};


/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

hemp_fragments
hemp_fragments_new(
    hemp_hemp       hemp,
    hemp_size       capacity
);

hemp_fragment
hemp_fragments_create(
    hemp_fragments  fragments,
    hemp_element    type,
    hemp_string     token,
    hemp_pos        position,
    hemp_size       length
);

hemp_fragment
hemp_fragments_add_fragment(
    hemp_fragments  fragments,
    hemp_element    type,
    hemp_string     token,
    hemp_pos        position,
    hemp_size       length
);

hemp_fragment
hemp_fragments_add_eof(
    hemp_fragments  fragments,
    hemp_pos        position
);

void 
hemp_fragments_free(
    hemp_fragments  fragments
);

hemp_bool
hemp_fragments_free_fragment(
    hemp_memory     item
);
    
void 
hemp_fragments_dump(
    hemp_fragments  fragments
);


#endif /* HEMP_FRAGMENTS_H */
