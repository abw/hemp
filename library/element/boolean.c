#include <math.h>
#include <hemp/element.h>


/*--------------------------------------------------------------------------
 * boolean operators
 *
 * The "base class" boolean element role is a "sub-class" of a binary operator
 * element.  It patches in a value() "method" which delegates to an element's
 * boolean() method.  Each boolean operator should then define a boolean()
 * method to Do The Right Thing[tm].  Note that we initially patch the 
 * boolean() method to throw an error just in case someone (i.e. me) forgets
 * to define a boolean() method for an operator.  This makes the mistake easy
 * to spot and fix rather than having the value() and boolean() methods call 
 * each other in a deadly embrace until the universe grows cold.
 *--------------------------------------------------------------------------*/

HEMP_ELEMENT(hemp_element_boolean) {
    hemp_element_infix(hemp, element);
    element->value   = &hemp_element_boolean_value;
    element->boolean = &hemp_element_not_boolean;
    return element;
}

HEMP_VALUE(hemp_element_boolean_value) {
    hemp_debug_call("hemp_element_boolean_value()\n");
    return hemp_obcall(value, boolean, context);
}



/*--------------------------------------------------------------------------
 * not
 *
 * The exception that proves the rule: 'not' is a prefix operator rather 
 * than an infix/postfix operator, so we have to unpick some of the work we
 * did in the call to hemp_element_boolean/infix.  Other than that, it's 
 * just a case of patching in the boolean() evaluation function.
 *--------------------------------------------------------------------------*/

HEMP_ELEMENT(hemp_element_boolean_not) {
    hemp_element_boolean(hemp, element);
    element->parse_prefix  = &hemp_element_parse_prefix;
    element->parse_postfix = NULL;
    element->boolean       = &hemp_element_boolean_not_value;
    return element;
}


HEMP_VALUE(hemp_element_boolean_not_value) {
    hemp_debug_call("hemp_element_boolean_not_value()\n");

    HempFragment fragment = hemp_val_frag(value);
    HempValue    expr     = hemp_expr(fragment);
    HempValue    result   = hemp_obcall(expr, boolean, context);

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

HEMP_ELEMENT(hemp_element_boolean_and) {
    hemp_element_boolean(hemp, element);
    element->boolean = &hemp_element_boolean_and_value;
    return element;
}


HEMP_VALUE(hemp_element_boolean_and_value) {
    hemp_debug_call("hemp_element_boolean_and_value()\n");

    HempFragment fragment = hemp_val_frag(value);
    HempValue    lhs      = hemp_lhs(fragment);
    HempValue    rhs      = hemp_rhs(fragment);
//  hemp_debug("[and] evaluating LHS (%s) as boolean\n", lhs->type->name);
    HempValue lval       = hemp_obcall(lhs, boolean, context);
    
    /* TODO: short-circuit if false */
//  hemp_debug("[and] evaluating RHS (%s) as boolean\n", rhs->type->name);
    HempValue rval       = hemp_obcall(rhs, boolean, context);

    return (hemp_is_true(lval) && hemp_is_true(rval))
        ? HempTrue
        : HempFalse;
}


/*--------------------------------------------------------------------------
 * or
 *
 * Another standard binary boolean operator.
 *--------------------------------------------------------------------------*/

HEMP_ELEMENT(hemp_element_boolean_or) {
    hemp_element_boolean(hemp, element);
    element->boolean = &hemp_element_boolean_or_value;
    return element;
}


HEMP_VALUE(hemp_element_boolean_or_value) {
    hemp_debug_call("hemp_element_boolean_or_value()\n");

    HempFragment fragment = hemp_val_frag(value);
    HempValue    lhs      = hemp_lhs(fragment);
    HempValue    rhs      = hemp_rhs(fragment);
    HempValue    lval     = hemp_obcall(lhs, boolean, context);
    HempValue    rval     = hemp_obcall(rhs, boolean, context);

    return (hemp_is_true(lval) || hemp_is_true(rval))
        ? HempTrue
        : HempFalse;
}


