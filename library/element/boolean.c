#include <math.h>
#include <hemp/element.h>


/*--------------------------------------------------------------------------
 * boolean operators
 *--------------------------------------------------------------------------*/

HEMP_SYMBOL_FUNC(hemp_element_boolean_symbol) {
    symbol->expr    = &hemp_element_not_expr;
    symbol->infix   = &hemp_element_parse_infix_left;
    symbol->source  = &hemp_element_binary_source;
    symbol->text    = &hemp_element_boolean_text;
    symbol->number  = &hemp_element_not_number;
    symbol->integer = &hemp_element_not_integer;
    symbol->boolean = &hemp_element_not_boolean;   /* symbols redefine this */
    symbol->compare = &hemp_element_not_compare;
    return symbol;
}


HEMP_OUTPUT_FUNC(hemp_element_boolean_text) {
    hemp_debug_call("hemp_element_boolean_text()\n");

    hemp_text_p text;
    hemp_prepare_output(output, text, 10);      // FIXME
    
//  hemp_debug("boolean output for %s\n", element->type->name);

    hemp_text_append_cstr(
        text, 
        hemp_is_true( element->type->boolean(element, context) )
            ? HEMP_STR_TRUE
            : HEMP_STR_FALSE
    );

    return output;
}


/*--------------------------------------------------------------------------
 * not
 *--------------------------------------------------------------------------*/

HEMP_SYMBOL_FUNC(hemp_element_boolean_not_symbol) {
    hemp_element_boolean_symbol(HEMP_SYMBOL_ARG_NAMES);
    symbol->expr    = &hemp_element_parse_prefix;
    symbol->infix   = &hemp_element_not_infix;
    symbol->boolean = &hemp_element_boolean_not_value;
    return symbol;
}


HEMP_VALUE_FUNC(hemp_element_boolean_not_value) {
    hemp_debug_call("hemp_element_boolean_not_value()\n");

    hemp_element_p expr = element->args.unary.expr;
    hemp_value_t   val  = expr->type->boolean(expr, context);

    return hemp_is_true(val)
        ? HempFalse
        : HempTrue;
}


/*--------------------------------------------------------------------------
 * and
 *--------------------------------------------------------------------------*/

HEMP_SYMBOL_FUNC(hemp_element_boolean_and_symbol) {
    hemp_element_boolean_symbol(HEMP_SYMBOL_ARG_NAMES);
    symbol->boolean = &hemp_element_boolean_and_value;
    return symbol;
}


HEMP_VALUE_FUNC(hemp_element_boolean_and_value) {
    hemp_debug_call("hemp_element_boolean_and_value()\n");

    hemp_element_p lhs = element->args.binary.lhs;
    hemp_element_p rhs = element->args.binary.rhs;
    hemp_value_t lval  = lhs->type->boolean(lhs, context);
    hemp_value_t rval  = rhs->type->boolean(rhs, context);

    return (hemp_is_true(lval) && hemp_is_true(rval))
        ? HempTrue
        : HempFalse;
}


/*--------------------------------------------------------------------------
 * or
 *--------------------------------------------------------------------------*/

HEMP_SYMBOL_FUNC(hemp_element_boolean_or_symbol) {
    hemp_element_boolean_symbol(HEMP_SYMBOL_ARG_NAMES);
    symbol->boolean = &hemp_element_boolean_or_value;
    return symbol;
}


HEMP_VALUE_FUNC(hemp_element_boolean_or_value) {
    hemp_debug_call("hemp_element_boolean_or_value()\n");

    hemp_element_p lhs = element->args.binary.lhs;
    hemp_element_p rhs = element->args.binary.rhs;
    hemp_value_t lval  = lhs->type->boolean(lhs, context);
    hemp_value_t rval  = rhs->type->boolean(rhs, context);

    return (hemp_is_true(lval) || hemp_is_true(rval))
        ? HempTrue
        : HempFalse;
}


