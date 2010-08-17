#include <math.h>
#include <hemp/element.h>


/*--------------------------------------------------------------------------
 * comparison operators return a tri-state before/equal/after value, e.g. 
 * cmp, <=>, etc.
 *--------------------------------------------------------------------------*/

HEMP_SYMBOL_FUNC(hemp_element_compare_symbol) {
    hemp_element_infix_symbol(hemp, symbol);
    symbol->value   = &hemp_element_compare_value;
    symbol->compare = &hemp_element_not_compare;    /* subtypes redefine    */
    return symbol;
}


HEMP_EVAL_FUNC(hemp_element_compare_value) {
    hemp_debug_call("hemp_element_compare_value()\n");
    return element->type->compare(element, context);
}


/*--------------------------------------------------------------------------
 * equal is a boolean operator (returns true/false) that calls the element's
 * compare() method to return a comparison value, then return true/false if
 * the value is HempEqual
 *--------------------------------------------------------------------------*/

HEMP_SYMBOL_FUNC(hemp_element_compare_equal_symbol) {
    hemp_element_boolean_symbol(hemp, symbol);
    symbol->boolean = &hemp_element_compare_equal_value;
    return symbol;
}


HEMP_EVAL_FUNC(hemp_element_compare_equal_value) {
    hemp_debug_call("hemp_element_compare_equal_value()\n");

    return hemp_is_equal( element->type->compare(element, context) )
         ? HempTrue
         : HempFalse;
}


/*--------------------------------------------------------------------------
 * not equal is like equal... NOT!
 *--------------------------------------------------------------------------*/

HEMP_SYMBOL_FUNC(hemp_element_compare_not_equal_symbol) {
    hemp_element_boolean_symbol(hemp, symbol);
    symbol->boolean = &hemp_element_compare_not_equal_value;
    return symbol;
}


HEMP_EVAL_FUNC(hemp_element_compare_not_equal_value) {
    hemp_debug_call("hemp_element_compare_not_equal_value()\n");

    return hemp_is_equal( element->type->compare(element, context) )
         ? HempFalse
         : HempTrue;
}


/*--------------------------------------------------------------------------
 * before - did the compare() method return HempBefore?
 *--------------------------------------------------------------------------*/

HEMP_SYMBOL_FUNC(hemp_element_compare_before_symbol) {
    hemp_element_boolean_symbol(hemp, symbol);
    symbol->boolean = &hemp_element_compare_before_value;
    return symbol;
}


HEMP_EVAL_FUNC(hemp_element_compare_before_value) {
    hemp_debug_call("hemp_element_compare_before()\n");

    return hemp_is_before( element->type->compare(element, context) )
         ? HempTrue
         : HempFalse;
}


/*--------------------------------------------------------------------------
 * not before (i.e. equal or after)
 *--------------------------------------------------------------------------*/

HEMP_SYMBOL_FUNC(hemp_element_compare_not_before_symbol) {
    hemp_element_boolean_symbol(hemp, symbol);
    symbol->boolean = &hemp_element_compare_not_before_value;
    return symbol;
}


HEMP_EVAL_FUNC(hemp_element_compare_not_before_value) {
    hemp_debug_call("hemp_element_compare_not_before_value()\n");

    return hemp_is_before( element->type->compare(element, context) )
         ? HempFalse
         : HempTrue;
}


/*--------------------------------------------------------------------------
 * after
 *--------------------------------------------------------------------------*/

HEMP_SYMBOL_FUNC(hemp_element_compare_after_symbol) {
    hemp_element_boolean_symbol(hemp, symbol);
    symbol->boolean = &hemp_element_compare_after_value;
    return symbol;
}


HEMP_EVAL_FUNC(hemp_element_compare_after_value) {
    hemp_debug_call("hemp_element_compare_after_value()\n");

    return hemp_is_after( element->type->compare(element, context) )
         ? HempTrue
         : HempFalse;
}


/*--------------------------------------------------------------------------
 * not after (i.e. equal or before)
 *--------------------------------------------------------------------------*/

HEMP_SYMBOL_FUNC(hemp_element_compare_not_after_symbol) {
    hemp_element_boolean_symbol(hemp, symbol);
    symbol->boolean = &hemp_element_compare_not_after_value;
    return symbol;
}


HEMP_EVAL_FUNC(hemp_element_compare_not_after_value) {
    hemp_debug_call("hemp_element_compare_not_after_value()\n");

    return hemp_is_after( element->type->compare(element, context) )
         ? HempFalse
         : HempTrue;
}

