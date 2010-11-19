#include <math.h>
#include <hemp/element.h>


/*--------------------------------------------------------------------------
 * assignment operators
 *--------------------------------------------------------------------------*/

HEMP_SYMBOL_FUNC(hemp_element_assign_symbol) {
    hemp_element_infix_right_symbol(hemp, symbol);
    symbol->text   = &hemp_element_assign_text;
    symbol->value  = &hemp_element_assign_value;
    symbol->params = &hemp_element_assign_params;
    return symbol;
}


HEMP_OUTPUT_FUNC(hemp_element_assign_text) {
    hemp_debug_call("hemp_element_assign_text()\n");
    hemp_element_assign_value(value, context);
    hemp_text_p text;
    hemp_prepare_text(context, output, text);
    return output;
}


HEMP_VALUE_FUNC(hemp_element_assign_value) {
    hemp_debug_call("hemp_element_assign_value()\n");
    hemp_element_p  element = hemp_val_elem(value);
    hemp_value_t    lhs     = hemp_lhs(element);
    hemp_value_t    rhs     = hemp_rhs(element);
    hemp_value_t    rval    = hemp_call(rhs, value, context);
//    hemp_debug(
//        "calling assign on %s: %p\n", 
//        hemp_val_elem(lhs)->type->name, 
//        hemp_val_elem(lhs)->type->assign
//    );
    return hemp_obcall(lhs, assign, context, rval);
}


HEMP_OUTPUT_FUNC(hemp_element_assign_params) {
    hemp_debug_call("hemp_element_assign_params()\n");
    hemp_element_assign_value(value, context);

    /* create a new output list on the off chance we weren't passed one */
    hemp_list_p  values;
    hemp_prepare_values(context, output, values);
    return output;
}

