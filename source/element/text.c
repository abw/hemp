#include "hemp/elements.h"
#include "hemp/debug.h"


static struct hemp_etype
    hemp_element_text = { 
        "text",
        &hemp_element_text_text
    };


hemp_etype_t HempElementText = &hemp_element_text;

