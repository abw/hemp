#include <math.h>
#include <hemp/element.h>

/* TODO: check for overflow/underflow/divide-by-zero/NaN/Infinity */

#define HEMP_NUMOP_GET_INT(e,c)             \
    HEMP_VAL_INT(                           \
        e->type->integer(e, c)              \
    )

#define HEMP_NUMOP_CAST_NUM(v)              \
    (HEMP_IS_NUM(v)                         \
        ? HEMP_VAL_NUM(v)                   \
        : ((hemp_num_t) HEMP_VAL_INT(v))    \
    )


/*--------------------------------------------------------------------------
 * static elements to represent literal numbers and integers
 *--------------------------------------------------------------------------*/

static struct hemp_symbol_s
    hemp_symbol_number = { 
        "number",                                   /* name                 */
        NULL,                                       /* start token          */
        NULL,                                       /* end token            */
        HEMP_BE_SOURCE       |                       /* flags                */
        HEMP_BE_STATIC,
        0, 0,                                       /* l/r precedence       */
        NULL,                                       /* scanner callback     */
        NULL,                                       /* cleanup callback     */
        &hemp_element_number_expr,                  /* parse expression     */
        &hemp_element_not_infix,                    /* parse infix expr     */
        &hemp_element_literal_source,               /* source code          */
        &hemp_element_number_text,                  /* output text          */
        &hemp_element_number_number,                /* numeric conversion   */
        &hemp_element_number_integer,               /* integer conversion   */
        &hemp_element_number_boolean,               /* boolean conversion   */
        &hemp_element_number_compare,               /* to comparison        */
    };

hemp_symbol_p HempSymbolNumber = &hemp_symbol_number;


static struct hemp_symbol_s
    hemp_symbol_integer = { 
        "integer",                                  /* name                 */
        NULL,                                       /* start token          */
        NULL,                                       /* end token            */
        HEMP_BE_SOURCE       |                       /* flags                */
        HEMP_BE_STATIC,
        0, 0,                                       /* l/r precedence       */
        NULL,                                       /* scanner callback     */
        NULL,                                       /* cleanup callback     */
        &hemp_element_number_expr,                  /* parse expression     */
        &hemp_element_not_infix,                    /* parse infix expr     */
        &hemp_element_literal_source,               /* source code          */
        &hemp_element_integer_text,                 /* output text          */
        &hemp_element_integer_number,               /* numeric conversion   */
        &hemp_element_integer_integer,              /* integer conversion   */
        &hemp_element_integer_boolean,              /* boolean conversion   */
        &hemp_element_integer_compare,              /* to comparison        */
    };
    
hemp_symbol_p HempSymbolInteger = &hemp_symbol_integer;



/*--------------------------------------------------------------------------
 * element to represent a literal number
 *--------------------------------------------------------------------------*/

HEMP_SYMBOL_FUNC(hemp_element_number_symbol) {
    symbol->expr    = &hemp_element_number_expr,
    symbol->source  = &hemp_element_literal_source;
    symbol->text    = &hemp_element_number_text;
    symbol->number  = &hemp_element_number_number;
    symbol->integer = &hemp_element_number_integer;
    symbol->boolean = &hemp_element_number_boolean;
    symbol->flags   = HEMP_BE_SOURCE | HEMP_BE_STATIC;
    return symbol;
}


HEMP_PARSE_FUNC(hemp_element_number_expr) {
    debug_call("hemp_element_number_parse_expr() precedence is %d, parg: %d\n", (*elemptr)->type->lprec, precedence);
    
    hemp_element_p element = *elemptr;
  
    if (hemp_has_next(elemptr)) {
        hemp_go_next(elemptr);
        hemp_skip_whitespace(elemptr);
        hemp_symbol_p sym = (*elemptr)->type;

        if (sym->infix) {
//          return hemp_parse_infix(HEMP_PARSE_ARGS, element);
            return sym->infix(elemptr, scope, precedence, force, element);
        }
    }

    return element;
}


