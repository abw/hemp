#include <hemp/element.h>

/*--------------------------------------------------------------------------
 * binary operator elements
 *
 * This generic operator symbol role patches in a bunch of functions to act 
 * as "methods" for operator elements.  This is further "sub-classed" to 
 * create prefix, postfix and infix operators that have the correct default
 * parsing methods attached (operators are of course free to implement their
 * own parsing methods), and the correct source() method for re-generating 
 * the canonical form of the template source.
 *--------------------------------------------------------------------------*/

HEMP_SYMBOL_FUNC(hemp_element_operator_symbol) {
    symbol->token   = &hemp_element_literal_token;
    symbol->source  = &hemp_element_literal_source;
    symbol->text    = &hemp_element_value_text;
    symbol->value   = &hemp_element_not_value;   /* sub-types redefine this */
    symbol->values  = &hemp_element_value_values;
    symbol->number  = &hemp_element_value_number;
    symbol->integer = &hemp_element_value_integer;
    symbol->boolean = &hemp_element_value_boolean;
    symbol->compare = &hemp_element_value_compare;
    return symbol;
}


HEMP_SYMBOL_FUNC(hemp_element_prefix_symbol) {
    hemp_element_operator_symbol(hemp, symbol);
    symbol->prefix  = &hemp_element_parse_prefix;
    symbol->source  = &hemp_element_prefix_source;
    return symbol;
}


HEMP_SYMBOL_FUNC(hemp_element_postfix_symbol) {
    hemp_element_operator_symbol(hemp, symbol);
    symbol->postfix = &hemp_element_parse_postfix;
    symbol->source  = &hemp_element_postfix_source;
    return symbol;
}


HEMP_SYMBOL_FUNC(hemp_element_prepostfix_symbol) {
    hemp_element_operator_symbol(hemp, symbol);
    symbol->prefix  = &hemp_element_parse_prefix;
    symbol->postfix = &hemp_element_parse_postfix;
    symbol->source  = &hemp_element_prepostfix_source;
    return symbol;
}


HEMP_SYMBOL_FUNC(hemp_element_infix_symbol) {
    hemp_element_operator_symbol(hemp, symbol);
    symbol->postfix = &hemp_element_parse_infix_left;   /* hmmm.... should be non-assoc */
    symbol->source  = &hemp_element_infix_source;
    symbol->flags  |= HEMP_BE_INFIX;
    return symbol;
}


HEMP_SYMBOL_FUNC(hemp_element_infix_left_symbol) {
    hemp_element_infix_symbol(hemp, symbol);
    symbol->postfix = &hemp_element_parse_infix_left;
    return symbol;
}

HEMP_SYMBOL_FUNC(hemp_element_infix_right_symbol) {
    hemp_element_infix_symbol(hemp, symbol);
    symbol->postfix = &hemp_element_parse_infix_right;
    return symbol;
}


HEMP_OUTPUT_FUNC(hemp_element_prefix_source) {
    hemp_todo("hemp_element_prefix_source()\n");
}


HEMP_OUTPUT_FUNC(hemp_element_postfix_source) {
    hemp_todo("hemp_element_postfix_source()\n");
}


HEMP_OUTPUT_FUNC(hemp_element_prepostfix_source) {
    hemp_todo("hemp_element_prepostfix_source()\n");

    return hemp_has_flag(hemp_val_elem(value), HEMP_BE_PREFIX)
        ? hemp_element_prefix_source(value, context, output)
        : hemp_element_postfix_source(value, context, output);
}


HEMP_OUTPUT_FUNC(hemp_element_infix_source) {
    hemp_todo("hemp_element_infix_source()\n");

    /* ARSE!  I forgot, I'm using the source "method" to display token
     * list as part of the parser debug... will have to disable this for
     * now
     */
    return hemp_element_literal_source(value, context, output);
}

