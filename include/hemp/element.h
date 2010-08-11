#ifndef HEMP_ELEMENT_H
#define HEMP_ELEMENT_H

#include <hemp/type.h>
#include <hemp/text.h>
#include <hemp/tag.h>
#include <hemp/symbol.h>
#include <hemp/template.h>
#include <hemp/context.h>
#include <hemp/value.h>
#include <hemp/macros.h>



/*--------------------------------------------------------------------------
 * data structures
 *--------------------------------------------------------------------------*/

struct hemp_unary_s {
    hemp_element_p  expr;
    // TODO: args
};

struct hemp_binary_s {
    hemp_element_p  lhs;
    hemp_element_p  rhs;
};

struct hemp_block_s {
    hemp_list_p     exprs;
};

typedef union hemp_eargs_u {
    hemp_value_t    value;
    hemp_block_t    block;
    hemp_unary_t    unary;
    hemp_binary_t   binary;
} hemp_eargs_t;

struct hemp_element_s {
    hemp_symbol_p   type;
    hemp_element_p  next;
    hemp_cstr_p     token;
    hemp_pos_t      position;
    hemp_size_t     length;
    hemp_flags_t    flags;
    hemp_eargs_t    args;
};


/*--------------------------------------------------------------------------
 * external static element types
 *--------------------------------------------------------------------------*/

extern hemp_symbol_p HempSymbolSpace;
extern hemp_symbol_p HempSymbolComment;
extern hemp_symbol_p HempSymbolTagStart;
extern hemp_symbol_p HempSymbolTagEnd;
extern hemp_symbol_p HempSymbolEof;
extern hemp_symbol_p HempSymbolText;
extern hemp_symbol_p HempSymbolWord;
extern hemp_symbol_p HempSymbolNumber;
extern hemp_symbol_p HempSymbolInteger;
extern hemp_symbol_p HempSymbolBlock;


/*--------------------------------------------------------------------------
 * macros
 *--------------------------------------------------------------------------*/

#define hemp_element_null(e) \
    hemp_element_free(e);    \
    e = NULL;

#define hemp_parse_expr(ep, sc, pr, fr)     \
    (hemp_element_p) (*ep)->type->expr(ep, sc, pr, fr)

#define hemp_parse_infix(ep, sc, pr, fr, lhs)           \
    ((*ep)->type->infix)                                \
        ? (*ep)->type->infix(ep, sc, pr, fr, lhs)       \
        : lhs

#define hemp_set_flag(item, flag) \
    item->flags |= flag

#define hemp_clear_flag(flags, flag) \
    item->flags &= ~flag

#define hemp_has_flag(item, flag) \
    item->flags & flag

#define hemp_not_flag(item, flag) \
    ! (hemp_has_flag(item, flag))

#define hemp_has_next(ep) \
    (*ep)->next

#define hemp_go_next(ep) \
    *ep = (*ep)->next

#define hemp_skip_while(ep, flag)               \
    while( hemp_has_next(ep)                    \
        && hemp_has_flag((*ep)->type, flag) )   \
            hemp_go_next(ep);

#define hemp_skip_whitespace(ep)                \
       hemp_skip_while(ep, HEMP_BE_WHITESPACE)
       
#define hemp_skip_separator(ep)                 \
       hemp_skip_while(ep, HEMP_BE_SEPARATOR)

#define hemp_skip_terminator(ep)                \
       hemp_skip_while(ep, HEMP_BE_TERMINATOR)

#define hemp_at_eof(ep) \
    (*ep)->type == HempSymbolEof


#define hemp_prepare_output(output, text, length)   \
    if (HEMP_IS_UNDEF(output)) {                    \
        text   = hemp_text_init(length);            \
        output = HEMP_TEXT_VAL(text);               \
    }                                               \
    else {                                          \
        text   = HEMP_VAL_TEXT(output);             \
    }

/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

hemp_element_p
    hemp_element_new();

hemp_element_p
    hemp_element_init(
        hemp_element_p,
        hemp_symbol_p, 
        hemp_cstr_p, 
        hemp_pos_t, 
        hemp_size_t
    );

void
    hemp_element_free(
        hemp_element_p element
    );


/*--------------------------------------------------------------------------
 * general parsing functions
 *--------------------------------------------------------------------------*/

hemp_element_p  hemp_element_parse(hemp_element_p);
hemp_list_p     hemp_element_parse_exprs(HEMP_PARSE_ARGS);

HEMP_PARSE_FUNC(hemp_element_parse_block);
HEMP_PARSE_FUNC(hemp_element_parse_expr);


/*--------------------------------------------------------------------------
 * decline functions
 *--------------------------------------------------------------------------*/

HEMP_PARSE_FUNC(hemp_element_not_expr);
HEMP_INFIX_FUNC(hemp_element_not_infix);
HEMP_OUTPUT_FUNC(hemp_element_not_source);
HEMP_OUTPUT_FUNC(hemp_element_not_text);
HEMP_VALUE_FUNC(hemp_element_not_number);
HEMP_VALUE_FUNC(hemp_element_not_integer);
HEMP_VALUE_FUNC(hemp_element_not_boolean);


/*--------------------------------------------------------------------------
 * delegation functions
 *--------------------------------------------------------------------------*/

HEMP_PARSE_FUNC(hemp_element_next_expr);
HEMP_INFIX_FUNC(hemp_element_next_infix);


/*--------------------------------------------------------------------------
 * expression parsing methods
 *--------------------------------------------------------------------------*/

