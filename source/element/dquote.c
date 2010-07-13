#include "hemp/elements.h"
#include "hemp/debug.h"


static struct hemp_etype
    hemp_element_dquote = { 
        "dquote",
        &hemp_element_text_text
    };


hemp_etype_t HempElementDQuote = &hemp_element_dquote;

