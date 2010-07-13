#include "hemp/elements.h"
#include "hemp/debug.h"


static struct hemp_etype
    hemp_element_integer = { 
        "integer",
        &hemp_element_skip_space,
        &hemp_literal_parse_expr,
        &hemp_element_text_text
    };


hemp_etype_t HempElementInteger = &hemp_element_integer;

