#ifndef HEMP_ELEMENT_H
#define HEMP_ELEMENT_H

#include <hemp/core.h>
#include <hemp/fragment.h>
#include <hemp/namespace.h>
#include <hemp/type.h>
#include <hemp/type/text.h>

//#include <hemp/type.h>
//#include <hemp/tag.h>
//#include <hemp/document.h>
//#include <hemp/context.h>
//#include <hemp/value.h>
//#include <hemp/macros.h>
//#include <hemp/proto.h>
//// #include <hemp/grammar.h>
//#include <hemp/type/text.h>



/*--------------------------------------------------------------------------
 * type definitions
 *--------------------------------------------------------------------------*/

struct hemp_element {
    /* an element is an extended object type */
    HEMP_TYPE_BASE

    /* an element is bound to a grammar */
    hemp_grammar    grammar;

    /* element metadata */
    hemp_string     start;
    hemp_string     end;
    hemp_flags      flags;
    hemp_oprec      lprec;
    hemp_oprec      rprec;

    /* intialisation (on scan) and cleanup methods */
    hemp_scanner    scanner;
    hemp_cleanup    cleanup;

    /* parsing methods */
    hemp_prefix     parse_prefix;
    hemp_postfix    parse_postfix;
    hemp_postfix    parse_branch;
    hemp_prefix     parse_fixed;
    hemp_prefix     parse_params;
    hemp_prefix     parse_body;
    hemp_fixup      parse_lvalue;
    hemp_fixup      parse_proto;

    /* value methods */
    hemp_output_f   token;
    hemp_output_f   source;
};

typedef hemp_element 
(* hemp_element_f)(
    hemp_hemp       hemp,
    hemp_element    element
);

struct hemp_elements {
    hemp_string     name;
    hemp_element_f  ctor;
};



/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

hemp_element
hemp_element_new(
    hemp_string     name,
    hemp_string     start,
    hemp_string     end
);


void
hemp_element_free(
    hemp_element    element
);

void 
hemp_element_dump(
    hemp_element    element
);


/*--------------------------------------------------------------------------
 * global elements and element constructors
 *--------------------------------------------------------------------------*/

extern hemp_element HempElementSpace;
extern hemp_element HempElementComment;
extern hemp_element HempElementTagStart;
extern hemp_element HempElementTagEnd;
extern hemp_element HempElementBlock;
extern hemp_element HempElementText;
extern hemp_element HempElementWord;
extern hemp_element HempElementNumber;
extern hemp_element HempElementInteger;
extern hemp_element HempElementEOF;


HEMP_GLOBAL_ELEMENT(hemp_global_element_space);
HEMP_GLOBAL_ELEMENT(hemp_global_element_comment);
HEMP_GLOBAL_ELEMENT(hemp_global_element_tag_start);
HEMP_GLOBAL_ELEMENT(hemp_global_element_tag_end);
HEMP_GLOBAL_ELEMENT(hemp_global_element_block);
HEMP_GLOBAL_ELEMENT(hemp_global_element_text);
HEMP_GLOBAL_ELEMENT(hemp_global_element_word);
HEMP_GLOBAL_ELEMENT(hemp_global_element_number);
HEMP_GLOBAL_ELEMENT(hemp_global_element_integer);
HEMP_GLOBAL_ELEMENT(hemp_global_element_eof);

/*--------------------------------------------------------------------------
 * decline functions
 *--------------------------------------------------------------------------*/

HEMP_FIXUP(hemp_element_not_proto);
HEMP_OUTPUT(hemp_element_not_text);
HEMP_VALUE(hemp_element_not_value);
HEMP_VALUE(hemp_element_not_number);
HEMP_VALUE(hemp_element_not_integer);
HEMP_VALUE(hemp_element_not_boolean);
HEMP_VALUE(hemp_element_not_compare);
HEMP_INPUT(hemp_element_not_assign);
HEMP_OUTPUT(hemp_element_not_token);
HEMP_OUTPUT(hemp_element_not_source);


/*--------------------------------------------------------------------------
 * general parsing functions
 *--------------------------------------------------------------------------*/

