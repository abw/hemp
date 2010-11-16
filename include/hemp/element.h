#ifndef HEMP_ELEMENT_H
#define HEMP_ELEMENT_H

#include <hemp/type.h>
#include <hemp/tag.h>
#include <hemp/symbol.h>
#include <hemp/template.h>
#include <hemp/context.h>
#include <hemp/value.h>
#include <hemp/macros.h>
#include <hemp/type/text.h>



/*--------------------------------------------------------------------------
 * data structures
 *--------------------------------------------------------------------------*/

struct hemp_unary_s {
    hemp_value_t    expr;
    // TODO: args
};

struct hemp_binary_s {
    hemp_value_t    lhs;
    hemp_value_t    rhs;
};

struct hemp_block_s {
    hemp_value_t    args;
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
    hemp_str_p      token;
    hemp_pos_t      position;
    hemp_size_t     length;
    hemp_flags_t    flags;
    hemp_eargs_t    args;
};


/*--------------------------------------------------------------------------
 * macros
 *--------------------------------------------------------------------------*/

#define hemp_set_flag(item, flag) \
    item->flags |= (flag)

#define hemp_clear_flag(flags, flag) \
    item->flags &= ~(flag)

#define hemp_has_flag(item, flag) \
    item->flags & (flag)

#define hemp_not_flag(item, flag) \
    ! (hemp_has_flag(item, flag))

#define hemp_has_next(ep) \
    (*ep)->next

#define hemp_go_next(ep) \
    (*ep = (*ep)->next)

#define hemp_skip_while(ep, flag)                       \
    while( hemp_has_next(ep)                            \
        && hemp_has_flag((*ep)->type, flag) )           \
            hemp_go_next(ep);

#define hemp_skip_whitespace(ep)                        \
    hemp_skip_while(ep, HEMP_BE_WHITESPACE)

#define hemp_skip_delimiter(ep)                         \
    hemp_skip_while(ep, HEMP_BE_DELIMITER|HEMP_BE_WHITESPACE)

#define hemp_skip_separator(ep)                         \
    hemp_skip_while(ep, HEMP_BE_SEPARATOR|HEMP_BE_DELIMITER|HEMP_BE_WHITESPACE)

#define hemp_skip_terminator(ep)                        \
    hemp_skip_while(ep, HEMP_BE_TERMINATOR|HEMP_BE_SEPARATOR|HEMP_BE_DELIMITER|HEMP_BE_WHITESPACE)

#define hemp_at_eof(ep) \
    (*ep)->type == HempSymbolEOF

#define hemp_expr(elem)                                         \
    (elem->args.unary.expr)

#define hemp_lhs(elem)                                          \
    (elem->args.binary.lhs)

#define hemp_rhs(elem)                                          \
    (elem->args.binary.rhs)

#define hemp_block_name(elem)                                   \
    (elem->args.block.name)

#define hemp_block_args(elem)                                   \
    (elem->args.block.args)

#define hemp_block_exprs(elem)                                  \
    (elem->args.block.exprs)

#define hemp_expr_element(elem)                                 \
    ((hemp_element_p) hemp_val_obj( hemp_expr(elem) ))

#define hemp_lhs_element(elem)                                  \
    ((hemp_element_p) hemp_val_obj( hemp_lhs(elem) ))

#define hemp_rhs_element(elem)                                  \
    ((hemp_element_p) hemp_val_obj( hemp_rhs(elem) ))

#define hemp_set_expr(elem, ex)                                 \
    (elem->args.unary.expr = ex)

#define hemp_set_lhs(elem, ex)                                  \
    (elem->args.binary.lhs = ex)

#define hemp_set_rhs(elem, ex)                                  \
    (elem->args.binary.rhs = ex)

#define hemp_set_block_name(elem, ex)                           \
    (elem->args.block.name = ex)

#define hemp_set_block_args(elem, ex)                           \
    (elem->args.block.args = ex)

#define hemp_set_block_exprs(elem, ex)                          \
    (elem->args.block.exprs = ex)

#define hemp_set_expr_element(elem, ex)                         \
    (hemp_set_expr( elem, hemp_obj_val((hemp_object_p) ex) ))

#define hemp_set_lhs_element(elem, ex)                          \
    (hemp_set_lhs( elem, hemp_obj_val((hemp_object_p) ex) ))

#define hemp_set_rhs_element(elem, ex)                          \
    (hemp_set_rhs( elem, hemp_obj_val((hemp_object_p) ex) ))

