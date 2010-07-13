#include "hemp/elements.h"
#include "hemp/text.h"
#include "hemp/debug.h"


hemp_element_t
hemp_element_literal_parse_expr(
    HEMP_PARSE_PROTO
) {
    debug_call("hemp_element_literal_parse_expr()");
    
    /* Advance the pointer to the next element after this one and return the 
     * current element as the yielded expression.
     */
    hemp_element_t element = *elemptr;
    
    if (hemp_has_next(elemptr))
        hemp_go_next(elemptr);

    return element;
}


hemp_text_t
hemp_element_literal_text(
    hemp_element_t  element,
    hemp_text_t     text
) {
    debug_call("hemp_element_literal_text()\n");

    if (! text) {
        text = hemp_text_init(element->length);
        debug_mem("created new text buffer at %p\n", text);
    }
    
    hemp_text_append_cstrn(text, element->token, element->length);

    return text;
}