HEMP_PREFIX(hemp_element_parse_fixed);
HEMP_PREFIX(hemp_element_parse_body);
HEMP_PREFIX(hemp_element_parse_body_block);
HEMP_PREFIX(hemp_element_parse_prefix);
HEMP_POSTFIX(hemp_element_parse_postfix);
HEMP_POSTFIX(hemp_element_parse_infix_left);
HEMP_POSTFIX(hemp_element_parse_infix_right);



/*--------------------------------------------------------------------------
 * whitespace, comments, etc
 *--------------------------------------------------------------------------*/

HEMP_ELEMENT(hemp_element_punctuation);
HEMP_ELEMENT(hemp_element_delimiter);
HEMP_ELEMENT(hemp_element_separator);
HEMP_ELEMENT(hemp_element_terminator);
HEMP_ELEMENT(hemp_element_space);
HEMP_ELEMENT(hemp_element_tag_start);
HEMP_ELEMENT(hemp_element_tag_end);
HEMP_ELEMENT(hemp_element_eof);
HEMP_ELEMENT(hemp_element_comment);

HEMP_PREFIX(hemp_element_space_prefix);
HEMP_POSTFIX(hemp_element_space_postfix);
HEMP_PREFIX(hemp_element_space_body);
HEMP_POSTFIX(hemp_element_space_branch);

hemp_bool hemp_element_terminator_matches(hemp_fragment, hemp_string);

/*--------------------------------------------------------------------------
 * tag elements
 *--------------------------------------------------------------------------*/

HEMP_ELEMENT(hemp_element_tag_inline);
HEMP_SCANNER(hemp_element_tag_inline_scanner);



/*--------------------------------------------------------------------------
 * identity elements
 *--------------------------------------------------------------------------*/

HEMP_ELEMENT(hemp_element_identity_true);
HEMP_ELEMENT(hemp_element_identity_false);
HEMP_ELEMENT(hemp_element_identity_empty);
HEMP_ELEMENT(hemp_element_identity_nothing);
HEMP_ELEMENT(hemp_element_identity_missing);
HEMP_ELEMENT(hemp_element_identity_before);
HEMP_ELEMENT(hemp_element_identity_after);
HEMP_ELEMENT(hemp_element_identity_equal);


/*--------------------------------------------------------------------------
 * literal elements
 *--------------------------------------------------------------------------*/


// NOTE: added the 'extern' to a handful of functions that I'm trying to 
// use in the dynamically loaded language/test module.  It looks like these
// have to be declared as extern (and in fact, they should all be)

HEMP_ELEMENT(hemp_element_literal);
HEMP_PREFIX(hemp_element_literal_prefix);
extern HEMP_OUTPUT(hemp_element_literal_text);
extern HEMP_VALUE(hemp_element_literal_value);
HEMP_CLEANUP(hemp_element_literal_cleanup);

HEMP_ELEMENT(hemp_element_fragment);
HEMP_SCANNER(hemp_element_fragment_scanner);
hemp_bool hemp_match_end_fragment(hemp_fragment *fragptr, hemp_fragment start);



/*--------------------------------------------------------------------------
 * text elements
 *--------------------------------------------------------------------------*/

HEMP_ELEMENT(hemp_element_text);
HEMP_PREFIX(hemp_element_text_prefix);
HEMP_VALUE(hemp_element_text_value);
HEMP_CLEANUP(hemp_element_text_cleanup);


/*--------------------------------------------------------------------------
 * words
 *--------------------------------------------------------------------------*/

HEMP_ELEMENT(hemp_element_word);
HEMP_PREFIX(hemp_element_word_prefix);
HEMP_FIXUP(hemp_element_word_proto);
HEMP_FIXUP(hemp_element_word_lvalue);
HEMP_VALUE(hemp_element_word_value);
HEMP_INPUT(hemp_element_word_assign);
HEMP_CLEANUP(hemp_element_word_cleanup);


/*--------------------------------------------------------------------------
 * quoted strings
 *--------------------------------------------------------------------------*/

HEMP_ELEMENT(hemp_element_squote);
HEMP_SCANNER(hemp_element_squote_scanner);

HEMP_ELEMENT(hemp_element_dquote);
HEMP_SCANNER(hemp_element_dquote_scanner);

HEMP_OUTPUT(hemp_element_quoted_text);
HEMP_VALUE(hemp_element_quoted_value);


/*--------------------------------------------------------------------------
 * number/integer elements
 *--------------------------------------------------------------------------*/

