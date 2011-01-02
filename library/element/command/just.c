#include <hemp/element.h>


HEMP_ELEMENT(hemp_element_command_just);
HEMP_OUTPUT(hemp_element_command_just_text);

HempHash
hemp_element_command_with_params_hash(
    HempFragment   fragment,
    HempContext    context
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
    Hemp     hemp     = context->hemp;
    HempFragment fragment = hemp_val_frag(value);
    HempHash     params   = hemp_element_command_with_params_hash(fragment, context);
    HempValue    body     = hemp_rhs(fragment);
    HempHash     saved;

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
