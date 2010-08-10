#include <hemp/element.h>


HEMP_PARSE_FUNC(hemp_element_literal_expr) {
    debug_call("hemp_element_literal_parse_expr()\n");
    
    /* Advance the pointer to the next element after this one and return the 
     * current element as the yielded expression.
     */
    hemp_element_p element = *elemptr;
    
    if (hemp_has_next(elemptr))
        hemp_go_next(elemptr);

    return element;
}


HEMP_OUTPUT_FUNC(hemp_element_literal_source) {
    debug_call("hemp_element_literal_source(%p) [%s]\n", element, element->type->name);

    hemp_text_p text;
    hemp_prepare_output(output, text, element->length);

    if (hemp_has_flag(element, HEMP_IS_FIXED)) {
        hemp_symbol_p symbol = element->type;
        hemp_size_t slen     = symbol->start ? strlen(symbol->start) : 0;
        hemp_size_t elen     = symbol->end   ? strlen(symbol->end)   : 0;
//      debug("pinching string\n");
        /* "pinch" in the ends of the token range to avoid the quotes */
        hemp_text_append_cstrn(
            text, 
            element->token  + slen,         /* skip past start              */
            element->length - slen - elen   /* shorten by start/end lengths */
        );
    }
    else {
        debug("de-referencing dynamic string at %p", HEMP_VAL_STR(element->args.value));
        hemp_text_append_cstr(text, HEMP_VAL_STR(element->args.value));
    }

    return output;
}


HEMP_OUTPUT_FUNC(hemp_element_literal_text) {
    debug_call("hemp_element_literal_text()\n");

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


