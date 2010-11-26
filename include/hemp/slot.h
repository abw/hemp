#ifndef HEMP_SLOT_H
#define HEMP_SLOT_H

#include <hemp/core.h>
#include <hemp/type.h>


/*--------------------------------------------------------------------------
 * data structures
 *--------------------------------------------------------------------------*/

struct hemp_slot {
    hemp_value        parent;     /* owner of this slot                   */
    hemp_size           index;      /* numerical hash/index value           */
    hemp_string         name;       /* lookup key                           */
    hemp_value        value;      /* corresponding value                  */
    hemp_slot           next;       /* next slot in hash/free linked list   */
};


/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

HEMP_INLINE hemp_slot
    hemp_slot_init(  
        hemp_value    parent,
        hemp_size       index,
        hemp_string     name,
        hemp_value    value,
        hemp_slot       next
    );

void
    hemp_slot_free(
        hemp_slot       slot
    );

#endif /* HEMP_SLOT_H */
