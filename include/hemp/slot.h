#ifndef HEMP_SLOT_H
#define HEMP_SLOT_H

#include <hemp/core.h>
#include <hemp/value.h>


/*--------------------------------------------------------------------------
 * data structures
 *--------------------------------------------------------------------------*/

struct hemp_slot_s {
    hemp_value_t        parent;     /* owner of this slot                   */
    hemp_size_t         index;      /* numerical hash/index value           */
    hemp_cstr_p         name;       /* lookup key                           */
    hemp_value_t        value;      /* corresponding value                  */
    hemp_slot_p         next;       /* next slot in hash/free linked list   */
};


/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

HEMP_DO_INLINE hemp_slot_p
    hemp_slot_init(  
        hemp_value_t    parent,
        hemp_size_t     index,
        hemp_cstr_p     name,
        hemp_value_t    value,
        hemp_slot_p     next
    );

void
    hemp_slot_free(
        hemp_slot_p slot
    );

#endif /* HEMP_SLOT_H */
