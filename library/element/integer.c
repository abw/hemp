#include <hemp/element.h>

struct hemp_symbol_s
    hemp_symbol_integer = { 
        "integer",
        "integer:",NULL,
        HEMP_IS_STATIC, 0, 0,
        NULL, NULL,
        &hemp_skip_none_vtable,
        NULL,
        NULL,
        &hemp_element_literal_text,
        &hemp_element_number_parse_expr
    };

hemp_symbol_p HempSymbolInteger = &hemp_symbol_integer;

