#ifndef HEMP_ELEMENT_H
#define HEMP_ELEMENT_H

#include "hemp/memory.h"


/*--------------------------------------------------------------------------
 * data structures
 *--------------------------------------------------------------------------*/

struct hemp_etype {
    hemp_name_t     name;
    hemp_cstr_t     (*text)();
};

struct hemp_element {
    hemp_etype_t    type;
    hemp_cstr_t     token;
    hemp_pos_t      position;
    hemp_size_t     length;
    hemp_element_t  next;
};


/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

hemp_element_t  
    hemp_element_new(
        hemp_etype_t, 
        hemp_cstr_t, 
        hemp_pos_t, 
        hemp_size_t
    );

hemp_element_t  
    hemp_element_init();
    
void
    hemp_element_free(
        hemp_element_t element
    );


/*--------------------------------------------------------------------------
 * macros
 *--------------------------------------------------------------------------*/

#define hemp_element_null(e) \
    hemp_element_free(e);    \
    e = NULL;                


#endif /* HEMP_ELEMENT_H */