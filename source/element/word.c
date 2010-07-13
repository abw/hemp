#include "hemp/elements.h"
#include "hemp/debug.h"


static struct hemp_etype
    hemp_element_word = { 
        "word",
        &hemp_element_dont_skip,
        &hemp_element_dont_skip,
        &hemp_element_dont_skip,
        /* these aren't right, but they'll do for now, for testing purposes */
        &hemp_element_literal_parse_expr,
        &hemp_element_literal_text
    };

hemp_etype_t HempElementWord = &hemp_element_word;

