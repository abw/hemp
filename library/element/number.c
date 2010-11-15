#include <math.h>
#include <hemp/element.h>

/* TODO: check for overflow/underflow/divide-by-zero/NaN/Infinity */

#define HEMP_NUMOP_GET_INT(v,c)                 \
    hemp_val_int(                               \
        hemp_val_elem(v)->type->integer(v, c)   \
    )

#define HEMP_NUMOP_CAST_NUM(v)              \
    (hemp_is_number(v)                      \
        ? hemp_val_num(v)                   \
        : ((hemp_num_t) hemp_val_int(v))    \
    )


hemp_symbol_p HempSymbolNumber  = NULL;
hemp_symbol_p HempSymbolInteger = NULL;


/*--------------------------------------------------------------------------
 * static elements to represent literal numbers and integers
 *--------------------------------------------------------------------------*/

//
/*--------------------------------------------------------------------------
 * element to represent a literal number
 *--------------------------------------------------------------------------*/

HEMP_GLOBAL_SYMBOL(hemp_symbol_number) {
    hemp_debug_call("hemp_symbol_number()\n");
    return hemp_element_number_symbol(
        NULL,
        hemp_symbol_init("hemp.number", NULL, NULL)
    );
}


HEMP_SYMBOL_FUNC(hemp_element_number_symbol) {
    hemp_element_literal_symbol(hemp, symbol);
    symbol->prefix  = &hemp_element_literal_prefix;
    symbol->value   = &hemp_element_number_value;
    symbol->number  = &hemp_element_number_value;
    symbol->integer = &hemp_element_value_integer;
    symbol->boolean = &hemp_element_value_boolean;
    symbol->compare = &hemp_element_value_compare;
    symbol->flags   = HEMP_BE_SOURCE | HEMP_BE_FIXED;
    return symbol;
}


HEMP_VALUE_FUNC(hemp_element_number_value) {
    hemp_debug_call("hemp_element_number_value()\n");
    return hemp_val_elem(value)->args.value;
}



/*--------------------------------------------------------------------------
 * element to represent a literal integer
 *--------------------------------------------------------------------------*/

HEMP_GLOBAL_SYMBOL(hemp_symbol_integer) {
    hemp_debug_call("hemp_symbol_integer()\n");
    return hemp_element_integer_symbol(
        NULL,
        hemp_symbol_init("hemp.integer", NULL, NULL)
    );
}


HEMP_SYMBOL_FUNC(hemp_element_integer_symbol) {
    hemp_element_number_symbol(hemp, symbol);
    symbol->value   = &hemp_element_integer_value;
    symbol->integer = &hemp_element_integer_value;
    symbol->number  = &hemp_element_integer_value;
    return symbol;
}


HEMP_VALUE_FUNC(hemp_element_integer_value) {
    hemp_debug_call("hemp_element_integer_value()\n");
    return hemp_val_elem(value)->args.value;
}



/*--------------------------------------------------------------------------
 * number operators
 *--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------
 * auto-increment
 *--------------------------------------------------------------------------*/

HEMP_SYMBOL_FUNC(hemp_element_number_autoinc_symbol) {
    hemp_element_prepostfix_symbol(hemp, symbol);
    symbol->value  =
    symbol->number = &hemp_element_number_autoinc_value;
    return symbol;
}


HEMP_VALUE_FUNC(hemp_element_number_autoinc_value) {
    hemp_todo("hemp_element_number_autoinc_value()\n");
}


/*--------------------------------------------------------------------------
 * auto-decrement
 *--------------------------------------------------------------------------*/

HEMP_SYMBOL_FUNC(hemp_element_number_autodec_symbol) {
    hemp_element_prepostfix_symbol(hemp, symbol);
    symbol->value  =
    symbol->number = &hemp_element_number_autodec_value;
    return symbol;
}


HEMP_VALUE_FUNC(hemp_element_number_autodec_value) {
    hemp_todo("hemp_element_number_autodec_value()\n");
}


/*--------------------------------------------------------------------------
 * number plus, e.g. '+'
 *--------------------------------------------------------------------------*/

HEMP_SYMBOL_FUNC(hemp_element_number_plus_symbol) {
    hemp_element_infix_left_symbol(hemp, symbol);
    /* also works as prefix operator */
    symbol->prefix = &hemp_element_parse_prefix;
    symbol->value  =
    symbol->number = &hemp_element_number_plus_value;
    return symbol;
}


