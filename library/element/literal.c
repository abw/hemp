#include <hemp/element.h>


HEMP_SYMBOL(hemp_element_literal_symbol) {
    symbol->token       = &hemp_element_literal_token;
    symbol->source      = &hemp_element_literal_source;
    symbol->text        = &hemp_element_literal_text;
    symbol->value       = &hemp_element_literal_value;
    symbol->values      = &hemp_element_value_values;
    symbol->parse_fixed = &hemp_element_fixed;
    symbol->cleanup     = &hemp_element_literal_clean;
    return symbol;
}


HEMP_PREFIX_FUNC(hemp_element_literal_prefix) {
    hemp_debug_call(
        "hemp_element_number_prefix() precedence is %d, parg: %d\n", 
        (*elemptr)->type->lprec, precedence
    );

    hemp_element element = *elemptr;

    if (hemp_has_next(elemptr)) {
        hemp_go_next(elemptr);
//      hemp_skip_whitespace(elemptr);
        return hemp_parse_postfix(elemptr, scope, precedence, force, element);
    }

    return element;
}


HEMP_OUTPUT_FUNC(hemp_element_literal_token) {
    hemp_debug_call("hemp_element_literal_token()\n");
    hemp_text text;
    hemp_element element = hemp_val_elem(value);
    hemp_prepare_text_size(context, output, text, element->length);
    hemp_text_append_stringn(text, element->token, element->length);
    return output;
}


HEMP_OUTPUT_FUNC(hemp_element_literal_source) {
    hemp_debug_call("hemp_element_literal_source()\n");
    hemp_text text;
    hemp_element element = hemp_val_elem(value);
    hemp_prepare_text_size(context, output, text, element->length);
    hemp_text_append_stringn(text, element->token, element->length);
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
    hemp_text text;
    hemp_element element = hemp_val_elem(value);
    hemp_prepare_text_size(context, output, text, element->length);
    hemp_text_append_stringn(text, element->token, element->length);
    return output;
}


HEMP_VALUE_FUNC(hemp_element_literal_value) {
    return hemp_element_literal_text(value, context, HempNothing);
}


void
hemp_element_literal_clean(
    hemp_element element
) {
    hemp_debug_call("hemp_element_literal_clean(%p)\n", element);

    /* literal elements may be used as fixed values in which case they
     * have memory allocated (via hemp_element_fixed()) which we must 
     * clean up.
     */

    if (hemp_has_flag(element, HEMP_BE_ALLOCATED)) {
        hemp_mem_free(
            hemp_val_str( 
                hemp_expr(element)
            )
        );
    }
}

