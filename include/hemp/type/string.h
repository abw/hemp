#ifndef HEMP_STRING_H
#define HEMP_STRING_H

#include <hemp/core.h>
#include <hemp/type/list.h>


/*--------------------------------------------------------------------------
 * type definitions
 *--------------------------------------------------------------------------*/

struct hemp_str_split {
    HempString     left;
    HempString     right;
};

typedef struct hemp_str_split * hemp_str_split;
typedef struct hemp_str_pos   * hemp_str_pos;



/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

HempString
hemp_string_vprintf(
    const HempString format,
    va_list args
);

HempString
hemp_string_sprintf(
    const HempString format,
    ...
);

HempString
hemp_string_extract(
    HempString     from,
    HempString     to
);

HempList
hemp_string_split(
    HempString     source,
    HempString     split
);

HempList
hemp_string_splits(
    HempString     source,
    HempString     token
);

void
hemp_string_trim(
    HempString     string
);

HEMP_INLINE void
hemp_string_chomp(
    HempString     src
);

HEMP_INLINE HempBool
hemp_string_wordlike(
    HempString     str
);

HEMP_INLINE HempBool
hemp_string_intlike(
    HempString     string
);

HEMP_INLINE HempBool
hemp_string_numlike(
    HempString     string
);

HEMP_INLINE HempString
hemp_string_next_space(
    HempString     string
);

HEMP_INLINE HempBool
hemp_string_to_next_space(
    HempString   * string
);

HEMP_INLINE HempString
hemp_string_next_nonspace(
    HempString     string
);

HEMP_INLINE HempBool
hemp_string_to_next_nonspace(
    HempString   * string
);

HEMP_INLINE HempString
hemp_string_next_line(
    HempString     string
);

HEMP_INLINE HempBool
hemp_string_to_next_line(
    HempString   * string
);

HEMP_INLINE HempList
hemp_string_words(
    HempString     string
);

HempList
hemp_string_nwords(
    HempString     string,
    HempSize       max
);

HempLocation
hemp_string_location(
    HempString     string,
    HempString     marker,
    HempLocation   location
);


HEMP_TYPE_FUNC(hemp_type_string);
HEMP_OUTPUT(hemp_type_string_text);
HEMP_VALUE(hemp_type_string_number);
HEMP_VALUE(hemp_type_string_integer);
HEMP_VALUE(hemp_type_string_boolean);
HEMP_VALUE(hemp_method_string_length);


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
    HempString _clone = hemp_string_copy(s);   \
    if (! _clone) hemp_mem_fail(n);             \
    _clone;                                     \
})

#define hemp_string_free(s)                     \
    hemp_mem_free(s)

#endif /* HEMP_STRING_H */
