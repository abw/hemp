#include "hemp/elements.h"
#include "hemp/debug.h"


static struct hemp_etype
    hemp_element_squote = { 
        "squote",
        &hemp_element_text_text
    };


hemp_etype_t HempElementSQuote = &hemp_element_squote;

