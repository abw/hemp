#include <math.h>
#include <hemp/element.h>


/*--------------------------------------------------------------------------
 * boolean operators
 *
 * The "base class" boolean symbol role is a "sub-class" of a binary operator
 * symbol.  It patches in a value() "method" which delegates to an element's
 * boolean() method.  Each boolean operator should then define a boolean()
 * method to Do The Right Thing[tm].  Note that we initially patch the 
 * boolean() method to throw an error just in case someone (i.e. me) forgets
 * to define a boolean() method for an operator.  This makes the mistake easy
 * to spot and fix rather than having the value() and boolean() methods call 
 * each other in a deadly embrace until the universe grows cold.
 *--------------------------------------------------------------------------*/

HEMP_SYMBOL_FUNC(hemp_element_boolean_symbol) {
    hemp_element_infix_symbol(hemp, symbol);
    symbol->value   = &hemp_element_boolean_value;
    symbol->boolean = &hemp_element_not_boolean;
    return symbol;
}

HEMP_EVAL_FUNC(hemp_element_boolean_value) {
    hemp_debug_call("hemp_element_boolean_value()\n");
    return element->type->boolean(element, context);
}



/*--------------------------------------------------------------------------
 * not
 *
 * The exception that proves the rule: 'not' is a prefix operator rather 
 * than an infix/postfix operator, so we have to unpick some of the work we
 * did in the call to hemp_element_(boolean|infix)_symbol().  Other than
 * that, it's just a case of patching in the boolean() evaluation function.
 *--------------------------------------------------------------------------*/

HEMP_SYMBOL_FUNC(hemp_element_boolean_not_symbol) {
    hemp_element_boolean_symbol(HEMP_SYMBOL_ARG_NAMES);
    symbol->prefix  = &hemp_element_parse_prefix;
    symbol->postfix = NULL;
    symbol->boolean = &hemp_element_boolean_not_value;
    return symbol;
}


HEMP_EVAL_FUNC(hemp_element_boolean_not_value) {
    hemp_debug_call("hemp_element_boolean_not_value()\n");

    hemp_element_p expr = hemp_expr_element(element);
    hemp_value_t   val  = expr->type->boolean(expr, context);

    return hemp_is_true(val)
        ? HempFalse
        : HempTrue;
}


/*--------------------------------------------------------------------------
 * and
 *
 * Standard binary boolean operator - just need to patch in the boolean() 
 * evaluation method.
 *--------------------------------------------------------------------------*/

HEMP_SYMBOL_FUNC(hemp_element_boolean_and_symbol) {
    hemp_element_boolean_symbol(HEMP_SYMBOL_ARG_NAMES);
    symbol->boolean = &hemp_element_boolean_and_value;
    return symbol;
}


HEMP_EVAL_FUNC(hemp_element_boolean_and_value) {
    hemp_debug_call("hemp_element_boolean_and_value()\n");

    hemp_element_p lhs = hemp_lhs_element(element);
    hemp_element_p rhs = hemp_rhs_element(element);
//  hemp_debug("[and] evaluating LHS (%s) as boolean\n", lhs->type->name);
    hemp_value_t lval  = lhs->type->boolean(lhs, context);
    
    /* TODO: short-circuit if false */
//  hemp_debug("[and] evaluating RHS (%s) as boolean\n", rhs->type->name);
    hemp_value_t rval  = rhs->type->boolean(rhs, context);

    return (hemp_is_true(lval) && hemp_is_true(rval))
        ? HempTrue
        : HempFalse;
}


/*--------------------------------------------------------------------------
 * or
 *
 * Another standard binary boolean operator.
 *--------------------------------------------------------------------------*/

HEMP_SYMBOL_FUNC(hemp_element_boolean_or_symbol) {
    hemp_element_boolean_symbol(HEMP_SYMBOL_ARG_NAMES);
    symbol->boolean = &hemp_element_boolean_or_value;
    return symbol;
}


HEMP_EVAL_FUNC(hemp_element_boolean_or_value) {
    hemp_debug_call("hemp_element_boolean_or_value()\n");

    hemp_element_p lhs = hemp_lhs_element(element);
    hemp_element_p rhs = hemp_rhs_element(element);
    hemp_value_t lval  = lhs->type->boolean(lhs, context);
    hemp_value_t rval  = rhs->type->boolean(rhs, context);

    return (hemp_is_true(lval) || hemp_is_true(rval))
        ? HempTrue
        : HempFalse;
}


