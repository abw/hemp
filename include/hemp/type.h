#ifndef HEMP_TYPE_H
#define HEMP_TYPE_H

#include <string.h>
#include <stdlib.h>
#include "hemp/types.h"


#define HEMP_TYPE_BASE      \
    hemp_name_t  name;      \
    hemp_type_t  base;      \
    hemp_size_t  size;      \
    hemp_pool_t  pool;      \
    hemp_init_fn init;      \
    hemp_free_fn free;      \
    hemp_value_fn value;      \
    hemp_text_fn text;


struct hemp_type {
    HEMP_TYPE_BASE
    /* record size, alloc, compare, print, getter, setter,
       as number/sequence/mapping, attributes.
       pool, dead, live, used, etc
    */
};


#endif /* HEMP_TYPE_H */