HEMP_VALUE_FUNC(hemp_element_number_plus_value) {
    hemp_debug_call("hemp_element_number_plus_value() [%p]\n", element);

    hemp_element_p element = hemp_val_elem(value);

    /* prefix unary '+' coerces value to a number */
    if (hemp_has_flag(element, HEMP_BE_PREFIX)) {
        hemp_value_t expr = hemp_expr(element);
        return hemp_val_elem(expr)->type->number(expr, context);
    }

    /* otherwise it's an infix addition operator */
    hemp_value_t lhs  = hemp_lhs(element);
    hemp_value_t rhs  = hemp_rhs(element);
    hemp_value_t lval = hemp_val_elem(lhs)->type->number(lhs, context);
    hemp_value_t rval, result;
    
    if (hemp_is_integer(lval)) {
        rval   = hemp_val_elem(rhs)->type->integer(rhs, context);
        result = hemp_int_val(
            hemp_val_int(lval) 
          + hemp_val_int(rval)
        );
    }
    else {
        rval   = hemp_val_elem(rhs)->type->number(rhs, context);
        result = hemp_num_val(
            hemp_val_num(lval) 
          + HEMP_NUMOP_CAST_NUM(rval)
        );
    }

    return result;
}


/*--------------------------------------------------------------------------
 * number minus, e.g. '-'
 *--------------------------------------------------------------------------*/

HEMP_SYMBOL_FUNC(hemp_element_number_minus_symbol) {
    hemp_element_infix_left_symbol(hemp, symbol);
    /* also works as prefix operator */
    symbol->prefix = &hemp_element_parse_prefix;
    symbol->value  =
    symbol->number = &hemp_element_number_minus_value;
    return symbol;
}


HEMP_VALUE_FUNC(hemp_element_number_minus_value) {
    hemp_debug_call("hemp_element_number_minus_value()\n");

    hemp_element_p element = hemp_val_elem(value);

    /* prefix unary '-' coerces value to a number and negates it */
    if (hemp_has_flag(element, HEMP_BE_PREFIX)) {
        hemp_value_t expr = hemp_expr(element);
        hemp_value_t val  = hemp_val_elem(expr)->type->number(expr, context);

        return hemp_is_integer(val)
            ? hemp_int_val(- hemp_val_int(val) )
            : hemp_num_val(- hemp_val_num(val) );
    }

    /* otherwise it's an infix subtraction operator */
    hemp_value_t lhs  = hemp_lhs(element);
    hemp_value_t rhs  = hemp_rhs(element);
    hemp_value_t lval = hemp_val_elem(lhs)->type->number(lhs, context);
    hemp_value_t rval, result;

    if (hemp_is_integer(lval)) {
        result = hemp_int_val(
            hemp_val_int(lval) 
          - HEMP_NUMOP_GET_INT(rhs, context)
        );
    }
    else {
        rval   = hemp_val_elem(rhs)->type->number(rhs, context);
        result = hemp_num_val(
            hemp_val_num(lval) 
          - HEMP_NUMOP_CAST_NUM(rval)
        );
    }

    return result;
}


/*--------------------------------------------------------------------------
 * number raised to power, e.g. '**', '^', or something similar
 *--------------------------------------------------------------------------*/

HEMP_SYMBOL_FUNC(hemp_element_number_power_symbol) {
    hemp_element_infix_right_symbol(hemp, symbol);
    symbol->value  =
    symbol->number = &hemp_element_number_power_value;
    return symbol;
}


