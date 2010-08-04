#include <hemp/element.h>


static struct hemp_etype_s
    hemp_element_integer = { 
        "integer",
        HEMP_IS_STATIC,
        NULL,
        &hemp_element_dont_skip,
        &hemp_element_dont_skip,
        &hemp_element_dont_skip,
        &hemp_element_literal_parse_expr,
        &hemp_element_literal_text
    };


hemp_etype_p HempElementInteger = &hemp_element_integer;

