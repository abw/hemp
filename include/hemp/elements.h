#ifndef HEMP_ELEMENTS_H
#define HEMP_ELEMENTS_H

#include <hemp/element.h>
#include <hemp/pool.h>
#include <hemp/symbol.h>


/*--------------------------------------------------------------------------
 * data structures
 *--------------------------------------------------------------------------*/

struct hemp_elements {
    hemp_hemp           hemp;
    hemp_pool           pool;
    hemp_element        head;
    hemp_element        tail;
};


/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

hemp_elements
    hemp_elements_new(
        hemp_hemp       hemp,
        hemp_size       capacity
    );

hemp_element
    hemp_elements_create(
        hemp_elements   elements,
        hemp_symbol     type,
        hemp_string     token,
        hemp_pos        position,
        hemp_size       length
    );

hemp_element
    hemp_elements_append(
        hemp_elements   elements,
        hemp_symbol     type,
        hemp_string     token,
        hemp_pos        position,
        hemp_size       length
    );

hemp_element
    hemp_elements_eof(
        hemp_elements   elements,
        hemp_pos        position
    );

void 
    hemp_elements_free(
        hemp_elements   elements
    );
    
void 
    hemp_elements_dump(
        hemp_elements   elements
    );


#endif /* HEMP_ELEMENTS_H */