HEMP_INFIX_FUNC(hemp_element_parse_infix_left);
HEMP_INFIX_FUNC(hemp_element_parse_infix_right);


/*--------------------------------------------------------------------------
 * literal elements
 *--------------------------------------------------------------------------*/

HEMP_PARSE_FUNC(hemp_element_literal_expr);
HEMP_OUTPUT_FUNC(hemp_element_literal_source);
HEMP_OUTPUT_FUNC(hemp_element_literal_text);
HEMP_VALUE_FUNC(hemp_element_literal_number);
HEMP_VALUE_FUNC(hemp_element_literal_integer);
HEMP_VALUE_FUNC(hemp_element_literal_boolean);


/*--------------------------------------------------------------------------
 * comments
 *--------------------------------------------------------------------------*/

HEMP_SYMBOL_FUNC(hemp_element_comment_symbol);
HEMP_SCAN_FUNC(hemp_element_comment_scanner);


/*--------------------------------------------------------------------------
 * quoted strings
 *--------------------------------------------------------------------------*/

HEMP_SYMBOL_FUNC(hemp_element_squote_symbol);
HEMP_SCAN_FUNC(hemp_element_squote_scanner);

HEMP_SYMBOL_FUNC(hemp_element_dquote_symbol);
HEMP_SCAN_FUNC(hemp_element_dquote_scanner);

HEMP_OUTPUT_FUNC(hemp_element_quoted_text);


/*--------------------------------------------------------------------------
 * boolean elements
 *--------------------------------------------------------------------------*/

HEMP_OUTPUT_FUNC(hemp_element_boolop_text);


/*--------------------------------------------------------------------------
 * number elements
 *--------------------------------------------------------------------------*/

HEMP_PARSE_FUNC(hemp_element_number_expr);

HEMP_OUTPUT_FUNC(hemp_element_number_text);
HEMP_VALUE_FUNC(hemp_element_number_number);
HEMP_VALUE_FUNC(hemp_element_number_integer);
HEMP_VALUE_FUNC(hemp_element_number_boolean);

HEMP_OUTPUT_FUNC(hemp_element_integer_text);
HEMP_VALUE_FUNC(hemp_element_integer_number);
HEMP_VALUE_FUNC(hemp_element_integer_integer);
HEMP_VALUE_FUNC(hemp_element_integer_boolean);


/*--------------------------------------------------------------------------
 * number operator elements
 *--------------------------------------------------------------------------*/

HEMP_OUTPUT_FUNC(hemp_element_numop_text);
HEMP_VALUE_FUNC(hemp_element_numop_integer);

HEMP_SYMBOL_FUNC(hemp_element_numop_plus_symbol);
HEMP_VALUE_FUNC(hemp_element_numop_plus_value);

HEMP_SYMBOL_FUNC(hemp_element_numop_minus_symbol);
HEMP_VALUE_FUNC(hemp_element_numop_minus_value);

HEMP_SYMBOL_FUNC(hemp_element_numop_power_symbol);
HEMP_VALUE_FUNC(hemp_element_numop_power_value);
HEMP_SYMBOL_FUNC(hemp_element_numop_multiply_symbol);
HEMP_VALUE_FUNC(hemp_element_numop_multiply_value);
HEMP_SYMBOL_FUNC(hemp_element_numop_divide_symbol);
HEMP_VALUE_FUNC(hemp_element_numop_divide_value);
HEMP_SYMBOL_FUNC(hemp_element_numop_divint_symbol);
HEMP_VALUE_FUNC(hemp_element_numop_divint_value);
HEMP_SYMBOL_FUNC(hemp_element_numop_modulus_symbol);
HEMP_VALUE_FUNC(hemp_element_numop_modulus_value);

HEMP_SYMBOL_FUNC(hemp_element_numop_compare_symbol);
HEMP_VALUE_FUNC(hemp_element_numop_compare_value);
HEMP_SYMBOL_FUNC(hemp_element_numop_equal_symbol);
HEMP_VALUE_FUNC(hemp_element_numop_equal_value);
HEMP_SYMBOL_FUNC(hemp_element_numop_not_equal_symbol);
HEMP_VALUE_FUNC(hemp_element_numop_not_equal_value);
HEMP_SYMBOL_FUNC(hemp_element_numop_less_than_symbol);
HEMP_VALUE_FUNC(hemp_element_numop_less_than_value);
HEMP_SYMBOL_FUNC(hemp_element_numop_less_equal_symbol);
HEMP_VALUE_FUNC(hemp_element_numop_less_equal_value);
HEMP_SYMBOL_FUNC(hemp_element_numop_more_than_symbol);
HEMP_VALUE_FUNC(hemp_element_numop_more_than_value);
HEMP_SYMBOL_FUNC(hemp_element_numop_more_equal_symbol);
HEMP_VALUE_FUNC(hemp_element_numop_more_equal_value);



/*--------------------------------------------------------------------------
 * function prototypes for specific element types
 *--------------------------------------------------------------------------*/

HEMP_PARSE_FUNC(hemp_element_space_parse_expr);
HEMP_OUTPUT_FUNC(hemp_element_block_source);
HEMP_OUTPUT_FUNC(hemp_element_block_text);
HEMP_OUTPUT_FUNC(hemp_element_binary_text);
HEMP_OUTPUT_FUNC(hemp_element_eof_text);


void
    hemp_element_text_clean(
        hemp_element_p element
    );

void
hemp_element_block_clean(
    hemp_element_p element
);

hemp_bool_t
    hemp_element_dump(
        hemp_element_p element
    );


#endif /* HEMP_ELEMENT_H */