#define hemp_parse_prefix(ep, sc, pr, fr)               \
    ((*ep)->type->prefix                                \
        ? (*ep)->type->prefix(ep, sc, pr, fr)           \
        : NULL                                          \
    )

#define hemp_parse_postfix(ep, sc, pr, fr, lhs)         \
    ((*ep)->type->postfix                               \
        ? (*ep)->type->postfix(ep, sc, pr, fr, lhs)     \
        : lhs                                           \
    )

#define hemp_parse_fixed(ep, sc, pr, fr)                \
    ((*ep)->type->fixed                                 \
        ? (*ep)->type->fixed(ep, sc, pr, fr)            \
        : NULL                                          \
    )

#define hemp_parse_params(ep, sc, pr, fr, lhs)              \
    ((*ep)->type->parse_params                              \
        ? (*ep)->type->parse_params(ep, sc, pr, fr, lhs)    \
        : NULL                                              \
    )

//#define hemp_parse_infix_rhs(self, ep, sc, pr, fr)      \
//    hemp_go_next(ep);                                   \
//    self->args.binary.rhs = hemp_parse_prefix(            \
//        ep, sc, self->type->lprec, 1                    \
//    );                                                  \
//    if (! self->args.binary.rhs)                        \
//        hemp_fatal(                                     \
//            "missing expression on rhs of %s\n",        \
//            self->type->start                           \
//        );
//

/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

hemp_element_p
    hemp_element_new();

