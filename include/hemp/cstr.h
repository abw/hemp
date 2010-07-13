#ifndef HEMP_TEXT_H
#define HEMP_TEXT_H

#include <string.h>
#include <stdlib.h>
#include "hemp/types.h"
#include "hemp/memory.h"
#include "hemp/list.h"

/* TODO: rename this to cstr.h */

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


#endif /* HEMP_TEXT_H */
