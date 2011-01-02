#include <hemp/slot.h>


HEMP_INLINE HempSlot
hemp_slot_init(
    HempValue      parent,
    HempSize       index,
    HempString     name,
    HempValue      value,
    HempSlot       next
) {
    HempSlot slot;
    HEMP_ALLOCATE(slot);

    slot->parent = parent;
    slot->index  = index;
    slot->name   = name;
    slot->value  = value;
    slot->next   = next;

    return slot;
}


HEMP_INLINE void
hemp_slot_free(
    HempSlot slot
) {
    // TODO: maybe free name
    // TODO: maybe free value
    hemp_mem_free(slot);
}

