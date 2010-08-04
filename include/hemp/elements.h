#ifndef HEMP_ELEMENTS_H
#define HEMP_ELEMENTS_H

#include <hemp/element.h>
#include <hemp/pool.h>


/*--------------------------------------------------------------------------
 * data structures
 *--------------------------------------------------------------------------*/

struct hemp_elements_s {
    hemp_pool_p     pool;
    hemp_element_p  head;
    hemp_element_p  tail;
};


/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

hemp_elements_p
    hemp_elements_init(
        hemp_size_t capacity
    );

hemp_element_p
    hemp_elements_create(
        hemp_elements_p elements,
        hemp_etype_p    type,
        hemp_cstr_p     token,
        hemp_pos_t      position,
        hemp_size_t     length
    );

hemp_element_p
    hemp_elements_append(
        hemp_elements_p elements,
        hemp_etype_p    type,
        hemp_cstr_p     token,
        hemp_pos_t      position,
        hemp_size_t     length
    );

hemp_element_p
    hemp_elements_eof(
        hemp_elements_p elements,
        hemp_pos_t      position
    );

void 
    hemp_elements_free(
        hemp_elements_p elements
    );
    
void 
    hemp_elements_dump(
        hemp_elements_p elements
    );



#endif /* HEMP_ELEMENTS_H */
