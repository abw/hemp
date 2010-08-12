#ifndef HEMP_CSTR_H
#define HEMP_CSTR_H

#include <hemp/core.h>
#include <hemp/list.h>

struct hemp_cstr_split_s {
    hemp_cstr_p left;
    hemp_cstr_p right;
};

typedef struct hemp_cstr_split_s * hemp_cstr_split_p;


#define hemp_cstr_eq(s1, s2) (      \
    strcmp(s1, s2) == 0             \
)

#define hemp_cstrn_eq(s1, s2, n) (  \
    strncmp(s1, s2, n) == 0         \
)

#define hemp_cstr_clone(s,n) ({             \
    hemp_cstr_p _clone = hemp_cstr_copy(s); \
    if (! _clone) hemp_mem_fail(n);         \
    _clone;                                 \
})

hemp_list_p
    hemp_cstr_split(
        hemp_cstr_p source,
        hemp_cstr_p split
    );

hemp_list_p
    hemp_cstr_splits(
        hemp_cstr_p source,
        hemp_cstr_p token
    );

void
    hemp_cstr_chomp(
        hemp_cstr_p src
    );

hemp_bool_t
    hemp_cstr_wordlike(
        hemp_cstr_p str
    );

void
    hemp_cstr_to_next_line(
        hemp_cstr_p *cstr
    );

#endif /* HEMP_CSTR_H */
