#ifndef HEMP_FRAME_H
#define HEMP_FRAME_H

#include <hemp/core.h>
#include <hemp/type/params.h>


/*--------------------------------------------------------------------------
 * type definitions
 *--------------------------------------------------------------------------*/

struct hemp_frame {
    HempContext     context;
    HempFrame       parent;
    HempFragment    fragment;
    HempParams      params;
    HempHash        vars;
};


/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

HempFrame
hemp_frame_init(
    HempFrame       frame
);

void
hemp_frame_free(
    HempFrame       frame
);


/*--------------------------------------------------------------------------
 * macros
 *--------------------------------------------------------------------------*/

#define hemp_frame_new()    \
    hemp_frame_init(NULL)


#endif /* HEMP_FRAME_H */