#include <hemp/element.h>

/*--------------------------------------------------------------------------
 * binary operator elements
 *
 * This generic operator element role patches in a bunch of functions to act 
 * as "methods" for operator elements.  This is further "sub-classed" to 
 * create prefix, postfix and infix operators that have the correct default
 * parsing methods attached (operators are of course free to implement their
 * own parsing methods), and the correct source() method for re-generating 
 * the canonical form of the template source.
 *--------------------------------------------------------------------------*/

HEMP_ELEMENT(hemp_element_operator) {
    element->token   = &hemp_element_literal_text;
    element->source  = &hemp_element_literal_text;
    element->text    = &hemp_element_value_text;
    element->value   = &hemp_element_not_value;   /* sub-types redefine this */
    element->values  = &hemp_element_value_values;
    element->number  = &hemp_element_value_number;
    element->integer = &hemp_element_value_integer;
    element->boolean = &hemp_element_value_boolean;
    element->compare = &hemp_element_value_compare;
    return element;
}


HEMP_ELEMENT(hemp_element_prefix) {
    hemp_element_operator(hemp, element);
    element->parse_prefix  = &hemp_element_parse_prefix;
    element->source        = &hemp_element_prefix_source;
    return element;
}


HEMP_ELEMENT(hemp_element_postfix) {
    hemp_element_operator(hemp, element);
    element->parse_postfix   = &hemp_element_parse_postfix;
    element->source          = &hemp_element_postfix_source;
    return element;
}


HEMP_ELEMENT(hemp_element_prepostfix) {
    hemp_element_operator(hemp, element);
    element->parse_prefix  = &hemp_element_parse_prefix;
    element->parse_postfix = &hemp_element_parse_postfix;
    element->source        = &hemp_element_prepostfix_source;
    return element;
}


HEMP_ELEMENT(hemp_element_infix) {
    hemp_element_operator(hemp, element);
    element->parse_postfix = &hemp_element_parse_infix_left;   /* hmmm.... should be non-assoc */
    element->source        = &hemp_element_infix_source;
    element->flags        |= HEMP_BE_INFIX;
    return element;
}


HEMP_ELEMENT(hemp_element_infix_left) {
    hemp_element_infix(hemp, element);
    element->parse_postfix = &hemp_element_parse_infix_left;
    return element;
}

HEMP_ELEMENT(hemp_element_infix_right) {
    hemp_element_infix(hemp, element);
    element->parse_postfix = &hemp_element_parse_infix_right;
    return element;
}


HEMP_OUTPUT(hemp_element_prefix_source) {
    hemp_todo("hemp_element_prefix_source()\n");
    return output;
}


HEMP_OUTPUT(hemp_element_postfix_source) {
    hemp_todo("hemp_element_postfix_source()\n");
    return output;
}


HEMP_OUTPUT(hemp_element_prepostfix_source) {
    hemp_todo("hemp_element_prepostfix_source()\n");

    return hemp_has_flag(hemp_val_frag(value), HEMP_BE_PREFIX)
        ? hemp_element_prefix_source(value, context, output)
        : hemp_element_postfix_source(value, context, output);
}


HEMP_OUTPUT(hemp_element_infix_source) {
    hemp_todo("hemp_element_infix_source()\n");

    /* ARSE!  I forgot, I'm using the source "method" to display token
     * list as part of the parser debug... will have to disable this for
     * now
     */
    return hemp_element_literal_text(value, context, output);
}

