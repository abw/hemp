#include <hemp/element.h>


/*--------------------------------------------------------------------------
 * number operators
 *--------------------------------------------------------------------------*/

HEMP_SYMBOL_FUNC(hemp_element_numop_plus_symbol) {
    // not quite right - needs to accept expr as well...
    symbol->expr   = &hemp_element_not_expr;
    symbol->infix  = &hemp_element_parse_infix_left;
    symbol->source = &hemp_element_literal_text;
    symbol->text   = &hemp_element_numop_text;
    symbol->number = &hemp_element_numop_plus_value;
    return symbol;
//    return hemp_element_numop_TODO_symbol(hemp, symbol);
}


HEMP_OUTPUT_FUNC(hemp_element_numop_text) {
    debug_call("hemp_element_numop_text()\n");

    static hemp_char_t buffer[HEMP_BUFFER_SIZE];

    hemp_text_p text;
    hemp_value_t value = element->type->number(element, context);

    if (HEMP_IS_INT(value))
        snprintf(buffer, HEMP_BUFFER_SIZE, "%d", HEMP_VAL_INT(value));
    else
        snprintf(buffer, HEMP_BUFFER_SIZE, "%lf", HEMP_VAL_NUM(value));

    hemp_prepare_output(output, text, strlen(buffer));
    hemp_text_append_cstr(text, buffer);

    return output;
}


HEMP_VALUE_FUNC(hemp_element_numop_plus_value) {
    debug_call("hemp_element_numop_plus_value()\n");

    struct hemp_binary_s exprs = element->args.binary;
    hemp_element_p lhs = exprs.lhs;
    hemp_element_p rhs = exprs.rhs;
    hemp_value_t lval  = lhs->type->number(lhs, context);
    hemp_value_t rval, result;

    debug("LHS value: ");
    hemp_dump_value(lval);

    if (HEMP_IS_INT(lval)) {
        debug("lhs is integer\n");
        rval   = rhs->type->integer(rhs, context);
        result = HEMP_INT_VAL(
            HEMP_VAL_INT(lval) + HEMP_VAL_INT(rval)
        );
    }
    else {
        debug("lhs is number\n");
        rval   = rhs->type->number(rhs, context);
        if (HEMP_IS_INT(rval)) {
            debug("RHS value: ");
            hemp_dump_value(rval);
            debug("recasting rhs integer to float\n");
            rval = HEMP_NUM_VAL((hemp_num_t) HEMP_VAL_INT(rval));
        }
        result = HEMP_NUM_VAL(
            HEMP_VAL_NUM(lval) + HEMP_VAL_NUM(rval)
        );
    }

    debug("RHS value: ");
    hemp_dump_value(rval);

    debug("Result: ");
    hemp_dump_value(result);

    return result;
}