HEMP_OUTPUT_FUNC(hemp_element_number_text) {
    debug_call("hemp_element_number_text()\n");

    static hemp_char_t buffer[HEMP_BUFFER_SIZE];
    hemp_text_p text;

    snprintf(
        buffer, HEMP_BUFFER_SIZE, HEMP_FMT_NUM, 
        HEMP_VAL_NUM(element->args.value)
    );
    hemp_prepare_output(output, text, strlen(buffer));
    hemp_text_append_cstr(text, buffer);

    return output;
}


HEMP_VALUE_FUNC(hemp_element_number_number) {
    debug_call("hemp_element_number_number()\n");
    return element->args.value;
}


HEMP_VALUE_FUNC(hemp_element_number_integer) {
    debug_call("hemp_element_number_integer()\n");
    hemp_value_t value = element->args.value;
    return HEMP_IS_INT(value)
        ? value
        : HEMP_INT_VAL((hemp_int_t) HEMP_VAL_NUM(value));
}


HEMP_VALUE_FUNC(hemp_element_number_boolean) {
    hemp_todo("hemp_element_number_boolean() : number element => boolean value\n");
}


HEMP_VALUE_FUNC(hemp_element_number_compare) {
    debug_todo("hemp_element_number_compare()\n");
    // TODO: coerce number to HempBefore/HempEqual/HempAfter, etc
}



/*--------------------------------------------------------------------------
 * element to represent a literal integer
 *--------------------------------------------------------------------------*/

HEMP_SYMBOL_FUNC(hemp_element_integer_symbol) {
    hemp_element_number_symbol(hemp, symbol);
    symbol->text    = &hemp_element_integer_text;
    symbol->number  = &hemp_element_integer_number;
    symbol->integer = &hemp_element_integer_integer;
    symbol->boolean = &hemp_element_integer_boolean;
    symbol->compare = &hemp_element_not_compare;
    return symbol;
}


HEMP_OUTPUT_FUNC(hemp_element_integer_text) {
    debug_call("hemp_element_integer_text()\n");

    static hemp_char_t buffer[HEMP_BUFFER_SIZE];
    hemp_text_p text;

    snprintf(
        buffer, HEMP_BUFFER_SIZE, HEMP_FMT_INT,
        HEMP_VAL_INT(element->args.value)
    );
    hemp_prepare_output(output, text, strlen(buffer));
    hemp_text_append_cstr(text, buffer);

    return output;
}


HEMP_VALUE_FUNC(hemp_element_integer_number) {
    debug_call("hemp_element_integer_number()\n");
    hemp_value_t value = element->args.value;
    
    // FIXME: not sure what's best to do here... do we allow number() 
    // functions to return integers?
    return value;
    return HEMP_IS_NUM(value)
        ? value
        : HEMP_NUM_VAL((hemp_num_t) HEMP_VAL_INT(value));
}


HEMP_VALUE_FUNC(hemp_element_integer_integer) {
    debug_call("hemp_element_number_integer()\n");
    return element->args.value;
}


HEMP_VALUE_FUNC(hemp_element_integer_boolean) {
    hemp_todo("hemp_element_integer_boolean() : number element => boolean value\n");
}


HEMP_VALUE_FUNC(hemp_element_integer_compare) {
    debug_todo("hemp_element_integer_compare()\n");
    // TODO: coerce number to HempBefore/HempEqual/HempAfter, etc
}


/*--------------------------------------------------------------------------
 * number operators
 *--------------------------------------------------------------------------*/

HEMP_SYMBOL_FUNC(hemp_element_numop_symbol) {
    symbol->expr    = &hemp_element_not_expr;
    symbol->infix   = &hemp_element_parse_infix_left;
    symbol->source  = &hemp_element_binary_source;
    symbol->text    = &hemp_element_numop_text;
    symbol->integer = &hemp_element_numop_integer;
    symbol->boolean = &hemp_element_numop_boolean;
    symbol->compare = &hemp_element_not_compare;
    return symbol;
}


