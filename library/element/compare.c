#include <math.h>
#include <hemp/element.h>


/*--------------------------------------------------------------------------
 * comparison operators
 *--------------------------------------------------------------------------*/

HEMP_SYMBOL_FUNC(hemp_element_compare_symbol) {
    symbol->expr    = &hemp_element_not_expr;
    symbol->infix   = &hemp_element_parse_infix_left;
    symbol->source  = &hemp_element_binary_source;
    symbol->text    = &hemp_element_compare_text;
    symbol->number  = &hemp_element_compare_integer;    // TODO: decide if this is OK
    symbol->integer = &hemp_element_compare_integer;
    return symbol;
}


HEMP_OUTPUT_FUNC(hemp_element_compare_text) {
    debug_call("hemp_element_compare_text()\n");

    hemp_value_t value = element->type->compare(element, context);
    hemp_cstr_p  cstr  = HEMP_TYPE_NAME(value);
                
    hemp_text_p text;
    hemp_prepare_output(output, text, strlen(cstr));
    hemp_text_append_cstr(text, cstr);

    return output;
}


HEMP_VALUE_FUNC(hemp_element_compare_integer) {
    debug_call("hemp_element_compare_integer()\n");

    hemp_value_t value = element->type->compare(element, context);
    hemp_int_t   compare;
    
    debug("compare value type [%s]: %s\n", HEMP_TYPE_ID(value), HEMP_TYPE_NAME(value));

    switch (HEMP_IDENT_ID(value)) {
        case HEMP_IDENT_BEFORE_ID:
            compare = -1;
            break;

        case HEMP_IDENT_EQUAL_ID:
            compare = 0;
            break;

        case HEMP_IDENT_AFTER_ID:
            compare = 1;
            break;
        
        default:
            // FIXME
            hemp_fatal(
                "%s compare function did not return a comparison value\n",
                element->type->name
            );
    }
    
    debug("comparison yielded number: %d\n", compare);
                
    return hemp_int_val(compare);
}


/*--------------------------------------------------------------------------
 * equal
 *--------------------------------------------------------------------------*/

HEMP_SYMBOL_FUNC(hemp_element_compare_equal_symbol) {
    hemp_element_boolean_symbol(hemp, symbol);
    symbol->boolean = &hemp_element_compare_equal_value;
    return symbol;
}


HEMP_VALUE_FUNC(hemp_element_compare_equal_value) {
    debug_call("hemp_element_compare_equal_value()\n");

    return HEMP_IS_EQUAL( element->type->compare(element, context) )
         ? HempTrue
         : HempFalse;
}


/*--------------------------------------------------------------------------
 * not equal
 *--------------------------------------------------------------------------*/

HEMP_SYMBOL_FUNC(hemp_element_compare_not_equal_symbol) {
    hemp_element_boolean_symbol(hemp, symbol);
    symbol->boolean = &hemp_element_compare_not_equal_value;
    return symbol;
}


HEMP_VALUE_FUNC(hemp_element_compare_not_equal_value) {
    debug_call("hemp_element_compare_not_equal_value()\n");

    return HEMP_IS_EQUAL( element->type->compare(element, context) )
         ? HempFalse
         : HempTrue;
}


/*--------------------------------------------------------------------------
 * before
 *--------------------------------------------------------------------------*/

HEMP_SYMBOL_FUNC(hemp_element_compare_before_symbol) {
    hemp_element_boolean_symbol(hemp, symbol);
    symbol->boolean = &hemp_element_compare_before_value;
    return symbol;
}


HEMP_VALUE_FUNC(hemp_element_compare_before_value) {
    debug_call("hemp_element_compare_before()\n");

    return HEMP_IS_BEFORE( element->type->compare(element, context) )
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


HEMP_VALUE_FUNC(hemp_element_compare_not_before_value) {
    debug_call("hemp_element_compare_not_before_value()\n");

    return HEMP_IS_BEFORE( element->type->compare(element, context) )
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


HEMP_VALUE_FUNC(hemp_element_compare_after_value) {
    debug_call("hemp_element_compare_after_value()\n");

    return HEMP_IS_AFTER( element->type->compare(element, context) )
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


HEMP_VALUE_FUNC(hemp_element_compare_not_after_value) {
    debug_call("hemp_element_compare_not_after_value()\n");

    return HEMP_IS_AFTER( element->type->compare(element, context) )
         ? HempFalse
         : HempTrue;
}

