#include "hemp/elements.h"
#include "hemp/debug.h"


static struct hemp_etype
    hemp_element_dquote = { 
        "dquote",
        &hemp_element_dont_skip,
        &hemp_element_dont_skip,
        &hemp_element_dont_skip,
        &hemp_element_literal_parse_expr,
        &hemp_element_literal_text
    };


hemp_etype_t HempElementDQuote = &hemp_element_dquote;