HEMP_OUTPUT_FUNC(hemp_element_numop_text) {
    debug_call("hemp_element_numop_text()\n");

    static hemp_char_t buffer[HEMP_BUFFER_SIZE];

    hemp_text_p text;
    hemp_value_t value = element->type->number(element, context);

    if (HEMP_IS_INT(value))
        snprintf(buffer, HEMP_BUFFER_SIZE, HEMP_FMT_INT, HEMP_VAL_INT(value));
    else
        snprintf(buffer, HEMP_BUFFER_SIZE, HEMP_FMT_NUM, HEMP_VAL_NUM(value));

    hemp_prepare_output(output, text, strlen(buffer));
    hemp_text_append_cstr(text, buffer);

    return output;
}


HEMP_VALUE_FUNC(hemp_element_numop_integer) {
    debug_call("hemp_element_numop_integer()\n");

    hemp_text_p text;
    hemp_value_t value = element->type->number(element, context);

    return HEMP_IS_INT(value)
        ? value
        : HEMP_INT_VAL((int) HEMP_VAL_NUM(value));
}


HEMP_VALUE_FUNC(hemp_element_numop_boolean) {
    debug_call("hemp_element_numop_boolean()\n");
    hemp_todo("hemp_element_numop_boolean()");
}


/*--------------------------------------------------------------------------
 * number plus, e.g. '+'
 *--------------------------------------------------------------------------*/

HEMP_SYMBOL_FUNC(hemp_element_number_plus_symbol) {
    hemp_element_numop_symbol(hemp, symbol);
    symbol->expr   = &hemp_element_parse_prefix;
    symbol->number = &hemp_element_number_plus_value;
    return symbol;
}


HEMP_VALUE_FUNC(hemp_element_number_plus_value) {
    debug_call("hemp_element_number_plus_value()\n");

    if (hemp_has_flag(element, HEMP_BE_PREFIX)) {
        hemp_todo("+ as a prefix operator");
    }

    hemp_element_p lhs = element->args.binary.lhs;
    hemp_element_p rhs = element->args.binary.rhs;
    hemp_value_t lval  = lhs->type->number(lhs, context);
    hemp_value_t rval, result;
    
    if (HEMP_IS_INT(lval)) {
        result = HEMP_INT_VAL(
            HEMP_VAL_INT(lval) 
          + HEMP_NUMOP_GET_INT(rhs, context)
        );
    }
    else {
        rval   = rhs->type->number(rhs, context);
        result = HEMP_NUM_VAL(
            HEMP_VAL_NUM(lval) 
          + HEMP_NUMOP_CAST_NUM(rval)
        );
    }

    return result;
}


/*--------------------------------------------------------------------------
 * number minus, e.g. '-'
 *--------------------------------------------------------------------------*/

HEMP_SYMBOL_FUNC(hemp_element_number_minus_symbol) {
    hemp_element_numop_symbol(hemp, symbol);
    symbol->expr   = &hemp_element_parse_prefix;
    symbol->number = &hemp_element_number_minus_value;
    return symbol;
}


HEMP_VALUE_FUNC(hemp_element_number_minus_value) {
    debug_call("hemp_element_number_minus_value()\n");

    if (hemp_has_flag(element, HEMP_BE_PREFIX)) {
        hemp_todo("+ as a prefix operator");
    }

    hemp_element_p lhs = element->args.binary.lhs;
    hemp_element_p rhs = element->args.binary.rhs;
    hemp_value_t lval  = lhs->type->number(lhs, context);
    hemp_value_t rval, result;

    if (HEMP_IS_INT(lval)) {
        result = HEMP_INT_VAL(
            HEMP_VAL_INT(lval) 
          - HEMP_NUMOP_GET_INT(rhs, context)
        );
    }
    else {
        rval   = rhs->type->number(rhs, context);
        result = HEMP_NUM_VAL(
            HEMP_VAL_NUM(lval) 
          - HEMP_NUMOP_CAST_NUM(rval)
        );
    }

    return result;
}


/*--------------------------------------------------------------------------
 * number raised to power, e.g. '**', '^', or something similar
 *--------------------------------------------------------------------------*/

HEMP_SYMBOL_FUNC(hemp_element_number_power_symbol) {
    hemp_element_numop_symbol(hemp, symbol);
    symbol->infix  = &hemp_element_parse_infix_right;
    symbol->number = &hemp_element_number_power_value;
    return symbol;
}


