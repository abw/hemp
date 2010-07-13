#ifndef HEMP_TEXT_H
#define HEMP_TEXT_H

#include <stdio.h>
#include <string.h>
#include "hemp/memory.h"


/*--------------------------------------------------------------------------
 * data structures
 *--------------------------------------------------------------------------*/

struct hemp_text {
    hemp_cstr_t string;
    hemp_size_t capacity;
    hemp_size_t length;
}; 


/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

hemp_text_t hemp_text_init(hemp_size_t size);
void        hemp_text_free(hemp_text_t text);
hemp_text_t hemp_text_from_text(hemp_text_t text);
hemp_text_t hemp_text_from_cstr(hemp_cstr_t cstr);
hemp_text_t hemp_text_append_text(hemp_text_t text, hemp_text_t append);
hemp_text_t hemp_text_append_cstr(hemp_text_t text, hemp_cstr_t append);
hemp_text_t hemp_text_insert_cstr(hemp_text_t text, hemp_offset_t offset, hemp_cstr_t insert);

/*
int hemp_string_equals(HEMP_STRING a, HEMP_STRING b);
unsigned int hemp_string_hash(HEMP_STRING s);
*/

/*--------------------------------------------------------------------------
 * macros
 *--------------------------------------------------------------------------*/

#define hemp_text_null(t)       \
    hemp_text_free(t);          \
    t = NULL;

#define TEST_hemp_text_set_capacity(text, size)     \
    (text->memory = text->memory                    \
        ? hemp_memory_resize(text->memory, size)    \
        : hemp_memory_acquire(length + 1))          


#endif /* HEMP_TEXT_H */
