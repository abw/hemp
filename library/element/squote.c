#include <hemp/elements.h>


static struct hemp_etype_s
    hemp_element_squote = { 
        "squote",
        HEMP_IS_STATIC,
        &hemp_element_text_clean,
        &hemp_element_dont_skip,         /* not skippable as whitespace... */
        &hemp_element_dont_skip,         /* ...delimiter...                */
        &hemp_element_dont_skip,         /* ...or separator                */
        &hemp_element_literal_parse_expr,
        &hemp_element_quoted_text
    };


hemp_etype_p HempElementSQuote = &hemp_element_squote;

