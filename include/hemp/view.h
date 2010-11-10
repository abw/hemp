#ifndef HEMP_VIEW_H
#define HEMP_VIEW_H

#include <hemp/core.h>
#include <hemp/element.h>



/*--------------------------------------------------------------------------
 * data structures
 *--------------------------------------------------------------------------*/

struct hemp_view_s {
    hemp_p       hemp;
    hemp_str_p   name;
    hemp_view_p  parent;
    hemp_view_f *viewer;
    hemp_size_t  size;
};



/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

hemp_view_p
    hemp_view_init(
        hemp_p      hemp,
        hemp_str_p  name
    );

void
    hemp_view_free( 
        hemp_view_p view
    );

hemp_view_p 
    hemp_view_text_init(
        hemp_p      hemp,
        hemp_str_p  name
    );

HEMP_VIEW_FUNC(hemp_view_element);



#endif /* HEMP_VIEW_H */
