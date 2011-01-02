#include <math.h>
#include <hemp/element.h>

/* TODO: check for overflow/underflow/divide-by-zero/NaN/Infinity */

#define HEMP_NUMOP_GET_INT(v,c)                 \
    hemp_val_int(                               \
        hemp_val_frag(v)->type->integer(v, c)   \
    )

#define HEMP_NUMOP_CAST_NUM(v)              \
    (hemp_is_number(v)                      \
        ? hemp_val_num(v)                   \
        : ((HempNum) hemp_val_int(v))    \
    )


/*--------------------------------------------------------------------------
 * global element types
 *--------------------------------------------------------------------------*/

HempElement HempElementNumber  = NULL;
HempElement HempElementInteger = NULL;

HEMP_GLOBAL_ELEMENT(hemp_global_element_number) {
    return hemp_element_number(
        NULL,
        hemp_element_new("hemp.number", NULL, NULL)
    );
}


HEMP_GLOBAL_ELEMENT(hemp_global_element_integer) {
    return hemp_element_integer(
        NULL,
        hemp_element_new("hemp.integer", NULL, NULL)
    );
}



/*--------------------------------------------------------------------------
 * element to represent a literal number
 *--------------------------------------------------------------------------*/

HEMP_ELEMENT(hemp_element_number) {
    hemp_element_literal(hemp, element);
    element->parse_prefix   = &hemp_element_literal_prefix;
    element->value          = &hemp_element_number_value;
    element->number         = &hemp_element_number_value;
    element->integer        = &hemp_element_value_integer;
    element->boolean        = &hemp_element_value_boolean;
    element->compare        = &hemp_element_value_compare;
    element->flags          = HEMP_BE_SOURCE | HEMP_BE_FIXED;
    return element;
}


HEMP_VALUE(hemp_element_number_value) {
    hemp_debug_call("hemp_element_number_value()\n");
    return hemp_val_frag(value)->args.value;
}



/*--------------------------------------------------------------------------
 * element to represent a literal integer
 *--------------------------------------------------------------------------*/

HEMP_ELEMENT(hemp_element_integer) {
    hemp_element_number(hemp, element);
    element->value   = &hemp_element_integer_value;
    element->integer = &hemp_element_integer_value;
    element->number  = &hemp_element_integer_value;
    return element;
}


HEMP_VALUE(hemp_element_integer_value) {
    hemp_debug_call("hemp_element_integer_value()\n");
    return hemp_val_frag(value)->args.value;
}



/*--------------------------------------------------------------------------
 * number operators
 *--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------
 * auto-increment
 *--------------------------------------------------------------------------*/

HEMP_ELEMENT(hemp_element_number_autoinc) {
    hemp_element_prepostfix(hemp, element);
    element->value  =
    element->number = &hemp_element_number_autoinc_value;
    return element;
}


HEMP_VALUE(hemp_element_number_autoinc_value) {
    hemp_todo("hemp_element_number_autoinc_value()\n");
    return HempMissing;
}


/*--------------------------------------------------------------------------
 * auto-decrement
 *--------------------------------------------------------------------------*/

HEMP_ELEMENT(hemp_element_number_autodec) {
    hemp_element_prepostfix(hemp, element);
    element->value  =
    element->number = &hemp_element_number_autodec_value;
    return element;
}


HEMP_VALUE(hemp_element_number_autodec_value) {
    hemp_todo("hemp_element_number_autodec_value()\n");
    return HempMissing;
}


/*--------------------------------------------------------------------------
 * number plus, e.g. '+'
 *--------------------------------------------------------------------------*/

HEMP_ELEMENT(hemp_element_number_plus) {
    hemp_element_infix_left(hemp, element);
    /* also works as prefix operator */
    element->parse_prefix    = &hemp_element_parse_prefix;
    element->value           =
    element->number          = &hemp_element_number_plus_value;
    return element;
}