HEMP_VALUE_FUNC(hemp_element_number_power_value) {
    debug_call("hemp_element_number_power_value()\n");

    hemp_element_p lhs = element->args.binary.lhs;
    hemp_element_p rhs = element->args.binary.rhs;
    hemp_value_t lval  = lhs->type->number(lhs, context);
    hemp_value_t rval  = rhs->type->number(rhs, context);
    hemp_value_t result;

    if (HEMP_IS_INT(lval)) {
        result = HEMP_INT_VAL(
            (hemp_int_t)
            pow( 
                (hemp_num_t) HEMP_VAL_INT(lval),
                HEMP_NUMOP_CAST_NUM(rval)
            )
        );
    }
    else {
        result = HEMP_NUM_VAL(
            pow( 
                HEMP_VAL_NUM(lval),
                HEMP_NUMOP_CAST_NUM(rval)
            )
        );
    }

    return result;
}


/*--------------------------------------------------------------------------
 * number multiplication, e.g. '*' or 'x'
 *--------------------------------------------------------------------------*/

HEMP_SYMBOL_FUNC(hemp_element_number_multiply_symbol) {
    hemp_element_numop_symbol(hemp, symbol);
    symbol->number = &hemp_element_number_multiply_value;
    return symbol;
}


HEMP_VALUE_FUNC(hemp_element_number_multiply_value) {
    debug_call("hemp_element_number_multiply_value()\n");

    hemp_element_p lhs = element->args.binary.lhs;
    hemp_element_p rhs = element->args.binary.rhs;
    hemp_value_t lval  = lhs->type->number(lhs, context);
    hemp_value_t rval, result;

    if (HEMP_IS_INT(lval)) {
        result = HEMP_INT_VAL(
            HEMP_VAL_INT(lval) 
          * HEMP_NUMOP_GET_INT(rhs, context)
        );
    }
    else {
        rval   = rhs->type->number(rhs, context);
        result = HEMP_NUM_VAL(
            HEMP_VAL_NUM(lval) 
          * HEMP_NUMOP_CAST_NUM(rval)
        );
    }

    return result;
}


/*--------------------------------------------------------------------------
 * number division, e.g. '/'
 *--------------------------------------------------------------------------*/

HEMP_SYMBOL_FUNC(hemp_element_number_divide_symbol) {
    hemp_element_numop_symbol(hemp, symbol);
    symbol->number = &hemp_element_number_divide_value;
    return symbol;
}


HEMP_VALUE_FUNC(hemp_element_number_divide_value) {
    debug_call("hemp_element_number_divide_value()\n");

    hemp_element_p lhs = element->args.binary.lhs;
    hemp_element_p rhs = element->args.binary.rhs;
    hemp_value_t lval  = lhs->type->number(lhs, context);
    hemp_value_t rval, result;

    if (HEMP_IS_INT(lval)) {
        result = HEMP_INT_VAL(
            HEMP_VAL_INT(lval) 
          / HEMP_NUMOP_GET_INT(rhs, context)
        );
    }
    else {
        rval   = rhs->type->number(rhs, context);
        result = HEMP_NUM_VAL(
            HEMP_VAL_NUM(lval) 
          / HEMP_NUMOP_CAST_NUM(rval)
        );
    }

    return result;
}


/*--------------------------------------------------------------------------
 * integer division forces both sides to integers, e.g. 'div'
 *--------------------------------------------------------------------------*/

HEMP_SYMBOL_FUNC(hemp_element_number_divint_symbol) {
    hemp_element_numop_symbol(hemp, symbol);
    symbol->number  = &hemp_element_number_divint_value;
    symbol->integer = &hemp_element_number_divint_value;
    return symbol;
}


HEMP_VALUE_FUNC(hemp_element_number_divint_value) {
    debug_call("hemp_element_number_divint_value()\n");

    hemp_element_p lhs  = element->args.binary.lhs;
    hemp_element_p rhs  = element->args.binary.rhs;
    
    return HEMP_INT_VAL(
        (hemp_int_t)
        HEMP_VAL_INT( lhs->type->integer(lhs, context) )
      / HEMP_VAL_INT( rhs->type->integer(rhs, context) )
    );
}