HEMP_ELEMENT(hemp_element_number);
HEMP_VALUE(hemp_element_number_value);

HEMP_ELEMENT(hemp_element_integer);
HEMP_VALUE(hemp_element_integer_value);


/*--------------------------------------------------------------------------
 * block elements
 *--------------------------------------------------------------------------*/

HEMP_ELEMENT(hemp_element_block);
HEMP_VALUE(hemp_element_block_value);
HEMP_OUTPUT(hemp_element_block_token);
HEMP_OUTPUT(hemp_element_block_source);
HEMP_OUTPUT(hemp_element_block_text);
HEMP_OUTPUT(hemp_element_block_values);
HEMP_OUTPUT(hemp_element_block_params);
HEMP_CLEANUP(hemp_element_block_cleanup);


/*--------------------------------------------------------------------------
 * brackets
 *--------------------------------------------------------------------------*/

HEMP_ELEMENT(hemp_element_brackets);
HEMP_ELEMENT(hemp_element_parens);
HEMP_ELEMENT(hemp_element_list);
HEMP_ELEMENT(hemp_element_hash);

HEMP_PREFIX(hemp_element_brackets_parse);
HEMP_PREFIX(hemp_element_brackets_prefix);
HEMP_CLEANUP(hemp_element_brackets_clean);

HEMP_POSTFIX(hemp_element_parens_postfix);
HEMP_FIXUP(hemp_element_parens_proto);
HEMP_VALUE(hemp_element_parens_value);

HEMP_VALUE(hemp_element_list_value);

HEMP_PREFIX(hemp_element_hash_prefix);
HEMP_PREFIX(hemp_element_hash_body);
HEMP_VALUE(hemp_element_hash_value);


/*--------------------------------------------------------------------------
 * generic operators
 *--------------------------------------------------------------------------*/

HEMP_ELEMENT(hemp_element_operator);
HEMP_ELEMENT(hemp_element_prefix);
HEMP_ELEMENT(hemp_element_postfix);
HEMP_ELEMENT(hemp_element_prepostfix);
HEMP_ELEMENT(hemp_element_infix);
HEMP_ELEMENT(hemp_element_infix_left);
HEMP_ELEMENT(hemp_element_infix_right);

HEMP_OUTPUT(hemp_element_prefix_source);
HEMP_OUTPUT(hemp_element_postfix_source);
HEMP_OUTPUT(hemp_element_prepostfix_source);
HEMP_OUTPUT(hemp_element_infix_source);


/*--------------------------------------------------------------------------
 * generic element comparison functions
 *--------------------------------------------------------------------------*/

HEMP_ELEMENT(hemp_element_compare);
HEMP_ELEMENT(hemp_element_compare_equal);
HEMP_ELEMENT(hemp_element_compare_not_equal);
HEMP_ELEMENT(hemp_element_compare_before);
HEMP_ELEMENT(hemp_element_compare_not_before);
HEMP_ELEMENT(hemp_element_compare_after);
HEMP_ELEMENT(hemp_element_compare_not_after);


HEMP_VALUE(hemp_element_compare_value);
HEMP_VALUE(hemp_element_compare_equal_value);
HEMP_VALUE(hemp_element_compare_not_equal_value);
HEMP_VALUE(hemp_element_compare_before_value);
HEMP_VALUE(hemp_element_compare_not_before_value);
HEMP_VALUE(hemp_element_compare_after_value);
HEMP_VALUE(hemp_element_compare_not_after_value);


/*--------------------------------------------------------------------------
 * boolean operators
 *--------------------------------------------------------------------------*/

HEMP_ELEMENT(hemp_element_boolean);
HEMP_ELEMENT(hemp_element_boolean_not);
HEMP_ELEMENT(hemp_element_boolean_and);
HEMP_ELEMENT(hemp_element_boolean_or);

HEMP_VALUE(hemp_element_boolean_value);
HEMP_VALUE(hemp_element_boolean_not_value);
HEMP_VALUE(hemp_element_boolean_and_value);
HEMP_VALUE(hemp_element_boolean_or_value);


/*--------------------------------------------------------------------------
 * number operator elements
 *--------------------------------------------------------------------------*/

