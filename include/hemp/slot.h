#ifndef HEMP_SLOT_H
#define HEMP_SLOT_H

#include <hemp/core.h>
#include <hemp/type.h>


/*--------------------------------------------------------------------------
 * type definitions
 *--------------------------------------------------------------------------*/

struct hemp_slot {
    HempValue       parent;         /* owner of this slot                   */
    HempSize        index;          /* numerical hash/index value           */
    HempString      name;           /* lookup key                           */
    HempValue       value;          /* corresponding value                  */
    HempSlot        next;           /* next slot in hash/free linked list   */
};


/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

HempSlot
hemp_slot_init(
    HempValue       parent,
    HempSize        index,
    HempString      name,
    HempValue       value,
    HempSlot        next
);

void
hemp_slot_free(
    HempSlot       slot
);


#endif /* HEMP_SLOT_H */
