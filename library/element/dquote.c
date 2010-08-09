#include <hemp/element.h>


struct hemp_symbol_s
    hemp_symbol_dquote = { 
        "dquote",
        "dquote:",NULL,
//      HEMP_IS_STATIC, 0, 0,
        0, 0, 0,
        NULL,
        (hemp_clean_f) &hemp_element_text_clean,
        &hemp_skip_none_vtable,
        NULL, NULL,
        &hemp_element_quoted_text,
        &hemp_element_literal_parse_expr
    };

hemp_symbol_p HempSymbolDQuote = &hemp_symbol_dquote;

