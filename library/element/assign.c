#include <math.h>
#include <hemp/element.h>


/*--------------------------------------------------------------------------
 * assignment operators
 *--------------------------------------------------------------------------*/

HEMP_SYMBOL_FUNC(hemp_element_assign_symbol) {
    hemp_element_infix_right_symbol(hemp, symbol);
    symbol->text  = &hemp_element_assign_text;
    symbol->value = &hemp_element_assign_value;
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
    hemp_value_t    rval    = hemp_obcall(rhs, value, context);
    return hemp_obcall(lhs, assign, context, rval);
}

