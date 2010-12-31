#ifndef HEMP_VIEWER_H
#define HEMP_VIEWER_H

#include <hemp/core.h>
#include <hemp/element.h>



/*--------------------------------------------------------------------------
 * type definitions
 *--------------------------------------------------------------------------*/

struct hemp_viewer {
    hemp_hemp       hemp;
    hemp_string     name;
    hemp_viewer     parent;
    hemp_view_f   * view;
    hemp_u16        size;
};



/*--------------------------------------------------------------------------
 * Factory prototypes
 *--------------------------------------------------------------------------*/

HEMP_FACTORY(hemp_viewer_factory);
HEMP_HASH_ITERATOR(hemp_viewer_cleaner);


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

HEMP_VIEWER(hemp_viewer_text);
HEMP_VIEW(hemp_viewer_fragment);


#endif /* HEMP_VIEWER_H */
