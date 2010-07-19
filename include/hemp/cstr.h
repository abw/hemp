#ifndef HEMP_CSTR_H
#define HEMP_CSTR_H

#include <string.h>
#include <stdlib.h>
#include "hemp/types.h"
#include "hemp/type.h"
#include "hemp/memory.h"
#include "hemp/list.h"

typedef hemp_cstr_t *hemp_hstr_t;

hemp_type_t
    hemp_hstr_type();

hemp_bool_t
    hemp_hstr_prepare(
        hemp_item_t item,
        HEMP_ARGS
    );

hemp_bool_t
    hemp_hstr_cleanup(
        hemp_item_t item
    );


#define hemp_cstr_eq(s1, s2) (      \
    strcmp(s1, s2) == 0             \
)

#define hemp_cstrn_eq(s1, s2, n) (  \
    strncmp(s1, s2, n) == 0         \
)

hemp_list_t 
    hemp_cstr_split(
        hemp_cstr_t source,
        hemp_cstr_t split
    );


#endif /* HEMP_CSTR_H */
