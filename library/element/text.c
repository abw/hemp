#include <hemp/element.h>


/*--------------------------------------------------------------------------
 * static element to represent literal text
 *--------------------------------------------------------------------------*/

static struct hemp_symbol_s
    hemp_symbol_text = { 
        "text",                                     /* name                 */
        NULL,                                       /* start token          */
        NULL,                                       /* end token            */
        HEMP_BE_SOURCE   |                           /* flags                */
        HEMP_BE_STATIC,
        0, 0,                                       /* l/r precedence       */
        NULL,                                       /* scanner callback     */
        NULL,                                       /* cleanup callback     */
        &hemp_element_text_expr,                    /* parse expression     */
        &hemp_element_not_infix,                    /* parse infix expr     */
        &hemp_element_literal_source,               /* source code          */
        &hemp_element_literal_text,                 /* output text          */
        &hemp_element_literal_number,               /* numeric conversion   */
        &hemp_element_literal_integer,              /* integer conversion   */
        &hemp_element_literal_boolean,              /* boolean conversion   */
    };

hemp_symbol_p HempSymbolText = &hemp_symbol_text;


/*--------------------------------------------------------------------------
 * text elements and operators created as dynamic grammar symbols
 *--------------------------------------------------------------------------*/

HEMP_SYMBOL_FUNC(hemp_element_text_symbol) {
    symbol->flags   = HEMP_BE_SOURCE | HEMP_BE_STATIC;
    symbol->expr    = &hemp_element_literal_expr,
    symbol->source  = &hemp_element_literal_source;
    symbol->text    = &hemp_element_literal_text;
    symbol->number  = &hemp_element_literal_number;
    symbol->integer = &hemp_element_literal_integer;
    symbol->boolean = &hemp_element_literal_boolean;
    return symbol;
}


HEMP_SYMBOL_FUNC(hemp_element_textop_symbol) {
    symbol->expr    = &hemp_element_not_expr;
    symbol->infix   = &hemp_element_parse_infix_left;
    symbol->source  = &hemp_element_binary_source;
    symbol->text    = &hemp_element_not_text;      /* redefined for each op */
    symbol->number  = &hemp_element_textop_number;
    symbol->integer = &hemp_element_textop_integer;
    symbol->boolean = &hemp_element_textop_boolean;
    return symbol;
}

HEMP_PARSE_FUNC(hemp_element_text_expr) {
    hemp_debug_call("hemp_element_text_expr()\n");
    
    /* Advance the pointer to the next element after this one and return the 
     * current element as the yielded expression.
     */
    hemp_element_p element = *elemptr;
    
    if (hemp_has_next(elemptr))
        hemp_go_next(elemptr);

    return element;
}




/*--------------------------------------------------------------------------
 * functions for coercing text to other value types
 *--------------------------------------------------------------------------*/

HEMP_EVAL_FUNC(hemp_element_textop_number) {
    hemp_debug_call("hemp_element_textop_number()\n");

    hemp_text_p text;
    hemp_value_t value = element->type->text(element, context, HempNothing);

    hemp_todo("see if text [%s] will convert to number\n", hemp_val_str(value));
}


HEMP_EVAL_FUNC(hemp_element_textop_integer) {
    hemp_debug_call("hemp_element_textop_integer()\n");

    hemp_text_p text;
    hemp_value_t value = element->type->text(element, context, HempNothing);

    hemp_todo("see if text [%s] will convert to integer\n", hemp_val_str(value));
}


HEMP_EVAL_FUNC(hemp_element_textop_boolean) {
    hemp_debug_call("hemp_element_textop_boolean()\n");

    hemp_text_p text;
    hemp_value_t value = element->type->text(element, context, HempNothing);

    hemp_todo("see if text [%s] will convert to boolean\n", hemp_val_str(value));
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
    hemp_element_textop_symbol(hemp, symbol);
    symbol->text = &hemp_element_text_concat_value;
    return symbol;
}


HEMP_OUTPUT_FUNC(hemp_element_text_concat_value) {
    hemp_debug_call("hemp_element_text_concat_value()\n");

    hemp_text_p text;
    hemp_prepare_output(output, text, 0);
    hemp_element_p lhs = element->args.binary.lhs;
    hemp_element_p rhs = element->args.binary.rhs;
    lhs->type->text(lhs, context, output);
    rhs->type->text(rhs, context, output);

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


HEMP_EVAL_FUNC(hemp_element_text_compare_value) {
    hemp_debug_call("hemp_element_text_compare_value()\n");

    hemp_element_p lhs = element->args.binary.lhs;
    hemp_element_p rhs = element->args.binary.rhs;
    hemp_value_t lval  = lhs->type->text(lhs, context, HempNothing);
    hemp_value_t rval  = rhs->type->text(rhs, context, HempNothing);
    hemp_int_t compare = strcmp( 
        hemp_val_text(lval)->string,
        hemp_val_text(rval)->string
    );
    hemp_text_free( hemp_val_text(lval) );
    hemp_text_free( hemp_val_text(rval) );
    
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