HEMP_VALUE_FUNC(hemp_element_number_power_value) {
    hemp_debug_call("hemp_element_number_power_value()\n");

    hemp_element_p  element = hemp_val_elem(value);
    hemp_value_t    lhs     = hemp_lhs(element);
    hemp_value_t    rhs     = hemp_rhs(element);
    hemp_value_t    lval    = hemp_val_elem(lhs)->type->number(lhs, context);
    hemp_value_t    rval    = hemp_val_elem(rhs)->type->number(rhs, context);
    hemp_value_t    result;

    if (hemp_is_integer(lval)) {
        result = hemp_int_val(
            (hemp_int_t)
            pow( 
                (hemp_num_t) hemp_val_int(lval),
                HEMP_NUMOP_CAST_NUM(rval)
            )
        );
    }
    else {
        result = hemp_num_val(
            pow( 
                hemp_val_num(lval),
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
    hemp_element_infix_left_symbol(hemp, symbol);
    symbol->value  =
    symbol->number = &hemp_element_number_multiply_value;
    return symbol;
}


HEMP_VALUE_FUNC(hemp_element_number_multiply_value) {
    hemp_debug_call("hemp_element_number_multiply_value()\n");

    hemp_element_p  element = hemp_val_elem(value);
    hemp_value_t    lhs     = hemp_lhs(element);
    hemp_value_t    rhs     = hemp_rhs(element);
    hemp_value_t    lval    = hemp_val_elem(lhs)->type->number(lhs, context);
    hemp_value_t    rval, result;

    if (hemp_is_integer(lval)) {
        result = hemp_int_val(
            hemp_val_int(lval) 
          * HEMP_NUMOP_GET_INT(rhs, context)
        );
    }
    else {
        rval   = hemp_val_elem(rhs)->type->number(rhs, context);
        result = hemp_num_val(
            hemp_val_num(lval) 
          * HEMP_NUMOP_CAST_NUM(rval)
        );
    }

    return result;
}


/*--------------------------------------------------------------------------
 * number division, e.g. '/'
 *--------------------------------------------------------------------------*/

HEMP_SYMBOL_FUNC(hemp_element_number_divide_symbol) {
    hemp_element_infix_left_symbol(hemp, symbol);
    symbol->value  = 
    symbol->number = &hemp_element_number_divide_value;
    return symbol;
}


HEMP_VALUE_FUNC(hemp_element_number_divide_value) {
    hemp_debug_call("hemp_element_number_divide_value()\n");

    hemp_element_p  element = hemp_val_elem(value);
    hemp_value_t    lhs     = hemp_lhs(element);
    hemp_value_t    rhs     = hemp_rhs(element);
    hemp_value_t    lval    = hemp_val_elem(lhs)->type->number(lhs, context);
    hemp_value_t    rval, result;

    if (hemp_is_integer(lval)) {
//      hemp_debug("number is integer\n");
        result = hemp_int_val(
            hemp_val_int(lval) 
          / HEMP_NUMOP_GET_INT(rhs, context)
        );
    }
    else {
//      hemp_debug("number is real\n");
        rval   = hemp_val_elem(rhs)->type->number(rhs, context);
        result = hemp_num_val(
            hemp_val_num(lval) 
          / HEMP_NUMOP_CAST_NUM(rval)
        );
    }

    return result;
}


/*--------------------------------------------------------------------------
 * integer division forces both sides to integers, e.g. 'div'
 *--------------------------------------------------------------------------*/

HEMP_SYMBOL_FUNC(hemp_element_number_divint_symbol) {
    hemp_element_infix_left_symbol(hemp, symbol);
    symbol->value   =
    symbol->number  =
    symbol->integer = &hemp_element_number_divint_value;
    return symbol;
}


HEMP_VALUE_FUNC(hemp_element_number_divint_value) {
    hemp_debug_call("hemp_element_number_divint_value()\n");

    hemp_element_p  element = hemp_val_elem(value);
    hemp_value_t    lhs     = hemp_lhs(element);
    hemp_value_t    rhs     = hemp_rhs(element);
    
    return hemp_int_val(
        (hemp_int_t)
        hemp_val_int( hemp_val_elem(lhs)->type->integer(lhs, context) )
      / hemp_val_int( hemp_val_elem(rhs)->type->integer(rhs, context) )
    );
}


/*--------------------------------------------------------------------------
 * integer modulus, e.g. '%', 'mod', etc.
 *--------------------------------------------------------------------------*/

HEMP_SYMBOL_FUNC(hemp_element_number_modulus_symbol) {
    hemp_element_infix_left_symbol(hemp, symbol);
    symbol->value   =
    symbol->number  =
    symbol->integer = &hemp_element_number_modulus_value;
    return symbol;
}


HEMP_VALUE_FUNC(hemp_element_number_modulus_value) {
    hemp_debug_call("hemp_element_number_modulus_value()\n");

    hemp_element_p  element = hemp_val_elem(value);
    hemp_value_t    lhs     = hemp_lhs(element);
    hemp_value_t    rhs     = hemp_rhs(element);
    
    return hemp_int_val(
        (hemp_int_t)
        hemp_val_int( hemp_val_elem(lhs)->type->integer(lhs, context) )
      % hemp_val_int( hemp_val_elem(rhs)->type->integer(rhs, context) )
    );
}


/*--------------------------------------------------------------------------
 * number comparison, e.g. '<=>'
 *--------------------------------------------------------------------------*/

HEMP_SYMBOL_FUNC(hemp_element_number_compare_symbol) {
    hemp_element_compare_symbol(hemp, symbol);
    symbol->value   = 
    symbol->compare = &hemp_element_number_compare_value;
    return symbol;
}


HEMP_VALUE_FUNC(hemp_element_number_compare_value) {
    hemp_debug_call("hemp_element_number_compare_value()\n");

    hemp_element_p  element = hemp_val_elem(value);
    hemp_value_t    lhs     = hemp_lhs(element);
    hemp_value_t    rhs     = hemp_rhs(element);
    hemp_value_t    lval    = hemp_val_elem(lhs)->type->number(lhs, context);
    hemp_value_t    rval    = hemp_val_elem(rhs)->type->number(rhs, context);
    hemp_num_t      lnum    = HEMP_NUMOP_CAST_NUM(lval);
    hemp_num_t      rnum    = HEMP_NUMOP_CAST_NUM(rval);

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



