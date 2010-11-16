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

HEMP_VALUE_FUNC(hemp_element_boolean_value) {
    hemp_debug_call("hemp_element_boolean_value()\n");
    return hemp_obcall(value, boolean, context);
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


HEMP_VALUE_FUNC(hemp_element_boolean_not_value) {
    hemp_debug_call("hemp_element_boolean_not_value()\n");

    hemp_element_p element  = hemp_val_elem(value);
    hemp_value_t   expr     = hemp_expr(element);
    hemp_value_t   result   = hemp_obcall(expr, boolean, context);

    return hemp_is_true(result)
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


HEMP_VALUE_FUNC(hemp_element_boolean_and_value) {
    hemp_debug_call("hemp_element_boolean_and_value()\n");

    hemp_element_p element  = hemp_val_elem(value);
    hemp_value_t   lhs      = hemp_lhs(element);
    hemp_value_t   rhs      = hemp_rhs(element);
//  hemp_debug("[and] evaluating LHS (%s) as boolean\n", lhs->type->name);
    hemp_value_t lval       = hemp_obcall(lhs, boolean, context);
    
    /* TODO: short-circuit if false */
//  hemp_debug("[and] evaluating RHS (%s) as boolean\n", rhs->type->name);
    hemp_value_t rval       = hemp_obcall(rhs, boolean, context);

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


HEMP_VALUE_FUNC(hemp_element_boolean_or_value) {
    hemp_debug_call("hemp_element_boolean_or_value()\n");

    hemp_element_p  element = hemp_val_elem(value);
    hemp_value_t    lhs     = hemp_lhs(element);
    hemp_value_t    rhs     = hemp_rhs(element);
    hemp_value_t    lval    = hemp_obcall(lhs, boolean, context);
    hemp_value_t    rval    = hemp_obcall(rhs, boolean, context);

    return (hemp_is_true(lval) || hemp_is_true(rval))
        ? HempTrue
        : HempFalse;
}


