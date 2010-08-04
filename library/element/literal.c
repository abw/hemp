#include <hemp/element.h>


hemp_element_p
hemp_element_literal_parse_expr(
    HEMP_PARSE_PROTO
) {
    debug_call("hemp_element_literal_parse_expr()\n");
    
    /* Advance the pointer to the next element after this one and return the 
     * current element as the yielded expression.
     */
    hemp_element_p element = *elemptr;
    
    if (hemp_has_next(elemptr))
        hemp_go_next(elemptr);

    return element;
}


hemp_text_p
hemp_element_literal_text(
    hemp_element_p  element,
    hemp_text_p     text
) {
    debug_call("hemp_element_literal_text()\n");

    if (! text)
        text = hemp_text_init(element->length);
    
    hemp_text_append_cstrn(text, element->token, element->length);

    return text;
}


hemp_text_p
hemp_element_quoted_text(
    hemp_element_p  element,
    hemp_text_p     text
) {
    debug_call("hemp_element_quoted_text()\n");

    if (! text)
        text = hemp_text_init(element->length);

    if (hemp_has_flag(element, HEMP_IS_FIXED)) {
        /* "pinch" in the ends of the token range to avoid the quotes */
        hemp_text_append_cstrn(text, element->token + 1, element->length - 2);
    }
    else {
        hemp_text_append_cstr(text, element->value.text);
    }

    return text;
}

