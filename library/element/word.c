#include <hemp/element.h>


struct hemp_symbol_s
    hemp_symbol_word = { 
        "word",
        "word:",
        0, 0, 0,   /* HEMP_IS_FIXED?   Is this for source regeneration? */
        NULL, NULL,
        &hemp_skip_none_vtable,
        NULL, NULL,
        &hemp_element_literal_text,
        &hemp_element_literal_parse_expr            // FIXME
    };

hemp_symbol_p HempSymbolWord = &hemp_symbol_word;