HEMP_ELEMENT(hemp_element_number_autoinc);
HEMP_ELEMENT(hemp_element_number_autodec);
HEMP_ELEMENT(hemp_element_number_plus);
HEMP_ELEMENT(hemp_element_number_minus);
HEMP_ELEMENT(hemp_element_number_power);
HEMP_ELEMENT(hemp_element_number_multiply);
HEMP_ELEMENT(hemp_element_number_divide);
HEMP_ELEMENT(hemp_element_number_divint);
HEMP_ELEMENT(hemp_element_number_modulus);
HEMP_ELEMENT(hemp_element_number_compare);
HEMP_ELEMENT(hemp_element_number_equal);
HEMP_ELEMENT(hemp_element_number_not_equal);
HEMP_ELEMENT(hemp_element_number_before);
HEMP_ELEMENT(hemp_element_number_not_before);
HEMP_ELEMENT(hemp_element_number_not_after);
HEMP_ELEMENT(hemp_element_number_after);

HEMP_VALUE(hemp_element_number_autoinc_value);
HEMP_VALUE(hemp_element_number_autodec_value);
HEMP_VALUE(hemp_element_number_plus_value);
HEMP_VALUE(hemp_element_number_minus_value);
HEMP_VALUE(hemp_element_number_power_value);
HEMP_VALUE(hemp_element_number_multiply_value);
HEMP_VALUE(hemp_element_number_divide_value);
HEMP_VALUE(hemp_element_number_divint_value);
HEMP_VALUE(hemp_element_number_modulus_value);
HEMP_VALUE(hemp_element_number_compare_value);


/*--------------------------------------------------------------------------
 * text operator elements
 *--------------------------------------------------------------------------*/

HEMP_ELEMENT(hemp_element_text_concat);
HEMP_ELEMENT(hemp_element_text_compare);
HEMP_ELEMENT(hemp_element_text_equal);
HEMP_ELEMENT(hemp_element_text_not_equal);
HEMP_ELEMENT(hemp_element_text_before);
HEMP_ELEMENT(hemp_element_text_not_before);
HEMP_ELEMENT(hemp_element_text_after);
HEMP_ELEMENT(hemp_element_text_not_after);

HEMP_OUTPUT(hemp_element_text_concat_value);
HEMP_VALUE(hemp_element_text_compare_value);


/*--------------------------------------------------------------------------
 * assignment operator
 *--------------------------------------------------------------------------*/

HEMP_ELEMENT(hemp_element_assign);
HEMP_POSTFIX(hemp_element_assign_postfix);
HEMP_OUTPUT(hemp_element_assign_text);
HEMP_VALUE(hemp_element_assign_value);
HEMP_OUTPUT(hemp_element_assign_params);
HEMP_OUTPUT(hemp_element_assign_pairs);


/*--------------------------------------------------------------------------
 * other operators
 *--------------------------------------------------------------------------*/

HEMP_ELEMENT(hemp_element_dotop);
HEMP_POSTFIX(hemp_element_dotop_postfix);
HEMP_VALUE(hemp_element_dotop_value);
HEMP_INPUT(hemp_element_dotop_assign);
HEMP_CLEANUP(hemp_element_dotop_cleanup);

HEMP_ELEMENT(hemp_element_sub);
HEMP_PREFIX(hemp_element_sub_prefix);
HEMP_VALUE(hemp_element_sub_value);
HEMP_OUTPUT(hemp_element_sub_text);
HEMP_CLEANUP(hemp_element_sub_cleanup);

HEMP_ELEMENT(hemp_element_command_if);
HEMP_ELEMENT(hemp_element_command_else);
HEMP_ELEMENT(hemp_element_command_elsif);

HEMP_ELEMENT(hemp_element_apply);

/*--------------------------------------------------------------------------
 * basic values
 *--------------------------------------------------------------------------*/

HEMP_VALUE(hemp_element_value);
HEMP_VALUE(hemp_element_value_number);
HEMP_VALUE(hemp_element_value_integer);
HEMP_VALUE(hemp_element_value_boolean);
HEMP_VALUE(hemp_element_value_compare);
extern HEMP_OUTPUT(hemp_element_value_text);
HEMP_OUTPUT(hemp_element_value_values);



HEMP_SCANNER(hemp_element_comment_scanner);
HEMP_PREFIX(hemp_element_space_parse_expr);
HEMP_OUTPUT(hemp_element_eof_token);





#endif /* HEMP_ELEMENT_H */