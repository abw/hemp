#include <hemp/element.h>


hemp_symbol_p HempSymbolText = NULL;


/*--------------------------------------------------------------------------
 * text elements and operators created as dynamic grammar symbols
 *--------------------------------------------------------------------------*/

HEMP_GLOBAL_SYMBOL(hemp_symbol_text) {
    hemp_debug_call("hemp_symbol_text()\n");
    return hemp_element_text_symbol(
        NULL,
        hemp_symbol_init("hemp.text", NULL, NULL)
    );
}


HEMP_SYMBOL_FUNC(hemp_element_text_symbol) {
    hemp_element_literal_symbol(hemp, symbol);
    symbol->prefix  = &hemp_element_literal_prefix;
    symbol->flags   = HEMP_BE_SOURCE | HEMP_BE_FIXED;
    return symbol;
}


//HEMP_SYMBOL_FUNC(hemp_element_textop_symbol) {
//    hemp_element_binary_symbol(hemp, symbol);
//    symbol->text    = &hemp_element_not_text;      /* redefined for each op */
//    symbol->number  = &hemp_element_textop_number;
//    symbol->integer = &hemp_element_textop_integer;
//    symbol->boolean = &hemp_element_textop_boolean;
//    return symbol;
//}

HEMP_PREFIX_FUNC(hemp_element_text_prefix) {
    hemp_debug_call("hemp_element_text_prefix()\n");
    
    /* Advance the pointer to the next element after this one and return the 
     * current element as the yielded expression.
     */
    hemp_element_p element = *elemptr;
    
    if (hemp_has_next(elemptr))
        hemp_go_next(elemptr);

    return element;
}



/* TODO: perhaps move this as it only applies to FIXED literal text
 * like squote and dquote 
 */

void
hemp_element_text_clean(
    hemp_element_p element
) {
    hemp_debug_call("hemp_element_text_clean(%p)\n", element);

    /* squote and dquote elements may have allocated memory to accomodate
     * translated escape sequences, e.g. \n \\, etc
     */
    if (hemp_not_flag(element, HEMP_BE_SOURCE)) {
        hemp_mem_free((hemp_mem_p) hemp_val_str(element->args.value));
    }
}


/*--------------------------------------------------------------------------
 * text concatentation '~'
 *--------------------------------------------------------------------------*/

HEMP_SYMBOL_FUNC(hemp_element_text_concat_symbol) {
    hemp_element_infix_left_symbol(hemp, symbol);
    symbol->value = &hemp_element_text_value;
    symbol->text  = &hemp_element_text_concat_value;
    return symbol;
}

HEMP_VALUE_FUNC(hemp_element_text_value) {
    return hemp_obcall(value, text, context, HempNothing);
}


HEMP_OUTPUT_FUNC(hemp_element_text_concat_value) {
    hemp_debug_call("hemp_element_text_concat_value()\n");
    hemp_element_p element = hemp_val_elem(value);

    hemp_text_p text;
    hemp_prepare_text(context, output, text);
    hemp_value_t lhs = hemp_lhs(element);
    hemp_value_t rhs = hemp_rhs(element);
    hemp_obcall(lhs, text, context, output);
    hemp_obcall(rhs, text, context, output);

    return output;
}


/*--------------------------------------------------------------------------
 * text comparison, e.g. '<=>'
 *--------------------------------------------------------------------------*/

HEMP_SYMBOL_FUNC(hemp_element_text_compare_symbol) {
    hemp_element_compare_symbol(hemp, symbol);
    symbol->compare = &hemp_element_text_compare_value;
    return symbol;
}


HEMP_VALUE_FUNC(hemp_element_text_compare_value) {
    hemp_debug_call("hemp_element_text_compare_value()\n");

    hemp_element_p  element = hemp_val_elem(value);
    hemp_value_t    lhs     = hemp_lhs(element);
    hemp_value_t    rhs     = hemp_rhs(element);
    hemp_value_t    lval    = hemp_obcall(lhs, text, context, HempNothing);
    hemp_value_t    rval    = hemp_obcall(rhs, text, context, HempNothing);
    hemp_int_t      compare = strcmp( 
        hemp_val_text(lval)->string,
        hemp_val_text(rval)->string
    );
//    hemp_text_free( hemp_val_text(lval) );
//    hemp_text_free( hemp_val_text(rval) );
    
    return  compare < 0 ? HempBefore
        :   compare > 0 ? HempAfter
        :                 HempEqual;
}


/*--------------------------------------------------------------------------
 * text equality, e.g. 'eq'
 *--------------------------------------------------------------------------*/

HEMP_SYMBOL_FUNC(hemp_element_text_equal_symbol) {
    hemp_element_compare_equal_symbol(hemp, symbol);
    symbol->compare = &hemp_element_text_compare_value;
    return symbol;
}


/*--------------------------------------------------------------------------
 * text inequality, e.g. 'ne'
 *--------------------------------------------------------------------------*/

HEMP_SYMBOL_FUNC(hemp_element_text_not_equal_symbol) {
    hemp_element_compare_not_equal_symbol(hemp, symbol);
    symbol->compare = &hemp_element_text_compare_value;
    return symbol;
}


/*--------------------------------------------------------------------------
 * text less than, e.g. 'lt'
 *--------------------------------------------------------------------------*/

HEMP_SYMBOL_FUNC(hemp_element_text_before_symbol) {
    hemp_element_compare_before_symbol(hemp, symbol);
    symbol->compare = &hemp_element_text_compare_value;
    return symbol;
}


/*--------------------------------------------------------------------------
 * text more than more than or equal to, e.g. 'ge'
 *--------------------------------------------------------------------------*/

HEMP_SYMBOL_FUNC(hemp_element_text_not_before_symbol) {
    hemp_element_compare_not_before_symbol(hemp, symbol);
    symbol->compare = &hemp_element_text_compare_value;
    return symbol;
}


/*--------------------------------------------------------------------------
 * text more than, e.g. 'gt'
 *--------------------------------------------------------------------------*/

HEMP_SYMBOL_FUNC(hemp_element_text_after_symbol) {
    hemp_element_compare_after_symbol(hemp, symbol);
    symbol->compare = &hemp_element_text_compare_value;
    return symbol;
}


/*--------------------------------------------------------------------------
 * text less than or equal to, e.g. 'le'
 *--------------------------------------------------------------------------*/

HEMP_SYMBOL_FUNC(hemp_element_text_not_after_symbol) {
    hemp_element_compare_not_after_symbol(hemp, symbol);
    symbol->compare = &hemp_element_text_compare_value;
    return symbol;
}