hemp_element_p
    hemp_element_init(
        hemp_element_p,
        hemp_symbol_p, 
        hemp_str_p, 
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

hemp_element_p  hemp_element_parse(hemp_element_p, hemp_scope_p);
hemp_list_p     hemp_element_parse_exprs(HEMP_PREFIX_ARGS);

HEMP_PREFIX_FUNC(hemp_element_parse_block);
//HEMP_PREFIX_FUNC(hemp_element_parse_expr);
HEMP_PREFIX_FUNC(hemp_element_parse_prefix);         // TODO: s/PARSE/PREFIX
HEMP_POSTFIX_FUNC(hemp_element_parse_postfix);        // TODO: s/INFIX/POSTFIX
HEMP_POSTFIX_FUNC(hemp_element_parse_infix_left);
HEMP_POSTFIX_FUNC(hemp_element_parse_infix_right);
HEMP_PREFIX_FUNC(hemp_element_fixed);

HEMP_OUTPUT_FUNC(hemp_element_binary_source);


/*--------------------------------------------------------------------------
 * decline functions
 *--------------------------------------------------------------------------*/

HEMP_PREFIX_FUNC(hemp_element_decline);
HEMP_PREFIX_FUNC(hemp_element_not_prefix);
HEMP_POSTFIX_FUNC(hemp_element_not_postfix);
HEMP_PREFIX_FUNC(hemp_element_not_word);
HEMP_OUTPUT_FUNC(hemp_element_not_token);
HEMP_OUTPUT_FUNC(hemp_element_not_source);
HEMP_OUTPUT_FUNC(hemp_element_not_text);
HEMP_VALUE_FUNC(hemp_element_not_value);
HEMP_VALUE_FUNC(hemp_element_not_number);
HEMP_VALUE_FUNC(hemp_element_not_integer);
HEMP_VALUE_FUNC(hemp_element_not_boolean);
HEMP_VALUE_FUNC(hemp_element_not_compare);
HEMP_OUTPUT_FUNC(hemp_element_not_values);
HEMP_OPERATE_FUNC(hemp_element_not_assign);


/*--------------------------------------------------------------------------
 * delegation functions
 *--------------------------------------------------------------------------*/

HEMP_PREFIX_FUNC(hemp_element_next_prefix);
HEMP_POSTFIX_FUNC(hemp_element_next_postfix);


/*--------------------------------------------------------------------------
 * literal elements
 *--------------------------------------------------------------------------*/

HEMP_SYMBOL_FUNC(hemp_element_literal_symbol);
HEMP_PREFIX_FUNC(hemp_element_literal_prefix);
HEMP_OUTPUT_FUNC(hemp_element_literal_token);
HEMP_OUTPUT_FUNC(hemp_element_literal_source);
HEMP_OUTPUT_FUNC(hemp_element_literal_text);
HEMP_VALUE_FUNC(hemp_element_literal_value);

HEMP_SYMBOL_FUNC(hemp_element_word_symbol);
HEMP_PREFIX_FUNC(hemp_element_word_prefix);
HEMP_PREFIX_FUNC(hemp_element_word_word);
HEMP_VALUE_FUNC(hemp_element_word_value);
HEMP_OPERATE_FUNC(hemp_element_word_assign);


/*--------------------------------------------------------------------------
 * number/integer elements
 *--------------------------------------------------------------------------*/

HEMP_SYMBOL_FUNC(hemp_element_number_symbol);
HEMP_PREFIX_FUNC(hemp_element_number_prefix);
HEMP_PREFIX_FUNC(hemp_element_number_word);
HEMP_OUTPUT_FUNC(hemp_element_number_text);
HEMP_VALUE_FUNC(hemp_element_number_value);
HEMP_VALUE_FUNC(hemp_element_number_number);
HEMP_VALUE_FUNC(hemp_element_number_integer);
HEMP_VALUE_FUNC(hemp_element_number_boolean);
HEMP_VALUE_FUNC(hemp_element_number_compare);

HEMP_SYMBOL_FUNC(hemp_element_integer_symbol);
HEMP_OUTPUT_FUNC(hemp_element_integer_text);
HEMP_VALUE_FUNC(hemp_element_integer_value);
HEMP_VALUE_FUNC(hemp_element_integer_number);
HEMP_VALUE_FUNC(hemp_element_integer_integer);
HEMP_VALUE_FUNC(hemp_element_integer_boolean);
HEMP_VALUE_FUNC(hemp_element_integer_compare);


/*--------------------------------------------------------------------------
 * whitespace, comments, etc
 *--------------------------------------------------------------------------*/

HEMP_SYMBOL_FUNC(hemp_element_space_symbol);
HEMP_SYMBOL_FUNC(hemp_element_comment_symbol);
HEMP_SYMBOL_FUNC(hemp_element_tag_start_symbol);
HEMP_SYMBOL_FUNC(hemp_element_tag_end_symbol);
HEMP_SYMBOL_FUNC(hemp_element_eof_symbol);
HEMP_SYMBOL_FUNC(hemp_element_delimiter_symbol);
HEMP_SYMBOL_FUNC(hemp_element_separator_symbol);
HEMP_SYMBOL_FUNC(hemp_element_terminator_symbol);

hemp_bool_t hemp_element_terminator_matches(hemp_element_p, hemp_str_p);


HEMP_SCAN_FUNC(hemp_element_comment_scanner);
HEMP_PREFIX_FUNC(hemp_element_space_parse_expr);
HEMP_OUTPUT_FUNC(hemp_element_eof_token);


/*--------------------------------------------------------------------------
 * text elements
 *--------------------------------------------------------------------------*/

HEMP_SYMBOL_FUNC(hemp_element_text_symbol);
HEMP_PREFIX_FUNC(hemp_element_text_prefix);


/*--------------------------------------------------------------------------
 * quoted strings
 *--------------------------------------------------------------------------*/

HEMP_SYMBOL_FUNC(hemp_element_squote_symbol);
HEMP_SCAN_FUNC(hemp_element_squote_scanner);

HEMP_SYMBOL_FUNC(hemp_element_dquote_symbol);
HEMP_SCAN_FUNC(hemp_element_dquote_scanner);

HEMP_OUTPUT_FUNC(hemp_element_quoted_text);
HEMP_VALUE_FUNC(hemp_element_quoted_value);


/*--------------------------------------------------------------------------
 * generic operators
 *--------------------------------------------------------------------------*/

HEMP_SYMBOL_FUNC(hemp_element_operator_symbol);
HEMP_SYMBOL_FUNC(hemp_element_prefix_symbol);
HEMP_SYMBOL_FUNC(hemp_element_postfix_symbol);
HEMP_SYMBOL_FUNC(hemp_element_prepostfix_symbol);
HEMP_SYMBOL_FUNC(hemp_element_infix_symbol);
HEMP_SYMBOL_FUNC(hemp_element_infix_left_symbol);
HEMP_SYMBOL_FUNC(hemp_element_infix_right_symbol);

HEMP_OUTPUT_FUNC(hemp_element_prefix_source);
HEMP_OUTPUT_FUNC(hemp_element_postfix_source);
HEMP_OUTPUT_FUNC(hemp_element_prepostfix_source);
HEMP_OUTPUT_FUNC(hemp_element_infix_source);



/*--------------------------------------------------------------------------
 * brackets
 *--------------------------------------------------------------------------*/

HEMP_SYMBOL_FUNC(hemp_element_brackets_symbol);
HEMP_PREFIX_FUNC(hemp_element_brackets_prefix);
HEMP_POSTFIX_FUNC(hemp_element_brackets_postfix);

HEMP_SYMBOL_FUNC(hemp_element_parens_symbol);
//HEMP_PREFIX_FUNC(hemp_element_parens_prefix);
//HEMP_POSTFIX_FUNC(hemp_element_parens_postfix);
HEMP_VALUE_FUNC(hemp_element_parens_value);
HEMP_OUTPUT_FUNC(hemp_element_parens_values);
HEMP_POSTFIX_FUNC(hemp_element_parens_postfix);
HEMP_POSTFIX_FUNC(hemp_element_parens_parse_params);

HEMP_SYMBOL_FUNC(hemp_element_list_symbol);
HEMP_VALUE_FUNC(hemp_element_list_value);

HEMP_SYMBOL_FUNC(hemp_element_hash_symbol);
HEMP_VALUE_FUNC(hemp_element_hash_value);



/*--------------------------------------------------------------------------
 * boolean operators
 *--------------------------------------------------------------------------*/

HEMP_OUTPUT_FUNC(hemp_element_boolean_text);
HEMP_SYMBOL_FUNC(hemp_element_boolean_symbol);
HEMP_VALUE_FUNC(hemp_element_boolean_value);

HEMP_SYMBOL_FUNC(hemp_element_boolean_not_symbol);
HEMP_VALUE_FUNC(hemp_element_boolean_not_value);

HEMP_SYMBOL_FUNC(hemp_element_boolean_and_symbol);
HEMP_VALUE_FUNC(hemp_element_boolean_and_value);

HEMP_SYMBOL_FUNC(hemp_element_boolean_or_symbol);
HEMP_VALUE_FUNC(hemp_element_boolean_or_value);



/*--------------------------------------------------------------------------
 * generic element comparison functions
 *--------------------------------------------------------------------------*/

HEMP_SYMBOL_FUNC(hemp_element_compare_symbol);
HEMP_OUTPUT_FUNC(hemp_element_compare_text);
HEMP_VALUE_FUNC(hemp_element_compare_integer);
HEMP_VALUE_FUNC(hemp_element_compare_value);

HEMP_SYMBOL_FUNC(hemp_element_compare_equal_symbol);
HEMP_SYMBOL_FUNC(hemp_element_compare_not_equal_symbol);
HEMP_SYMBOL_FUNC(hemp_element_compare_before_symbol);
HEMP_SYMBOL_FUNC(hemp_element_compare_not_before_symbol);
HEMP_SYMBOL_FUNC(hemp_element_compare_after_symbol);
HEMP_SYMBOL_FUNC(hemp_element_compare_not_after_symbol);

HEMP_VALUE_FUNC(hemp_element_compare_equal_value);
HEMP_VALUE_FUNC(hemp_element_compare_not_equal_value);
HEMP_VALUE_FUNC(hemp_element_compare_before_value);
HEMP_VALUE_FUNC(hemp_element_compare_not_before_value);
HEMP_VALUE_FUNC(hemp_element_compare_after_value);
HEMP_VALUE_FUNC(hemp_element_compare_not_after_value);


/*--------------------------------------------------------------------------
 * number operator elements
 *--------------------------------------------------------------------------*/

HEMP_SYMBOL_FUNC(hemp_element_numop_symbol);
HEMP_OUTPUT_FUNC(hemp_element_numop_text);
HEMP_VALUE_FUNC(hemp_element_numop_value);
HEMP_VALUE_FUNC(hemp_element_numop_integer);
HEMP_VALUE_FUNC(hemp_element_numop_boolean);

HEMP_SYMBOL_FUNC(hemp_element_number_autoinc_symbol);
HEMP_SYMBOL_FUNC(hemp_element_number_autodec_symbol);
HEMP_SYMBOL_FUNC(hemp_element_number_plus_symbol);
HEMP_SYMBOL_FUNC(hemp_element_number_minus_symbol);
HEMP_SYMBOL_FUNC(hemp_element_number_power_symbol);
HEMP_SYMBOL_FUNC(hemp_element_number_multiply_symbol);
HEMP_SYMBOL_FUNC(hemp_element_number_divide_symbol);
HEMP_SYMBOL_FUNC(hemp_element_number_divint_symbol);
HEMP_SYMBOL_FUNC(hemp_element_number_modulus_symbol);
HEMP_SYMBOL_FUNC(hemp_element_number_compare_symbol);
HEMP_SYMBOL_FUNC(hemp_element_number_equal_symbol);
HEMP_SYMBOL_FUNC(hemp_element_number_not_equal_symbol);
HEMP_SYMBOL_FUNC(hemp_element_number_before_symbol);
HEMP_SYMBOL_FUNC(hemp_element_number_not_before_symbol);
HEMP_SYMBOL_FUNC(hemp_element_number_not_after_symbol);
HEMP_SYMBOL_FUNC(hemp_element_number_after_symbol);

HEMP_VALUE_FUNC(hemp_element_number_autoinc_value);
HEMP_VALUE_FUNC(hemp_element_number_autodec_value);
HEMP_VALUE_FUNC(hemp_element_number_plus_value);
HEMP_VALUE_FUNC(hemp_element_number_minus_value);
HEMP_VALUE_FUNC(hemp_element_number_power_value);
HEMP_VALUE_FUNC(hemp_element_number_multiply_value);
HEMP_VALUE_FUNC(hemp_element_number_divide_value);
HEMP_VALUE_FUNC(hemp_element_number_divint_value);
HEMP_VALUE_FUNC(hemp_element_number_modulus_value);
HEMP_VALUE_FUNC(hemp_element_number_compare_value);


/*--------------------------------------------------------------------------
 * text operator elements
 *--------------------------------------------------------------------------*/

//HEMP_SYMBOL_FUNC(hemp_element_textop_symbol);
//HEMP_VALUE_FUNC(hemp_element_textop_number);
//HEMP_VALUE_FUNC(hemp_element_textop_integer);
//HEMP_VALUE_FUNC(hemp_element_textop_boolean);

HEMP_SYMBOL_FUNC(hemp_element_text_concat_symbol);
HEMP_SYMBOL_FUNC(hemp_element_text_compare_symbol);
HEMP_SYMBOL_FUNC(hemp_element_text_equal_symbol);
HEMP_SYMBOL_FUNC(hemp_element_text_not_equal_symbol);
HEMP_SYMBOL_FUNC(hemp_element_text_before_symbol);
HEMP_SYMBOL_FUNC(hemp_element_text_not_before_symbol);
HEMP_SYMBOL_FUNC(hemp_element_text_after_symbol);
HEMP_SYMBOL_FUNC(hemp_element_text_not_after_symbol);

HEMP_OUTPUT_FUNC(hemp_element_text_concat_value);
HEMP_VALUE_FUNC(hemp_element_text_compare_value);
HEMP_VALUE_FUNC(hemp_element_text_value);


/*--------------------------------------------------------------------------
 * assignment operator
 *--------------------------------------------------------------------------*/

HEMP_SYMBOL_FUNC(hemp_element_assign_symbol);
HEMP_OUTPUT_FUNC(hemp_element_assign_text);
HEMP_VALUE_FUNC(hemp_element_assign_value);


/*--------------------------------------------------------------------------
 * other operators
 *--------------------------------------------------------------------------*/

HEMP_SYMBOL_FUNC(hemp_element_dotop_symbol);
HEMP_POSTFIX_FUNC(hemp_element_dotop_postfix);
HEMP_VALUE_FUNC(hemp_element_dotop_value);
HEMP_OPERATE_FUNC(hemp_element_dotop_assign);
void hemp_element_dotop_clean(hemp_element_p);



/*--------------------------------------------------------------------------
 * block element
 *--------------------------------------------------------------------------*/

HEMP_SYMBOL_FUNC(hemp_element_block_symbol);
HEMP_OUTPUT_FUNC(hemp_element_block_token);
HEMP_OUTPUT_FUNC(hemp_element_block_source);
HEMP_OUTPUT_FUNC(hemp_element_block_text);
HEMP_VALUE_FUNC(hemp_element_block_value);
HEMP_OUTPUT_FUNC(hemp_element_block_values);


/*--------------------------------------------------------------------------
 * basic values
 *--------------------------------------------------------------------------*/
HEMP_OUTPUT_FUNC(hemp_element_value_text);
HEMP_VALUE_FUNC(hemp_element_value_number);
HEMP_VALUE_FUNC(hemp_element_value_integer);
HEMP_VALUE_FUNC(hemp_element_value_boolean);
HEMP_VALUE_FUNC(hemp_element_value_compare);
HEMP_OUTPUT_FUNC(hemp_element_value_values);


void
    hemp_element_text_clean(
        hemp_element_p element
    );

void
    hemp_element_block_clean(
        hemp_element_p element
    );

void
    hemp_element_brackets_clean(
        hemp_element_p element
    );

hemp_bool_t
    hemp_element_dump(
        hemp_element_p element
    );


#endif /* HEMP_ELEMENT_H */