#include <hemp/slot.h>


HEMP_INLINE hemp_slot
hemp_slot_init(
    hemp_value      parent,
    hemp_size       index,
    hemp_string     name,
    hemp_value      value,
    hemp_slot       next
) {
    hemp_slot slot;
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
    hemp_slot slot
) {
    // TODO: maybe free name
    // TODO: maybe free value
    hemp_mem_free(slot);
}

