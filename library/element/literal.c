#include <hemp/element.h>


HEMP_ELEMENT(hemp_element_literal) {
    element->parse_fixed = &hemp_element_parse_fixed;
    element->token       = &hemp_element_literal_text;
    element->source      = &hemp_element_literal_text;
    element->text        = &hemp_element_literal_text;
    element->value       = &hemp_element_literal_value;
    element->cleanup     = &hemp_element_literal_cleanup;
    return element;
}


HEMP_PREFIX(hemp_element_literal_prefix) {
    hemp_debug_call("hemp_element_literal_prefix()\n"); 
    hemp_fragment fragment = *fragptr;

    return hemp_advance(fragptr)
        ? hemp_parse_postfix(fragptr, scope, precedence, force, fragment)
        : fragment;
}



HEMP_OUTPUT(hemp_element_literal_text) {
    hemp_debug_call("hemp_element_literal_text()\n");

    /* TODO: merge this with hemp_element_element().  They were 
     * different but are now the same
     */
    hemp_text text;
    hemp_fragment fragment = hemp_val_frag(value);
    hemp_prepare_text_size(context, output, text, fragment->length);
    hemp_text_append_stringn(text, fragment->token, fragment->length);
    return output;
}


HEMP_VALUE(hemp_element_literal_value) {
    return hemp_element_literal_text(value, context, HempNothing);
}


HEMP_CLEANUP(hemp_element_literal_cleanup) {
    hemp_debug_call("hemp_element_literal_cleanup(%p)\n", element);

    /* literal elements may be used as fixed values in which case they
     * have memory allocated (via hemp_element_parse_fixed()) which we must 
     * clean up.
     */

    if (hemp_has_flag(fragment, HEMP_BE_ALLOCATED)) {
        hemp_mem_free(
            hemp_val_str( 
                hemp_expr(fragment)
            )
        );
    }
}

