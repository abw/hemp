#ifndef HEMP_TEXT_H
#define HEMP_TEXT_H

#include <hemp/core.h>


/*--------------------------------------------------------------------------
 * data structures
 *--------------------------------------------------------------------------*/

struct hemp_text_s {
    hemp_str_p  string;
    hemp_size_t capacity;
    hemp_size_t length;
}; 


/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

//hemp_text_p hemp_text_new();
//hemp_text_p hemp_text_init(hemp_size_t);
hemp_text_p hemp_text_init_size(hemp_text_p, hemp_size_t);
hemp_text_p hemp_text_init_format(hemp_text_p, hemp_str_p, ...);
void        hemp_text_free(hemp_text_p);
hemp_text_p hemp_text_from_text(hemp_text_p);
hemp_text_p hemp_text_from_string(hemp_str_p);
hemp_text_p hemp_text_append_text(hemp_text_p, hemp_text_p);
hemp_text_p hemp_text_append_string(hemp_text_p, hemp_str_p);
hemp_text_p hemp_text_append_stringn(hemp_text_p, hemp_str_p, hemp_size_t);
hemp_text_p hemp_text_insert_string(hemp_text_p, hemp_offset_t, hemp_str_p);
hemp_text_p hemp_text_replace_string(hemp_text_p, hemp_str_p);
void        hemp_text_release(hemp_text_p);

/*
int hemp_string_equals(HEMP_STRING a, HEMP_STRING b);
unsigned int hemp_string_hash(HEMP_STRING s);
*/


/*--------------------------------------------------------------------------
 * type functions and methods
 *--------------------------------------------------------------------------*/

HEMP_TYPE_FUNC(hemp_type_text);
HEMP_VTEXT_FUNC(hemp_value_text_text);
HEMP_VALUE_FUNC(hemp_value_text_number);
HEMP_VALUE_FUNC(hemp_value_text_integer);
HEMP_VALUE_FUNC(hemp_value_text_boolean);
HEMP_VALUE_FUNC(hemp_method_text_length);


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

#define hemp_text_allocate(text) ({                     \
    if (! text) {                                       \
        text = (hemp_text_p) hemp_mem_alloc(            \
            sizeof(struct hemp_text_s)                  \
        );                                              \
        if (! text)                                     \
            hemp_mem_fail("text");                      \
    }                                                   \
})



#define hemp_text_append_string(text, append)         \
    hemp_text_append_stringn(text, append, strlen(append))


#endif /* HEMP_TEXT_H */
