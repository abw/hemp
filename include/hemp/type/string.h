#ifndef HEMP_STRING_H
#define HEMP_STRING_H

#include <hemp/core.h>
#include <hemp/type/list.h>


/*--------------------------------------------------------------------------
 * type definitions
 *--------------------------------------------------------------------------*/

struct hemp_string_split_s {
    hemp_string     left;
    hemp_string     right;
};

typedef struct hemp_string_split_s * hemp_string_split_p;



/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

hemp_string
hemp_string_sprintf(
    const hemp_string format,
    ...
);

hemp_string
hemp_string_extract(
    hemp_string     from,
    hemp_string     to
);

hemp_list
hemp_string_split(
    hemp_string     source,
    hemp_string     split
);

hemp_list
hemp_string_splits(
    hemp_string     source,
    hemp_string     token
);

void
hemp_string_trim(
    hemp_string     string
);

HEMP_INLINE void
hemp_string_chomp(
    hemp_string     src
);

HEMP_INLINE hemp_bool
hemp_string_wordlike(
    hemp_string     str
);

HEMP_INLINE hemp_bool
hemp_string_intlike(
    hemp_string     string
);

HEMP_INLINE hemp_bool
hemp_string_numlike(
    hemp_string     string
);

HEMP_INLINE hemp_string
hemp_string_next_space(
    hemp_string     string
);

HEMP_INLINE hemp_bool
hemp_string_to_next_space(
    hemp_string   * string
);

HEMP_INLINE hemp_string
hemp_string_next_nonspace(
    hemp_string     string
);

HEMP_INLINE hemp_bool
hemp_string_to_next_nonspace(
    hemp_string   * string
);

HEMP_INLINE hemp_string
hemp_string_next_line(
    hemp_string     string
);

HEMP_INLINE hemp_bool
hemp_string_to_next_line(
    hemp_string   * string
);

HEMP_INLINE hemp_list
hemp_string_words(
    hemp_string     string
);

hemp_list
hemp_string_nwords(
    hemp_string     string,
    hemp_size       max
);


HEMP_TYPE_FUNC(hemp_type_string);
HEMP_OUTPUT_FUNC(hemp_type_string_text);
HEMP_VALUE_FUNC(hemp_type_string_number);
HEMP_VALUE_FUNC(hemp_type_string_integer);
HEMP_VALUE_FUNC(hemp_type_string_boolean);
HEMP_VALUE_FUNC(hemp_method_string_length);


/*--------------------------------------------------------------------------
 * macros
 *--------------------------------------------------------------------------*/

#define hemp_string_eq(s1, s2) (                \
    strcmp(s1, s2) == 0                         \
)

#define hemp_stringn_eq(s1, s2, n) (            \
    strncmp(s1, s2, n) == 0                     \
)

#define hemp_string_clone(s,n) ({               \
    hemp_string _clone = hemp_string_copy(s);   \
    if (! _clone) hemp_mem_fail(n);             \
    _clone;                                     \
})

#define hemp_string_free(s)                     \
    hemp_mem_free(s)

#endif /* HEMP_STRING_H */