HEMP_VALUE(hemp_element_number_plus_value) {
    hemp_debug_call("hemp_element_number_plus_value()\n");

    HempFragment fragment = hemp_val_frag(value);

    /* prefix unary '+' coerces value to a number */
    if (hemp_has_flag(fragment, HEMP_BE_PREFIX)) {
        HempValue expr = hemp_expr(fragment);
        return hemp_obcall(expr, number, context);
    }

    /* otherwise it's an infix addition operator */
    HempValue lhs  = hemp_lhs(fragment);
    HempValue rhs  = hemp_rhs(fragment);
    HempValue lval = hemp_obcall(lhs, number, context);
    HempValue rval, result;
    
    if (hemp_is_integer(lval)) {
        rval   = hemp_obcall(rhs, integer, context);
        result = hemp_int_val(
            hemp_val_int(lval) 
          + hemp_val_int(rval)
        );
    }
    else {
        rval   = hemp_obcall(rhs, number, context);
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

HEMP_ELEMENT(hemp_element_number_minus) {
    hemp_element_infix_left(hemp, element);
    /* also works as prefix operator */
    element->parse_prefix    = &hemp_element_parse_prefix;
    element->value           =
    element->number          = &hemp_element_number_minus_value;
    return element;
}


HEMP_VALUE(hemp_element_number_minus_value) {
    hemp_debug_call("hemp_element_number_minus_value()\n");

    HempFragment fragment = hemp_val_frag(value);

    /* prefix unary '-' coerces value to a number and negates it */
    if (hemp_has_flag(fragment, HEMP_BE_PREFIX)) {
        HempValue expr = hemp_expr(fragment);
        HempValue val  = hemp_obcall(expr, number, context);

        return hemp_is_integer(val)
            ? hemp_int_val(- hemp_val_int(val) )
            : hemp_num_val(- hemp_val_num(val) );
    }

    /* otherwise it's an infix subtraction operator */
    HempValue lhs  = hemp_lhs(fragment);
    HempValue rhs  = hemp_rhs(fragment);
    HempValue lval = hemp_obcall(lhs, number, context);
    HempValue rval, result;

    if (hemp_is_integer(lval)) {
        result = hemp_int_val(
            hemp_val_int(lval) 
          - HEMP_NUMOP_GET_INT(rhs, context)
        );
    }
    else {
        rval   = hemp_obcall(rhs, number, context);
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

HEMP_ELEMENT(hemp_element_number_power) {
    hemp_element_infix_right(hemp, element);
    element->value  =
    element->number = &hemp_element_number_power_value;
    return element;
}


HEMP_VALUE(hemp_element_number_power_value) {
    hemp_debug_call("hemp_element_number_power_value()\n");

    HempFragment fragment  = hemp_val_frag(value);
    HempValue    lhs       = hemp_lhs(fragment);
    HempValue    rhs       = hemp_rhs(fragment);
    HempValue    lval      = hemp_obcall(lhs, number, context);
    HempValue    rval      = hemp_obcall(rhs, number, context);
    HempValue    result;

    if (hemp_is_integer(lval)) {
        result = hemp_int_val(
            (HempInt)
            pow( 
                (HempNum) hemp_val_int(lval),
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

HEMP_ELEMENT(hemp_element_number_multiply) {
    hemp_element_infix_left(hemp, element);
    element->value  =
    element->number = &hemp_element_number_multiply_value;
    return element;
}


HEMP_VALUE(hemp_element_number_multiply_value) {
    hemp_debug_call("hemp_element_number_multiply_value()\n");

    HempFragment fragment = hemp_val_frag(value);
    HempValue    lhs      = hemp_lhs(fragment);
    HempValue    rhs      = hemp_rhs(fragment);
    HempValue    lval     = hemp_obcall(lhs, number, context);
    HempValue    rval, result;

    if (hemp_is_integer(lval)) {
        result = hemp_int_val(
            hemp_val_int(lval) 
          * HEMP_NUMOP_GET_INT(rhs, context)
        );
    }
    else {
        rval   = hemp_obcall(rhs, number, context);
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

HEMP_ELEMENT(hemp_element_number_divide) {
    hemp_element_infix_left(hemp, element);
    element->value  = 
    element->number = &hemp_element_number_divide_value;
    return element;
}


HEMP_VALUE(hemp_element_number_divide_value) {
    hemp_debug_call("hemp_element_number_divide_value()\n");

    HempFragment fragment = hemp_val_frag(value);
    HempValue    lhs      = hemp_lhs(fragment);
    HempValue    rhs      = hemp_rhs(fragment);
    HempValue    lval     = hemp_obcall(lhs, number, context);
    HempValue    rval, result;

    if (hemp_is_integer(lval)) {
//      hemp_debug("number is integer\n");
        result = hemp_int_val(
            hemp_val_int(lval) 
          / HEMP_NUMOP_GET_INT(rhs, context)
        );
    }
    else {
//      hemp_debug("number is real\n");
        rval   = hemp_obcall(rhs, number, context);
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

HEMP_ELEMENT(hemp_element_number_divint) {
    hemp_element_infix_left(hemp, element);
    element->value   =
    element->number  =
    element->integer = &hemp_element_number_divint_value;
    return element;
}


HEMP_VALUE(hemp_element_number_divint_value) {
    hemp_debug_call("hemp_element_number_divint_value()\n");

    HempFragment fragment = hemp_val_frag(value);
    HempValue    lhs      = hemp_lhs(fragment);
    HempValue    rhs      = hemp_rhs(fragment);
    
    return hemp_int_val(
        (HempInt)
        hemp_val_int( hemp_obcall(lhs, integer, context) )
      / hemp_val_int( hemp_obcall(rhs, integer, context) )
    );
}


/*--------------------------------------------------------------------------
 * integer modulus, e.g. '%', 'mod', etc.
 *--------------------------------------------------------------------------*/

HEMP_ELEMENT(hemp_element_number_modulus) {
    hemp_element_infix_left(hemp, element);
    element->value   =
    element->number  =
    element->integer = &hemp_element_number_modulus_value;
    return element;
}


HEMP_VALUE(hemp_element_number_modulus_value) {
    hemp_debug_call("hemp_element_number_modulus_value()\n");

    HempFragment fragment = hemp_val_frag(value);
    HempValue    lhs      = hemp_lhs(fragment);
    HempValue    rhs      = hemp_rhs(fragment);
    
    return hemp_int_val(
        (HempInt)
        hemp_val_int( hemp_obcall(lhs, integer, context) )
      % hemp_val_int( hemp_obcall(rhs, integer, context) )
    );
}


/*--------------------------------------------------------------------------
 * number comparison, e.g. '<=>'
 *--------------------------------------------------------------------------*/

HEMP_ELEMENT(hemp_element_number_compare) {
    hemp_element_compare(hemp, element);
    element->value   = 
    element->compare = &hemp_element_number_compare_value;
    return element;
}


HEMP_VALUE(hemp_element_number_compare_value) {
    hemp_debug_call("hemp_element_number_compare_value()\n");

    HempFragment fragment = hemp_val_frag(value);
    HempValue    lhs      = hemp_lhs(fragment);
    HempValue    rhs      = hemp_rhs(fragment);
    HempValue    lval     = hemp_obcall(lhs, number, context);
    HempValue    rval     = hemp_obcall(rhs, number, context);
    HempNum      lnum     = HEMP_NUMOP_CAST_NUM(lval);
    HempNum      rnum     = HEMP_NUMOP_CAST_NUM(rval);

    return  lnum < rnum ? HempBefore
        :   lnum > rnum ? HempAfter
        :                 HempEqual;
}


/*--------------------------------------------------------------------------
 * number equality, e.g. '=='
 *--------------------------------------------------------------------------*/

HEMP_ELEMENT(hemp_element_number_equal) {
    hemp_element_compare_equal(hemp, element);
    element->compare = &hemp_element_number_compare_value;
    return element;
}


/*--------------------------------------------------------------------------
 * number inequality, e.g. '!=', '<>', etc.
 *--------------------------------------------------------------------------*/

HEMP_ELEMENT(hemp_element_number_not_equal) {
    hemp_element_compare_not_equal(hemp, element);
    element->compare = &hemp_element_number_compare_value;
    return element;
}


/*--------------------------------------------------------------------------
 * number less than, e.g. '<'
 *--------------------------------------------------------------------------*/

HEMP_ELEMENT(hemp_element_number_before) {
    hemp_element_compare_before(hemp, element);
    element->compare = &hemp_element_number_compare_value;
    return element;
}


/*--------------------------------------------------------------------------
 * number less than or equal to, e.g. '<=', '!>', etc
 *--------------------------------------------------------------------------*/

HEMP_ELEMENT(hemp_element_number_not_after) {
    hemp_element_compare_not_after(hemp, element);
    element->compare = &hemp_element_number_compare_value;
    return element;
}


/*--------------------------------------------------------------------------
 * number more than, e.g. '>'
 *--------------------------------------------------------------------------*/

HEMP_ELEMENT(hemp_element_number_after) {
    hemp_element_compare_after(hemp, element);
    element->compare = &hemp_element_number_compare_value;
    return element;
}


/*--------------------------------------------------------------------------
 * number more than more than or equal to, e.g. '>=', '!<', etc.
 *--------------------------------------------------------------------------*/

HEMP_ELEMENT(hemp_element_number_not_before) {
    hemp_element_compare_not_before(hemp, element);
    element->compare = &hemp_element_number_compare_value;
    return element;
}



