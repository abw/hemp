#include <hemp/elements.h>


static struct hemp_etype_s
    hemp_element_dquote = { 
        "dquote",
        0,
        &hemp_element_text_clean,
        &hemp_element_dont_skip,
        &hemp_element_dont_skip,
        &hemp_element_dont_skip,
        &hemp_element_literal_parse_expr,
        &hemp_element_quoted_text
    };


hemp_etype_p HempElementDQuote = &hemp_element_dquote;

