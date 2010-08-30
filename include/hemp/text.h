#ifndef HEMP_TEXT_H
#define HEMP_TEXT_H

#include <hemp/core.h>
// #include "hemp/type.h"


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

hemp_text_p hemp_text_new();
hemp_text_p hemp_text_init(hemp_size_t);
hemp_text_p hemp_text_init_format(hemp_str_p, ...);
void        hemp_text_free(hemp_text_p);
hemp_text_p hemp_text_from_text(hemp_text_p);
hemp_text_p hemp_text_from_string(hemp_str_p);
hemp_text_p hemp_text_append_text(hemp_text_p, hemp_text_p);
hemp_text_p hemp_text_append_string(hemp_text_p, hemp_str_p);
hemp_text_p hemp_text_append_stringn(hemp_text_p, hemp_str_p, hemp_size_t);
hemp_text_p hemp_text_insert_string(hemp_text_p, hemp_offset_t, hemp_str_p);
hemp_text_p hemp_text_replace_string(hemp_text_p, hemp_str_p);

/*
int hemp_string_equals(HEMP_STRING a, HEMP_STRING b);
unsigned int hemp_string_hash(HEMP_STRING s);
*/

HEMP_TYPE_METHOD(hemp_method_text_length);



/*--------------------------------------------------------------------------
 * macros
 *--------------------------------------------------------------------------*/

#define hemp_text_append_string(text, append)         \
    hemp_text_append_stringn(text, append, strlen(append))


#endif /* HEMP_TEXT_H */
