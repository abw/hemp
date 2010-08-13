#include <hemp/element.h>


HEMP_PARSE_FUNC(hemp_element_literal_expr) {
    hemp_debug_call("hemp_element_literal_parse_expr()\n");
    
    /* Advance the pointer to the next element after this one and return the 
     * current element as the yielded expression.
     */
    hemp_element_p element = *elemptr;
    
    if (hemp_has_next(elemptr))
        hemp_go_next(elemptr);

    hemp_skip_whitespace(elemptr);

    return hemp_parse_infix(
        elemptr, scope, precedence, 0,
        element
    );
}


HEMP_OUTPUT_FUNC(hemp_element_literal_source) {
    hemp_debug_call("hemp_element_literal_source()\n");
    hemp_text_p text;
    hemp_prepare_output(output, text, element->length);
    hemp_text_append_cstrn(text, element->token, element->length);
    return output;
}


HEMP_OUTPUT_FUNC(hemp_element_literal_text) {
    hemp_debug_call(
        "hemp_element_literal_text(%p) [%s]\n", 
        element, element->type->name
    );

    /* TODO: merge this with hemp_element_literal_source().  They were 
     * different but are now the same
     */
    hemp_text_p text;
    hemp_prepare_output(output, text, element->length);
    hemp_text_append_cstrn(text, element->token, element->length);
    return output;
}


HEMP_VALUE_FUNC(hemp_element_literal_number) {
    hemp_todo("hemp_element_literal_number()");
    return HempNothing;
}


HEMP_VALUE_FUNC(hemp_element_literal_integer) {
    hemp_todo("hemp_element_literal_integer()");
    return HempNothing;
}


HEMP_VALUE_FUNC(hemp_element_literal_boolean) {
    hemp_todo("hemp_element_literal_boolean()");
    return HempNothing;
}


