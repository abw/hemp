#include <math.h>
#include <hemp/element.h>


/*--------------------------------------------------------------------------
 * comparison operators return a tri-state before/equal/after value, e.g. 
 * cmp, <=>, etc.
 *--------------------------------------------------------------------------*/

HEMP_SYMBOL(hemp_element_compare_symbol) {
    hemp_element_infix_symbol(hemp, symbol);
    symbol->value   = &hemp_element_compare_value;
    symbol->compare = &hemp_element_not_compare;    /* subtypes redefine    */
    return symbol;
}


HEMP_VALUE_FUNC(hemp_element_compare_value) {
    hemp_debug_call("hemp_element_compare_value()\n");
    return hemp_obcall(value, compare, context);
}


/*--------------------------------------------------------------------------
 * equal is a boolean operator (returns true/false) that calls the element's
 * compare() method to return a comparison value, then return true/false if
 * the value is HempEqual
 *--------------------------------------------------------------------------*/

HEMP_SYMBOL(hemp_element_compare_equal_symbol) {
    hemp_element_boolean_symbol(hemp, symbol);
    symbol->boolean = &hemp_element_compare_equal_value;
    return symbol;
}


HEMP_VALUE_FUNC(hemp_element_compare_equal_value) {
    hemp_debug_call("hemp_element_compare_equal_value()\n");
    hemp_value result = hemp_obcall(value, compare, context);

    return hemp_is_equal(result)
         ? HempTrue
         : HempFalse;
}


/*--------------------------------------------------------------------------
 * not equal is like equal... NOT!
 *--------------------------------------------------------------------------*/

HEMP_SYMBOL(hemp_element_compare_not_equal_symbol) {
    hemp_element_boolean_symbol(hemp, symbol);
    symbol->boolean = &hemp_element_compare_not_equal_value;
    return symbol;
}


HEMP_VALUE_FUNC(hemp_element_compare_not_equal_value) {
    hemp_debug_call("hemp_element_compare_not_equal_value()\n");
    hemp_value result = hemp_obcall(value, compare, context);

    return hemp_is_equal(result)
         ? HempFalse
         : HempTrue;
}


/*--------------------------------------------------------------------------
 * before - did the compare() method return HempBefore?
 *--------------------------------------------------------------------------*/

HEMP_SYMBOL(hemp_element_compare_before_symbol) {
    hemp_element_boolean_symbol(hemp, symbol);
    symbol->boolean = &hemp_element_compare_before_value;
    return symbol;
}


HEMP_VALUE_FUNC(hemp_element_compare_before_value) {
    hemp_debug_call("hemp_element_compare_before()\n");
    hemp_value result = hemp_obcall(value, compare, context);

    return hemp_is_before(result)
         ? HempTrue
         : HempFalse;
}


/*--------------------------------------------------------------------------
 * not before (i.e. equal or after)
 *--------------------------------------------------------------------------*/

HEMP_SYMBOL(hemp_element_compare_not_before_symbol) {
    hemp_element_boolean_symbol(hemp, symbol);
    symbol->boolean = &hemp_element_compare_not_before_value;
    return symbol;
}


HEMP_VALUE_FUNC(hemp_element_compare_not_before_value) {
    hemp_debug_call("hemp_element_compare_not_before_value()\n");
    hemp_value result = hemp_obcall(value, compare, context);

    return hemp_is_before(result)
         ? HempFalse
         : HempTrue;
}


/*--------------------------------------------------------------------------
 * after
 *--------------------------------------------------------------------------*/

HEMP_SYMBOL(hemp_element_compare_after_symbol) {
    hemp_element_boolean_symbol(hemp, symbol);
    symbol->boolean = &hemp_element_compare_after_value;
    return symbol;
}


HEMP_VALUE_FUNC(hemp_element_compare_after_value) {
    hemp_debug_call("hemp_element_compare_after_value()\n");
    hemp_value result = hemp_obcall(value, compare, context);

    return hemp_is_after(result)
         ? HempTrue
         : HempFalse;
}


/*--------------------------------------------------------------------------
 * not after (i.e. equal or before)
 *--------------------------------------------------------------------------*/

HEMP_SYMBOL(hemp_element_compare_not_after_symbol) {
    hemp_element_boolean_symbol(hemp, symbol);
    symbol->boolean = &hemp_element_compare_not_after_value;
    return symbol;
}


HEMP_VALUE_FUNC(hemp_element_compare_not_after_value) {
    hemp_debug_call("hemp_element_compare_not_after_value()\n");
    hemp_value result = hemp_obcall(value, compare, context);

    return hemp_is_after(result)
         ? HempFalse
         : HempTrue;
}