/*--------------------------------------------------------------------------
 * integer modulus, e.g. '%', 'mod', etc.
 *--------------------------------------------------------------------------*/

HEMP_SYMBOL_FUNC(hemp_element_number_modulus_symbol) {
    hemp_element_numop_symbol(hemp, symbol);
    symbol->number  = &hemp_element_number_modulus_value;
    symbol->integer = &hemp_element_number_modulus_value;
    return symbol;
}


HEMP_VALUE_FUNC(hemp_element_number_modulus_value) {
    debug_call("hemp_element_number_modulus_value()\n");

    hemp_element_p lhs  = element->args.binary.lhs;
    hemp_element_p rhs  = element->args.binary.rhs;
    
    return HEMP_INT_VAL(
        (hemp_int_t)
        HEMP_VAL_INT( lhs->type->integer(lhs, context) )
      % HEMP_VAL_INT( rhs->type->integer(rhs, context) )
    );
}


/*--------------------------------------------------------------------------
 * number comparison, e.g. '<=>'
 *--------------------------------------------------------------------------*/

HEMP_SYMBOL_FUNC(hemp_element_number_compare_symbol) {
    hemp_element_compare_symbol(hemp, symbol);
    symbol->compare = &hemp_element_number_compare_value;
    return symbol;
}


HEMP_VALUE_FUNC(hemp_element_number_compare_value) {
    debug_call("hemp_element_number_compare_value()\n");

    hemp_element_p lhs = element->args.binary.lhs;
    hemp_element_p rhs = element->args.binary.rhs;
    hemp_value_t lval  = lhs->type->number(lhs, context);
    hemp_value_t rval  = rhs->type->number(rhs, context);
    hemp_num_t   lnum  = HEMP_NUMOP_CAST_NUM(lval);
    hemp_num_t   rnum  = HEMP_NUMOP_CAST_NUM(rval);
    
    return  lnum < rnum ? HempBefore
        :   lnum > rnum ? HempAfter
        :                 HempEqual;
}


/*--------------------------------------------------------------------------
 * number equality, e.g. '=='
 *--------------------------------------------------------------------------*/

HEMP_SYMBOL_FUNC(hemp_element_number_equal_symbol) {
    hemp_element_compare_equal_symbol(hemp, symbol);
    symbol->compare = &hemp_element_number_compare_value;
    return symbol;
}


/*--------------------------------------------------------------------------
 * number inequality, e.g. '!=', '<>', etc.
 *--------------------------------------------------------------------------*/

HEMP_SYMBOL_FUNC(hemp_element_number_not_equal_symbol) {
    hemp_element_compare_not_equal_symbol(hemp, symbol);
    symbol->compare = &hemp_element_number_compare_value;
    return symbol;
}


/*--------------------------------------------------------------------------
 * number less than, e.g. '<'
 *--------------------------------------------------------------------------*/

HEMP_SYMBOL_FUNC(hemp_element_number_before_symbol) {
    hemp_element_compare_before_symbol(hemp, symbol);
    symbol->compare = &hemp_element_number_compare_value;
    return symbol;
}


/*--------------------------------------------------------------------------
 * number less than or equal to, e.g. '<=', '!>', etc
 *--------------------------------------------------------------------------*/

HEMP_SYMBOL_FUNC(hemp_element_number_not_after_symbol) {
    hemp_element_compare_not_after_symbol(hemp, symbol);
    symbol->compare = &hemp_element_number_compare_value;
    return symbol;
}


/*--------------------------------------------------------------------------
 * number more than, e.g. '>'
 *--------------------------------------------------------------------------*/

HEMP_SYMBOL_FUNC(hemp_element_number_after_symbol) {
    hemp_element_compare_after_symbol(hemp, symbol);
    symbol->compare = &hemp_element_number_compare_value;
    return symbol;
}


/*--------------------------------------------------------------------------
 * number more than more than or equal to, e.g. '>=', '!<', etc.
 *--------------------------------------------------------------------------*/

HEMP_SYMBOL_FUNC(hemp_element_number_not_before_symbol) {
    hemp_element_compare_not_before_symbol(hemp, symbol);
    symbol->compare = &hemp_element_number_compare_value;
    return symbol;
}



