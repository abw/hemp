#ifndef HEMP_FRAME_H
#define HEMP_FRAME_H

#include <hemp/core.h>
#include <hemp/type/list.h>
#include <hemp/type/hash.h>


/*--------------------------------------------------------------------------
 * data structures
 *--------------------------------------------------------------------------*/

struct hemp_frame_s {
    hemp_context_p  context;
    hemp_frame_p    parent;
    hemp_element_p  element;
    hemp_list_p     args;
    hemp_hash_p     vars;
};


/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

hemp_frame_p
    hemp_frame_init(
        hemp_frame_p
    );

void
    hemp_frame_free(
        hemp_frame_p
    );


/*--------------------------------------------------------------------------
 * macros
 *--------------------------------------------------------------------------*/

#define hemp_frame_new()                        \
    hemp_frame_init(NULL)

#define hemp_frame_allocate(name)               \
    hemp_allocate_type_name(frame, name)


#endif /* HEMP_FRAME_H */