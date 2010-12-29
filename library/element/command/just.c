#include <hemp/element.h>


HEMP_ELEMENT(hemp_element_command_just);
HEMP_OUTPUT(hemp_element_command_just_text);

hemp_hash
hemp_element_command_with_params_hash(
    hemp_fragment   fragment,
    hemp_context    context
);


/*--------------------------------------------------------------------------
 * just <params> <expr>
 *--------------------------------------------------------------------------*/

HEMP_ELEMENT(hemp_element_command_just) {
    hemp_debug_call("hemp_element_command_just()\n");
    hemp_element_command_with(hemp, element);
    element->text = &hemp_element_command_just_text;
    return element;
}


HEMP_OUTPUT(hemp_element_command_just_text) {
    hemp_debug_call("hemp_element_just_text()\n");
    hemp_hemp     hemp     = context->hemp;
    hemp_fragment fragment = hemp_val_frag(value);
    hemp_hash     params   = hemp_element_command_with_params_hash(fragment, context);
    hemp_value    body     = hemp_rhs(fragment);
    hemp_hash     saved;

    saved = hemp_context_just(context, params);

    HEMP_TRY;
        output = hemp_vtext(body, context, output);
    HEMP_CATCH_ALL;
        hemp_context_without(context, saved);
        HEMP_RETHROW;
    HEMP_END;

    hemp_context_without(context, saved);

    return output;
}

/* TODO: separate methods for value/values */
