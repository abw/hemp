#include <hemp/element.h>


HEMP_ELEMENT(hemp_element_command_with);
HEMP_PREFIX(hemp_element_command_with_prefix);
HEMP_POSTFIX(hemp_element_command_with_postfix);
HEMP_OUTPUT(hemp_element_command_with_text);
HEMP_CLEANUP(hemp_element_command_with_cleanup);

HempHash
hemp_element_command_with_params_hash(
    HempFragment   fragment,
    HempContext    context
);


/*--------------------------------------------------------------------------
 * with <params> <expr>
 *--------------------------------------------------------------------------*/

HEMP_ELEMENT(hemp_element_command_with) {
    hemp_debug_call("hemp_element_command_with()\n");
    hemp_element_literal(hemp, element);
    element->parse_prefix    = &hemp_element_command_with_prefix;
    element->parse_postfix   = &hemp_element_command_with_postfix;
    element->text            = &hemp_element_command_with_text;
    element->cleanup         = &hemp_element_command_with_cleanup;
    element->value           = &hemp_element_text_value;
    element->number          = &hemp_element_value_number;
    element->integer         = &hemp_element_value_integer;
    element->boolean         = &hemp_element_value_boolean;
    element->compare         = &hemp_element_value_compare;
    element->flags           = HEMP_BE_SOURCE;
    return element;
}


HEMP_PREFIX(hemp_element_command_with_prefix) {
    hemp_debug_call("hemp_element_command_with_prefix()\n");

    HempFragment fragment = *fragptr;

    HEMP_PREFIX_PRECEDENCE;
    hemp_set_flag(fragment, HEMP_BE_PREFIX);

    /* skip keyword */
    hemp_advance(fragptr);

    /* parse a list of zero or more parameter pairs */
    hemp_parse_lhs_pairs_rprec(fragment);

    /* parse a body expression, checking for relevant terminator */
    hemp_parse_rhs_body_terminated(fragment);

    return fragment;
}


HEMP_POSTFIX(hemp_element_command_with_postfix) {
    hemp_debug_call("hemp_element_command_with_postfix()\n");

    HempFragment fragment = *fragptr;

    HEMP_INFIX_LEFT_PRECEDENCE;
    hemp_set_flag(fragment, HEMP_BE_INFIX);

    /* expression on lhs becomes the body */
    hemp_set_rhs_fragment(fragment, lhs);

    /* skip past the keyword */
    hemp_advance(fragptr);

    /* parse params into lhs */
    hemp_parse_lhs_pairs_lprec(fragment);

    return hemp_parse_postfix(
        fragptr, scope, precedence, 0,
        fragment
    );
}


HEMP_OUTPUT(hemp_element_command_with_text) {
    hemp_debug_call("hemp_element_with_text()\n");
    Hemp     hemp     = context->hemp;
    HempFragment fragment = hemp_val_frag(value);
    HempHash     params   = hemp_element_command_with_params_hash(fragment, context);
    HempValue    body     = hemp_rhs(fragment);

    hemp_context_with(context, params);
    
    HEMP_TRY;
        output = hemp_vtext(body, context, output);
    HEMP_CATCH_ALL;
        hemp_context_without(context, NULL);
        HEMP_RETHROW;
    HEMP_END;

    hemp_context_without(context, NULL);

    return output;
}


/* TODO: separate methods for value/values */


HEMP_CLEANUP(hemp_element_command_with_cleanup) {
    hemp_debug_call("hemp_element_command_with_clean(%p)\n", fragment);
    HempList params = hemp_val_list(hemp_lhs(fragment));
    hemp_list_free(params);
}


HempHash
hemp_element_command_with_params_hash(
    HempFragment    fragment,
    HempContext     context
) {
    HempList     params   = hemp_val_list(hemp_lhs(fragment));
    HempHash     hash     = hemp_context_tmp_hash(context);
    HempValue    vars     = hemp_hash_val(hash);
    HempValue    param;
    HempSize     n;

    for (n = 0; n < params->length; n++) {
        param = hemp_list_item(params, n);
        hemp_call(param, pairs, context, vars);
    }

    return hash;
}
