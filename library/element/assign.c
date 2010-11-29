#include <math.h>
#include <hemp/element.h>


/*--------------------------------------------------------------------------
 * assignment operators
 *--------------------------------------------------------------------------*/

HEMP_SYMBOL(hemp_element_assign_symbol) {
    hemp_element_infix_right_symbol(hemp, symbol);
    symbol->text   = &hemp_element_assign_text;
    symbol->value  = &hemp_element_assign_value;
    symbol->params = &hemp_element_assign_params;
    return symbol;
}


HEMP_OUTPUT_FUNC(hemp_element_assign_text) {
    hemp_debug_call("hemp_element_assign_text()\n");
    hemp_element_assign_value(value, context);
    hemp_text text;
    hemp_prepare_text(context, output, text);
    return output;
}


HEMP_VALUE_FUNC(hemp_element_assign_value) {
    hemp_debug_msg("hemp_element_assign_value()\n");
    hemp_element  element = hemp_val_elem(value);
    hemp_value    lhs     = hemp_lhs(element);
    hemp_value    rhs     = hemp_rhs(element);
    hemp_debug_msg("assign lhs: %s\n", hemp_type_name(lhs));
    return hemp_obcall(lhs, assign, context, rhs);

/*
Old way - pre-evaluated value

    hemp_value    rhs     = hemp_rhs(element);
    hemp_value    rval    = hemp_call(rhs, value, context);
//    hemp_debug(
//        "calling assign on %s: %p\n", 
//        hemp_val_elem(lhs)->type->name, 
//        hemp_val_elem(lhs)->type->assign
//    );
    return hemp_obcall(lhs, assign, context, rval);
*/
}


HEMP_OUTPUT_FUNC(hemp_element_assign_params) {
    hemp_debug_call("hemp_element_assign_params()\n");
    hemp_element_assign_value(value, context);

    /* create a new output list on the off chance we weren't passed one */
    hemp_list  values;
    hemp_prepare_values(context, output, values);
    return output;
}

