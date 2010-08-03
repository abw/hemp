#ifndef HEMP_CSTR_H
#define HEMP_CSTR_H

#include <hemp/core.h>
#include <hemp/list.h>


#define hemp_cstr_eq(s1, s2) (      \
    strcmp(s1, s2) == 0             \
)

#define hemp_cstrn_eq(s1, s2, n) (  \
    strncmp(s1, s2, n) == 0         \
)

hemp_list_p
    hemp_cstr_split(
        hemp_cstr_p source,
        hemp_cstr_p split
    );


#endif /* HEMP_CSTR_H */
