#ifndef HEMP_FRAME_H
#define HEMP_FRAME_H

#include <hemp/core.h>
#include <hemp/type/list.h>
#include <hemp/type/hash.h>


/*--------------------------------------------------------------------------
 * data structures
 *--------------------------------------------------------------------------*/

struct hemp_frame {
    hemp_context        context;
    hemp_frame          parent;
    hemp_element        element;
    hemp_list           args;
    hemp_hash           vars;
};


/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

hemp_frame
    hemp_frame_init(
        hemp_frame      frame
    );

void
    hemp_frame_free(
        hemp_frame      frame
    );


/*--------------------------------------------------------------------------
 * macros
 *--------------------------------------------------------------------------*/

#define hemp_frame_new()                        \
    hemp_frame_init(NULL)


#endif /* HEMP_FRAME_H */