#include <hemp/element.h>


HEMP_ELEMENT(hemp_element_command_if);
HEMP_PREFIX(hemp_element_command_if_prefix);
HEMP_VALUE(hemp_element_command_if_value);
HEMP_OUTPUT(hemp_element_command_if_text);
HEMP_OUTPUT(hemp_element_command_if_values);
HEMP_CLEANUP(hemp_element_command_if_cleanup);


HEMP_ELEMENT(hemp_element_command_if) {
    hemp_debug_call("hemp_element_command_if()\n");
    element->parse_prefix    = &hemp_element_command_if_prefix;
    element->cleanup         = &hemp_element_command_if_cleanup;
    element->values          = &hemp_element_command_if_values;
    element->value           = &hemp_element_command_if_value;
    element->text            = &hemp_element_command_if_text;
    element->number          = &hemp_element_value_number;
    element->integer         = &hemp_element_value_integer;
    element->boolean         = &hemp_element_value_boolean;
    element->compare         = &hemp_element_value_compare;
    element->token           = &hemp_element_literal_text;
    element->source          = &hemp_element_literal_text;
    element->flags           = HEMP_BE_SOURCE;
    return element;
}


HEMP_PREFIX(hemp_element_command_if_prefix) {
    hemp_fragment fragment = *fragptr;
    hemp_element  element  = fragment->type;

    hemp_debug_call("hemp_element_command_if_prefix()\n");

    HEMP_PREFIX_PRECEDENCE;

    /* skip past the keyword */
    hemp_advance(fragptr);
    hemp_set_flag(fragment, HEMP_BE_PREFIX);

    // TODO: look for option #fragment

    hemp_parse_lhs_expr(fragment);
    hemp_fragment block = hemp_parse_rhs_body(fragment);

    if (hemp_not_flag(block, HEMP_BE_TERMINATED)) {
//        hemp_debug_msg("looking for terminator: %s\n", element->end);
        if (hemp_element_terminator_matches(*fragptr, element->end)) {
//            hemp_debug_msg("found matching terminator for %s => %s\n", element->start, element->end);
            hemp_advance(fragptr);
        }
        else {
            HEMP_THROW_NOEND(fragment);
        }
    }

    return hemp_parse_postfix(
        fragptr, scope, precedence, 0,
        fragment
    );
}


HEMP_VALUE(hemp_element_command_if_value) {
    hemp_debug_call("hemp_element_command_if_value()\n");

    hemp_fragment   fragment = hemp_val_frag(value);
    hemp_value      test     = hemp_lhs(fragment);
    hemp_value      block    = hemp_rhs(fragment);
    hemp_value      truth    = hemp_call(test, boolean, context);

    return hemp_is_true(truth)
        ? hemp_obcall(block, value, context)
        : HempNothing;
}


HEMP_OUTPUT(hemp_element_command_if_values) {
    hemp_debug_call("hemp_element_command_if_values()\n");

    hemp_fragment   fragment = hemp_val_frag(value);
    hemp_value      test     = hemp_lhs(fragment);
    hemp_value      block    = hemp_rhs(fragment);
    hemp_value      truth    = hemp_call(test, boolean, context);
    hemp_list       values;

    hemp_prepare_values(context, output, values);

    return hemp_is_true(truth)
        ? hemp_obcall(block, values, context, output)
        : output;
}


HEMP_OUTPUT(hemp_element_command_if_text) {
    hemp_debug_call("hemp_element_command_if_text()\n");

    hemp_fragment   fragment = hemp_val_frag(value);
    hemp_value      test     = hemp_lhs(fragment);
    hemp_value      block    = hemp_rhs(fragment);
    hemp_value      truth    = hemp_call(test, boolean, context);
    hemp_text       text;

    hemp_prepare_text(context, output, text);

    return hemp_is_true(truth)
        ? hemp_obcall(block, text, context, output)
        : output;
}


HEMP_CLEANUP(hemp_element_command_if_cleanup) {
    hemp_debug_call("hemp_element_command_if_cleanup(%p)\n", fragment);

}

