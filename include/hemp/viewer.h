#ifndef HEMP_VIEWER_H
#define HEMP_VIEWER_H

#include <hemp/core.h>
#include <hemp/element.h>



/*--------------------------------------------------------------------------
 * data structures
 *--------------------------------------------------------------------------*/

struct hemp_viewer {
    hemp_hemp           hemp;
    hemp_string         name;
    hemp_viewer         parent;
    hemp_view_f       * view;
    hemp_u16            size;
};



/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

hemp_viewer
    hemp_viewer_new(
        hemp_hemp       hemp,
        hemp_string     name
    );

void
    hemp_viewer_free( 
        hemp_viewer     viewer
    );

hemp_bool
    hemp_viewer_add_view(
        hemp_viewer     viewer,
        hemp_string     name,
        hemp_view_f     view
    );

hemp_viewer 
    hemp_viewer_text_init(
        hemp_hemp       hemp,
        hemp_string     name
    );

HEMP_VIEW_FUNC(hemp_viewer_element);


#endif /* HEMP_VIEWER_H */
