#include <hemp/slot.h>


HEMP_DO_INLINE hemp_slot_p
hemp_slot_init(
    hemp_value_t    parent,
    hemp_size_t     index,
    hemp_str_p      name,
    hemp_value_t    value,
    hemp_slot_p     next
) {
    hemp_slot_p slot = (hemp_slot_p) hemp_mem_alloc(
        sizeof(struct hemp_slot_s)
    );
    
    if (! slot)
        hemp_mem_fail("slot");

    slot->parent = parent;
    slot->index  = index;
    slot->name   = name;
    slot->value  = value;
    slot->next   = next;

    return slot;
}


HEMP_DO_INLINE void
hemp_slot_free(
    hemp_slot_p slot
) {
    // TODO: maybe free name
    // TODO: maybe free value
    hemp_mem_free(slot);
}

