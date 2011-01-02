#include <math.h>
#include <hemp/element.h>


/*--------------------------------------------------------------------------
 * comparison operators return a tri-state before/equal/after value, e.g. 
 * cmp, <=>, etc.
 *--------------------------------------------------------------------------*/

HEMP_ELEMENT(hemp_element_compare) {
    hemp_element_infix(hemp, element);
    element->value   = &hemp_element_compare_value;
    element->compare = &hemp_element_not_compare;    /* subtypes redefine    */
    return element;
}


HEMP_VALUE(hemp_element_compare_value) {
    hemp_debug_call("hemp_element_compare_value()\n");
    return hemp_obcall(value, compare, context);
}


/*--------------------------------------------------------------------------
 * equal is a boolean operator (returns true/false) that calls the element's
 * compare() method to return a comparison value, then return true/false if
 * the value is HempEqual
 *--------------------------------------------------------------------------*/

HEMP_ELEMENT(hemp_element_compare_equal) {
    hemp_element_boolean(hemp, element);
    element->boolean = &hemp_element_compare_equal_value;
    return element;
}


HEMP_VALUE(hemp_element_compare_equal_value) {
    hemp_debug_call("hemp_element_compare_equal_value()\n");
    HempValue result = hemp_obcall(value, compare, context);

    return hemp_is_equal(result)
         ? HempTrue
         : HempFalse;
}


/*--------------------------------------------------------------------------
 * not equal is like equal... NOT!
 *--------------------------------------------------------------------------*/

HEMP_ELEMENT(hemp_element_compare_not_equal) {
    hemp_element_boolean(hemp, element);
    element->boolean = &hemp_element_compare_not_equal_value;
    return element;
}


HEMP_VALUE(hemp_element_compare_not_equal_value) {
    hemp_debug_call("hemp_element_compare_not_equal_value()\n");
    HempValue result = hemp_obcall(value, compare, context);

    return hemp_is_equal(result)
         ? HempFalse
         : HempTrue;
}


/*--------------------------------------------------------------------------
 * before - did the compare() method return HempBefore?
 *--------------------------------------------------------------------------*/

HEMP_ELEMENT(hemp_element_compare_before) {
    hemp_element_boolean(hemp, element);
    element->boolean = &hemp_element_compare_before_value;
    return element;
}


HEMP_VALUE(hemp_element_compare_before_value) {
    hemp_debug_call("hemp_element_compare_before()\n");
    HempValue result = hemp_obcall(value, compare, context);

    return hemp_is_before(result)
         ? HempTrue
         : HempFalse;
}


/*--------------------------------------------------------------------------
 * not before (i.e. equal or after)
 *--------------------------------------------------------------------------*/

HEMP_ELEMENT(hemp_element_compare_not_before) {
    hemp_element_boolean(hemp, element);
    element->boolean = &hemp_element_compare_not_before_value;
    return element;
}


HEMP_VALUE(hemp_element_compare_not_before_value) {
    hemp_debug_call("hemp_element_compare_not_before_value()\n");
    HempValue result = hemp_obcall(value, compare, context);

    return hemp_is_before(result)
         ? HempFalse
         : HempTrue;
}


/*--------------------------------------------------------------------------
 * after
 *--------------------------------------------------------------------------*/

HEMP_ELEMENT(hemp_element_compare_after) {
    hemp_element_boolean(hemp, element);
    element->boolean = &hemp_element_compare_after_value;
    return element;
}


HEMP_VALUE(hemp_element_compare_after_value) {
    hemp_debug_call("hemp_element_compare_after_value()\n");
    HempValue result = hemp_obcall(value, compare, context);

    return hemp_is_after(result)
         ? HempTrue
         : HempFalse;
}


/*--------------------------------------------------------------------------
 * not after (i.e. equal or before)
 *--------------------------------------------------------------------------*/

HEMP_ELEMENT(hemp_element_compare_not_after) {
    hemp_element_boolean(hemp, element);
    element->boolean = &hemp_element_compare_not_after_value;
    return element;
}


HEMP_VALUE(hemp_element_compare_not_after_value) {
    hemp_debug_call("hemp_element_compare_not_after_value()\n");
    HempValue result = hemp_obcall(value, compare, context);

    return hemp_is_after(result)
         ? HempFalse
         : HempTrue;
}

