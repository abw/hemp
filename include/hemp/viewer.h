#ifndef HEMP_VIEWER_H
#define HEMP_VIEWER_H

#include <hemp/core.h>
#include <hemp/element.h>



/*--------------------------------------------------------------------------
 * type definitions
 *--------------------------------------------------------------------------*/

struct hemp_viewer {
    Hemp            hemp;
    HempString      name;
    HempViewer      parent;
    hemp_view_f   * view;
    HempU16         size;
};



/*--------------------------------------------------------------------------
 * Factory prototypes
 *--------------------------------------------------------------------------*/

HEMP_FACTORY(hemp_viewer_factory);
HEMP_HASH_ITERATOR(hemp_viewer_cleaner);


/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

HempViewer
hemp_viewer_new(
    Hemp            hemp,
    HempString      name
);

void
hemp_viewer_free( 
    HempViewer      viewer
);

HempBool
hemp_viewer_add_view(
    HempViewer      viewer,
    HempString      name,
    hemp_view_f     view
);

HEMP_VIEWER(hemp_viewer_text);
HEMP_VIEW(hemp_viewer_fragment);


#endif /* HEMP_VIEWER_H */
