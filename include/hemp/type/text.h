#ifndef HEMP_TEXT_H
#define HEMP_TEXT_H

#include <hemp/core.h>


/*--------------------------------------------------------------------------
 * type definitions
 *--------------------------------------------------------------------------*/

struct hemp_text {
    hemp_string     string;
    hemp_size       capacity;
    hemp_size       length;
}; 


/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

hemp_text 
hemp_text_init_size(
    hemp_text       text, 
    hemp_size       capacity
);

hemp_text 
hemp_text_init_format(
    hemp_text       text, 
    hemp_string     format, 
    ...
);

void
hemp_text_free(
    hemp_text       text
);

hemp_text
hemp_text_from_text(
    hemp_text       text
);

hemp_text
hemp_text_from_string(
    hemp_string     string
);

hemp_text
hemp_text_append_text(
    hemp_text       text, 
    hemp_text       append
);

hemp_text
hemp_text_append_string(
    hemp_text       text, 
    hemp_string     append
);

hemp_text
hemp_text_append_stringn(
    hemp_text       text, 
    hemp_string     append, 
    hemp_size       length
);

hemp_text
hemp_text_insert_string(
    hemp_text       text, 
    hemp_offset     position, 
    hemp_string     insert
);

hemp_text
hemp_text_replace_string(
    hemp_text       text, 
    hemp_string     replace
);

hemp_text
hemp_text_truncate(
    hemp_text       text, 
    hemp_pos        length
);


void     
hemp_text_release(
    hemp_text       text
);


/*--------------------------------------------------------------------------
 * type functions and methods
 *--------------------------------------------------------------------------*/

HEMP_TYPE_FUNC(hemp_type_text);
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
