#ifndef HEMP_STRING_H
#define HEMP_STRING_H

#include <hemp/core.h>
#include <hemp/type/list.h>

struct hemp_string_split_s {
    hemp_str_p left;
    hemp_str_p right;
};

typedef struct hemp_string_split_s * hemp_string_split_p;


#define hemp_string_eq(s1, s2) (                \
    strcmp(s1, s2) == 0                         \
)

#define hemp_stringn_eq(s1, s2, n) (            \
    strncmp(s1, s2, n) == 0                     \
)

#define hemp_string_clone(s,n) ({               \
    hemp_str_p _clone = hemp_string_copy(s);    \
    if (! _clone) hemp_mem_fail(n);             \
    _clone;                                     \
})

hemp_str_p
    hemp_string_extract(
        hemp_str_p from,
        hemp_str_p to
    );

hemp_list_p
    hemp_string_split(
        hemp_str_p source,
        hemp_str_p split
    );

hemp_list_p
    hemp_string_splits(
        hemp_str_p source,
        hemp_str_p token
    );

void
    hemp_string_trim(
        hemp_str_p string
    );

void
    hemp_string_chomp(
        hemp_str_p src
    );

hemp_bool_t
    hemp_string_wordlike(
        hemp_str_p str
    );

hemp_bool_t
    hemp_string_numlike(
        hemp_str_p string
    );

HEMP_INLINE hemp_str_p
    hemp_string_next_space(
        hemp_str_p string
    );

HEMP_INLINE hemp_bool_t
    hemp_string_to_next_space(
        hemp_str_p *string
    );

HEMP_INLINE hemp_str_p
    hemp_string_next_nonspace(
        hemp_str_p string
    );

HEMP_INLINE hemp_bool_t
    hemp_string_to_next_nonspace(
        hemp_str_p *string
    );

HEMP_INLINE hemp_str_p
    hemp_string_next_line(
        hemp_str_p string
    );

HEMP_INLINE hemp_bool_t
    hemp_string_to_next_line(
        hemp_str_p *string
    );

HEMP_INLINE hemp_list_p
    hemp_string_words(
        hemp_str_p string
    );

hemp_list_p
    hemp_string_nwords(
        hemp_str_p  string,
        hemp_size_t max
    );


HEMP_TYPE_FUNC(hemp_type_string);
HEMP_OUTPUT_FUNC(hemp_value_string_text);
HEMP_VALUE_FUNC(hemp_value_string_number);
HEMP_VALUE_FUNC(hemp_value_string_integer);
HEMP_VALUE_FUNC(hemp_value_string_boolean);
HEMP_VALUE_FUNC(hemp_method_string_length);


#endif /* HEMP_STRING_H */
