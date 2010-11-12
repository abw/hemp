#include <hemp/element.h>


HEMP_SYMBOL_FUNC(hemp_element_literal_symbol) {
    symbol->token   = &hemp_element_literal_token;
    symbol->source  = &hemp_element_literal_source;
    symbol->text    = &hemp_element_literal_text;
    symbol->value   = &hemp_element_literal_value;
    symbol->fixed   = &hemp_element_fixed;
    return symbol;
}


HEMP_PREFIX_FUNC(hemp_element_literal_prefix) {
    hemp_debug_call(
        "hemp_element_number_prefix() precedence is %d, parg: %d\n", 
        (*elemptr)->type->lprec, precedence
    );

    hemp_element_p element = *elemptr;

    if (hemp_has_next(elemptr)) {
        hemp_go_next(elemptr);
//      hemp_skip_whitespace(elemptr);
        return hemp_parse_postfix(elemptr, scope, precedence, force, element);
    }

    return element;
}


HEMP_ETEXT_FUNC(hemp_element_literal_token) {
    hemp_debug_call("hemp_element_literal_token()\n");
    hemp_text_p text;
    hemp_prepare_output(output, text, element->length);
    hemp_text_append_stringn(text, element->token, element->length);
    return output;
}


HEMP_ETEXT_FUNC(hemp_element_literal_source) {
    hemp_debug_call("hemp_element_literal_source()\n");
    hemp_text_p text;
    hemp_prepare_output(output, text, element->length);
    hemp_text_append_stringn(text, element->token, element->length);
    return output;
}


HEMP_ETEXT_FUNC(hemp_element_literal_text) {
    hemp_debug_call(
        "hemp_element_literal_text(%p) [%s]\n", 
        element, element->type->name
    );

    /* TODO: merge this with hemp_element_literal_source().  They were 
     * different but are now the same
     */
    hemp_text_p text;
    hemp_prepare_output(output, text, element->length);
    hemp_text_append_stringn(text, element->token, element->length);
    return output;
}


HEMP_EVAL_FUNC(hemp_element_literal_value) {
    return hemp_element_literal_text(HEMP_EVAL_ARG_NAMES, HempNothing);
}


