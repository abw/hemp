#include "hemp/elements.h"
#include "hemp/debug.h"


static struct hemp_etype
    hemp_element_squote = { 
        "squote",
        &hemp_element_dont_skip,         /* not skippable as whitespace... */
        &hemp_element_dont_skip,         /* ...delimiter...                */
        &hemp_element_dont_skip,         /* ...or separator                */
        &hemp_element_literal_parse_expr,
        &hemp_element_quoted_text
    };


hemp_etype_t HempElementSQuote = &hemp_element_squote;

