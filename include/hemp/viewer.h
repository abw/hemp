#ifndef HEMP_VIEWER_H
#define HEMP_VIEWER_H

#include <hemp/core.h>
#include <hemp/element.h>



/*--------------------------------------------------------------------------
 * data structures
 *--------------------------------------------------------------------------*/

struct hemp_viewer_s {
    hemp_p          hemp;
    hemp_str_p      name;
    hemp_viewer_p   parent;
    hemp_view_f    *view;
    hemp_size_t     size;
};



/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

hemp_viewer_p
    hemp_viewer_init(
        hemp_p      hemp,
        hemp_str_p  name
    );

void
    hemp_viewer_free( 
        hemp_viewer_p view
    );

hemp_viewer_p 
    hemp_viewer_text_init(
        hemp_p      hemp,
        hemp_str_p  name
    );

HEMP_VIEW_FUNC(hemp_viewer_element);


#endif /* HEMP_VIEWER_H */
