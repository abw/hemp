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
 * number operators
 *--------------------------------------------------------------------------*/

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


/*--------------------------------------------------------------------------
 * number plus '+'
 *--------------------------------------------------------------------------*/

HEMP_SYMBOL_FUNC(hemp_element_numop_plus_symbol) {
    // not quite right - needs to accept expr as well...
    symbol->expr    = &hemp_element_not_expr;
    symbol->infix   = &hemp_element_parse_infix_left;
    symbol->source  = &hemp_element_literal_text;
    symbol->text    = &hemp_element_numop_text;
//  symbol->value   = &hemp_element_numop_plus_value;
    symbol->number  = &hemp_element_numop_plus_value;
    symbol->integer = &hemp_element_numop_integer;
    return symbol;
//  return hemp_element_numop_TODO_symbol(hemp, symbol);
}


HEMP_VALUE_FUNC(hemp_element_numop_plus_value) {
    debug_call("hemp_element_numop_plus_value()\n");

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
 * number minus '-'
 *--------------------------------------------------------------------------*/

HEMP_SYMBOL_FUNC(hemp_element_numop_minus_symbol) {
    // not quite right - needs to accept expr as well...
    symbol->expr    = &hemp_element_not_expr;
    symbol->infix   = &hemp_element_parse_infix_left;
    symbol->source  = &hemp_element_literal_text;
    symbol->text    = &hemp_element_numop_text;
    symbol->number  = &hemp_element_numop_minus_value;
    symbol->integer = &hemp_element_numop_integer;
    return symbol;
//  return hemp_element_numop_TODO_symbol(hemp, symbol);
}


HEMP_VALUE_FUNC(hemp_element_numop_minus_value) {
    debug_call("hemp_element_numop_minus_value()\n");

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
 * power, e.g. '**', '^', or something similar
 *--------------------------------------------------------------------------*/

HEMP_SYMBOL_FUNC(hemp_element_numop_power_symbol) {
    symbol->expr    = &hemp_element_not_expr;
    symbol->infix   = &hemp_element_parse_infix_right;
    symbol->source  = &hemp_element_literal_text;
    symbol->text    = &hemp_element_numop_text;
//  symbol->value   = &hemp_element_numop_multiple_value;
    symbol->number  = &hemp_element_numop_power_value;
    symbol->integer = &hemp_element_numop_integer;
    return symbol;
}


HEMP_VALUE_FUNC(hemp_element_numop_power_value) {
    debug_call("hemp_element_numop_power_value()\n");

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
 * number multiply '*'
 *--------------------------------------------------------------------------*/

HEMP_SYMBOL_FUNC(hemp_element_numop_multiply_symbol) {
    symbol->expr    = &hemp_element_not_expr;
    symbol->infix   = &hemp_element_parse_infix_left;
    symbol->source  = &hemp_element_literal_text;
    symbol->text    = &hemp_element_numop_text;
//  symbol->value   = &hemp_element_numop_multiple_value;
    symbol->number  = &hemp_element_numop_multiply_value;
    symbol->integer = &hemp_element_numop_integer;
    return symbol;
}


HEMP_VALUE_FUNC(hemp_element_numop_multiply_value) {
    debug_call("hemp_element_numop_multiply_value()\n");

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
 * number division '/'
 *--------------------------------------------------------------------------*/

HEMP_SYMBOL_FUNC(hemp_element_numop_divide_symbol) {
    symbol->expr    = &hemp_element_not_expr;
    symbol->infix   = &hemp_element_parse_infix_left;
    symbol->source  = &hemp_element_literal_text;
    symbol->text    = &hemp_element_numop_text;
    symbol->number  = &hemp_element_numop_divide_value;
    symbol->integer = &hemp_element_numop_integer;
    return symbol;
}


HEMP_VALUE_FUNC(hemp_element_numop_divide_value) {
    debug_call("hemp_element_numop_divide_value()\n");

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
 * integer division forces both sides to integers
 *--------------------------------------------------------------------------*/

HEMP_SYMBOL_FUNC(hemp_element_numop_divint_symbol) {
    symbol->expr    = &hemp_element_not_expr;
    symbol->infix   = &hemp_element_parse_infix_left;
    symbol->source  = &hemp_element_literal_text;
    symbol->text    = &hemp_element_numop_text;
    symbol->number  = &hemp_element_numop_divint_value;
    symbol->integer = &hemp_element_numop_divint_value;
    return symbol;
}


HEMP_VALUE_FUNC(hemp_element_numop_divint_value) {
    debug_call("hemp_element_numop_divint_value()\n");

    hemp_element_p lhs  = element->args.binary.lhs;
    hemp_element_p rhs  = element->args.binary.rhs;
    
    return HEMP_INT_VAL(
        (hemp_int_t)
        HEMP_VAL_INT( lhs->type->integer(lhs, context) )
      / HEMP_VAL_INT( rhs->type->integer(rhs, context) )
    );
}


/*--------------------------------------------------------------------------
 * modulus
 *--------------------------------------------------------------------------*/

HEMP_SYMBOL_FUNC(hemp_element_numop_modulus_symbol) {
    symbol->expr    = &hemp_element_not_expr;
    symbol->infix   = &hemp_element_parse_infix_left;
    symbol->source  = &hemp_element_literal_text;
    symbol->text    = &hemp_element_numop_text;
    symbol->number  = &hemp_element_numop_modulus_value;
    symbol->integer = &hemp_element_numop_modulus_value;
    return symbol;
}


HEMP_VALUE_FUNC(hemp_element_numop_modulus_value) {
    debug_call("hemp_element_numop_modulus_value()\n");

    hemp_element_p lhs  = element->args.binary.lhs;
    hemp_element_p rhs  = element->args.binary.rhs;
    
    return HEMP_INT_VAL(
        (hemp_int_t)
        HEMP_VAL_INT( lhs->type->integer(lhs, context) )
      % HEMP_VAL_INT( rhs->type->integer(rhs, context) )
    );
}


/*--------------------------------------------------------------------------
 * number comparisons, e.g. ==, !=, <, >, <=, >=, <=>
 *--------------------------------------------------------------------------*/

HEMP_SYMBOL_FUNC(hemp_element_numop_compare_symbol) {
    // not quite right - needs to accept expr as well...
    symbol->expr    = &hemp_element_not_expr;
    symbol->infix   = &hemp_element_parse_infix_left;
    symbol->source  = &hemp_element_literal_text;
    symbol->text    = &hemp_element_numop_text;
    symbol->number  = &hemp_element_numop_compare_value;        // FIXME: needs checking
    symbol->integer = &hemp_element_numop_compare_value;
    return symbol;
//  return hemp_element_numop_TODO_symbol(hemp, symbol);
}


HEMP_VALUE_FUNC(hemp_element_numop_compare_value) {
    debug_call("hemp_element_numop_compare_value()\n");

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


HEMP_SYMBOL_FUNC(hemp_element_numop_equal_symbol) {
    symbol->expr    = &hemp_element_not_expr;
    symbol->infix   = &hemp_element_parse_infix_left;
    symbol->source  = &hemp_element_literal_text;
    symbol->text    = &hemp_element_boolop_text;
    symbol->number  = &hemp_element_not_number;
    symbol->integer = &hemp_element_not_integer;
    symbol->boolean = &hemp_element_numop_equal_value;
    return symbol;
}


HEMP_VALUE_FUNC(hemp_element_numop_equal_value) {
    debug_call("hemp_element_numop_equal_value()\n");

    return HEMP_IS_EQUAL( hemp_element_numop_compare_value(element, context) )
         ? HempTrue
         : HempFalse;
}


HEMP_SYMBOL_FUNC(hemp_element_numop_not_equal_symbol) {
    symbol->expr    = &hemp_element_not_expr;
    symbol->infix   = &hemp_element_parse_infix_left;
    symbol->source  = &hemp_element_literal_text;
    symbol->text    = &hemp_element_boolop_text;
    symbol->number  = &hemp_element_not_number;
    symbol->integer = &hemp_element_not_integer;
    symbol->boolean = &hemp_element_numop_not_equal_value;
    return symbol;
}


HEMP_VALUE_FUNC(hemp_element_numop_not_equal_value) {
    debug_call("hemp_element_numop_not_equal_value()\n");

    return HEMP_IS_EQUAL( hemp_element_numop_compare_value(element, context) )
         ? HempFalse
         : HempTrue;
}


HEMP_SYMBOL_FUNC(hemp_element_numop_less_than_symbol) {
    symbol->expr    = &hemp_element_not_expr;
    symbol->infix   = &hemp_element_parse_infix_left;
    symbol->source  = &hemp_element_literal_text;
    symbol->text    = &hemp_element_boolop_text;
    symbol->number  = &hemp_element_not_number;
    symbol->integer = &hemp_element_not_integer;
    symbol->boolean = &hemp_element_numop_less_than_value;
    return symbol;
//  return hemp_element_numop_TODO_symbol(hemp, symbol);
}


HEMP_VALUE_FUNC(hemp_element_numop_less_than_value) {
    debug_call("hemp_element_numop_less_than_value()\n");

    return HEMP_IS_BEFORE( hemp_element_numop_compare_value(element, context) )
         ? HempTrue
         : HempFalse;
}


HEMP_SYMBOL_FUNC(hemp_element_numop_less_equal_symbol) {
    symbol->expr    = &hemp_element_not_expr;
    symbol->infix   = &hemp_element_parse_infix_left;
    symbol->source  = &hemp_element_literal_text;
    symbol->text    = &hemp_element_boolop_text;
    symbol->number  = &hemp_element_not_number;
    symbol->integer = &hemp_element_not_integer;
    symbol->boolean = &hemp_element_numop_less_equal_value;
    return symbol;
//  return hemp_element_numop_TODO_symbol(hemp, symbol);
}


HEMP_VALUE_FUNC(hemp_element_numop_less_equal_value) {
    debug_call("hemp_element_numop_less_than_value()\n");

    return HEMP_IS_AFTER( hemp_element_numop_compare_value(element, context) )
         ? HempFalse
         : HempTrue;
}


HEMP_SYMBOL_FUNC(hemp_element_numop_more_than_symbol) {
    symbol->expr    = &hemp_element_not_expr;
    symbol->infix   = &hemp_element_parse_infix_left;
    symbol->source  = &hemp_element_literal_text;
    symbol->text    = &hemp_element_boolop_text;
    symbol->number  = &hemp_element_not_number;
    symbol->integer = &hemp_element_not_integer;
    symbol->boolean = &hemp_element_numop_more_than_value;
    return symbol;
}


HEMP_VALUE_FUNC(hemp_element_numop_more_than_value) {
    debug_call("hemp_element_numop_more_than_value()\n");

    return HEMP_IS_AFTER( hemp_element_numop_compare_value(element, context) )
         ? HempTrue
         : HempFalse;
}


HEMP_SYMBOL_FUNC(hemp_element_numop_more_equal_symbol) {
    symbol->expr    = &hemp_element_not_expr;
    symbol->infix   = &hemp_element_parse_infix_left;
    symbol->source  = &hemp_element_literal_text;
    symbol->text    = &hemp_element_boolop_text;
    symbol->number  = &hemp_element_not_number;
    symbol->integer = &hemp_element_not_integer;
    symbol->boolean = &hemp_element_numop_more_equal_value;
    return symbol;
}


HEMP_VALUE_FUNC(hemp_element_numop_more_equal_value) {
    debug_call("hemp_element_numop_more_equal_value()\n");

    return HEMP_IS_BEFORE( hemp_element_numop_compare_value(element, context) )
         ? HempFalse
         : HempTrue;
}

