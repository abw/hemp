#ifndef HEMP_TEXT_H
#define HEMP_TEXT_H

#include <hemp/core.h>


/*--------------------------------------------------------------------------
 * type definitions
 *--------------------------------------------------------------------------*/

struct hemp_text {
    HempString      string;
    HempSize        capacity;
    HempSize        length;
}; 


/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

HempText 
hemp_text_init_size(
    HempText        text, 
    HempSize        capacity
);

HempText 
hemp_text_init_format(
    HempText        text, 
    HempString      format, 
    ...
);

void
hemp_text_free(
    HempText        text
);

HempText
hemp_text_from_text(
    HempText        text
);

HempText
hemp_text_from_string(
    HempString      string
);

HempText
hemp_text_append_text(
    HempText        text, 
    HempText        append
);

HempText
hemp_text_append_string(
    HempText        text, 
    HempString      append
);

HempText
hemp_text_append_stringn(
    HempText        text, 
    HempString      append, 
    HempSize        length
);

HempText
hemp_text_insert_string(
    HempText        text, 
    HempOffset      position, 
    HempString      insert
);

HempText
hemp_text_replace_string(
    HempText        text, 
    HempString      replace
);

HempText
hemp_text_truncate(
    HempText        text, 
    HempPos         length
);


void     
hemp_text_release(
    HempText        text
);

HEMP_INLINE void
hemp_text_clean(
    HempValue       value
);


/*--------------------------------------------------------------------------
 * type functions and methods
 *--------------------------------------------------------------------------*/

HEMP_TYPE(hemp_type_text);
HEMP_OUTPUT(hemp_type_text_text);
HEMP_VALUE(hemp_type_text_number);
HEMP_VALUE(hemp_type_text_integer);
HEMP_VALUE(hemp_type_text_boolean);
HEMP_VALUE(hemp_method_text_length);


/*--------------------------------------------------------------------------
 * macros
 *--------------------------------------------------------------------------*/

#define hemp_text_init(text)                            \
    hemp_text_init_size(text, 0)

#define hemp_text_new()                                 \
    hemp_text_init(NULL)

#define hemp_text_new_size(size)                        \
    hemp_text_init_size(NULL, size)

#define hemp_text_new_format(format, ...)               \
    hemp_text_init_format(NULL, format, __VA_ARGS__)

#define hemp_text_append_string(text, append)           \
    hemp_text_append_stringn(text, append, strlen(append))


#endif /* HEMP_TEXT_H */
