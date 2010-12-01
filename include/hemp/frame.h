#ifndef HEMP_FRAME_H
#define HEMP_FRAME_H

#include <hemp/core.h>
#include <hemp/type/params.h>


/*--------------------------------------------------------------------------
 * type definitions
 *--------------------------------------------------------------------------*/

struct hemp_frame {
    hemp_context    context;
    hemp_frame      parent;
    hemp_element    element;
    hemp_params     params;
    hemp_hash       vars;
//    hemp_list       args;
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