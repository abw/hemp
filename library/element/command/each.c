#include <hemp/element.h>


HEMP_ELEMENT(hemp_element_command_each);
HEMP_PREFIX(hemp_element_command_each_prefix);
HEMP_POSTFIX(hemp_element_command_each_postfix);
HEMP_OUTPUT(hemp_element_command_each_text);
HEMP_OUTPUT(hemp_element_command_each_text_item);


/*--------------------------------------------------------------------------
 * each <items> <expr>
 *--------------------------------------------------------------------------*/

HEMP_ELEMENT(hemp_element_command_each) {
    hemp_debug_msg("hemp_element_command_each()\n");
    hemp_element_literal(hemp, element);
    element->parse_prefix    = &hemp_element_command_each_prefix;
    element->parse_postfix   = &hemp_element_command_each_postfix;
    element->text            = &hemp_element_command_each_text;
//    element->cleanup         = &hemp_element_command_each_cleanup;
    element->value           = &hemp_element_text_value;
    element->number          = &hemp_element_value_number;
    element->integer         = &hemp_element_value_integer;
    element->boolean         = &hemp_element_value_boolean;
    element->compare         = &hemp_element_value_compare;
    element->flags           = HEMP_BE_SOURCE;
    return element;
}


HEMP_PREFIX(hemp_element_command_each_prefix) {
    hemp_debug_msg("hemp_element_command_each_prefix()\n");

    HempFragment fragment = *fragptr;

    HEMP_PREFIX_PRECEDENCE;
    hemp_set_flag(fragment, HEMP_BE_PREFIX);

    /* skip keyword */
    hemp_advance(fragptr);

    /* parse an expression */
    hemp_parse_lhs_expr_rprec(fragment);

    /* parse a body expression, checking for relevant terminator */
    hemp_parse_rhs_body_terminated(fragment);

    return fragment;
}


HEMP_POSTFIX(hemp_element_command_each_postfix) {
    hemp_debug_msg("hemp_element_command_each_postfix()\n");

    HempFragment fragment = *fragptr;

    HEMP_INFIX_LEFT_PRECEDENCE;
    hemp_set_flag(fragment, HEMP_BE_INFIX);

    /* expression on lhs becomes the body */
    hemp_set_rhs_fragment(fragment, lhs);

    /* skip past the keyword */
    hemp_advance(fragptr);

    /* parse expr into lhs */
    hemp_parse_lhs_expr_rprec(fragment);

    return hemp_parse_postfix(
        fragptr, scope, precedence, 0,
        fragment
    );
}


HEMP_OUTPUT(hemp_element_command_each_text) {
    hemp_debug_call("hemp_element_each_text()\n");
    Hemp         hemp     = context->hemp;
    HempFragment fragment = hemp_val_frag(value);
    HempValue    items    = hemp_lhs(fragment);
    HempValue    body     = hemp_rhs(fragment);
    HempValue    lhs      = hemp_call(items, value, context);
    HempText     text;
    HempList     list;

    hemp_prepare_text(context, output, text);
    hemp_debug_msg("TODO: hemp_element_each_text()\n");

    // quick hack to get something working
    hemp_debug_msg("LHS: %s\n", hemp_type_name(lhs));

    if (! hemp_is_list(lhs)) {
        hemp_debug_msg("TODO: proper iterators so we can handle things other than lists\n");
        hemp_text_append_string(text, "<TODO>");
        return output;
    }

    list = hemp_val_list(items);

    return output;
}


