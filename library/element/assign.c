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

HEMP_ETEXT_FUNC(hemp_element_assign_text) {
    hemp_debug("hemp_element_assign_text()\n");
    hemp_element_assign_value(element, context);
    hemp_text_p text;
    hemp_prepare_text(context, output, text);
    return output;
}

HEMP_EVAL_FUNC(hemp_element_assign_value) {
    hemp_debug("hemp_element_assign_value()\n");

    hemp_element_p lhs = hemp_lhs_element(element);
    hemp_element_p rhs = hemp_rhs_element(element);
    hemp_value_t rval  = rhs->type->value(rhs, context);
    return lhs->type->assign(lhs, context, rval);
}

