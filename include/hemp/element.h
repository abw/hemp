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

extern HEMP_FIXUP(hemp_element_not_proto);
extern HEMP_OUTPUT(hemp_element_not_text);
extern HEMP_VALUE(hemp_element_not_value);
extern HEMP_VALUE(hemp_element_not_number);
extern HEMP_VALUE(hemp_element_not_integer);
extern HEMP_VALUE(hemp_element_not_boolean);
extern HEMP_VALUE(hemp_element_not_compare);
extern HEMP_INPUT(hemp_element_not_assign);
extern HEMP_OUTPUT(hemp_element_not_token);
extern HEMP_OUTPUT(hemp_element_not_source);


/*--------------------------------------------------------------------------
 * general parsing functions
 *--------------------------------------------------------------------------*/

extern HEMP_PREFIX(hemp_element_parse_fixed);
extern HEMP_PREFIX(hemp_element_parse_body);
extern HEMP_PREFIX(hemp_element_parse_body_block);
extern HEMP_PREFIX(hemp_element_parse_prefix);
extern HEMP_POSTFIX(hemp_element_parse_postfix);
extern HEMP_POSTFIX(hemp_element_parse_infix_left);
extern HEMP_POSTFIX(hemp_element_parse_infix_right);



/*--------------------------------------------------------------------------
 * whitespace, comments, etc
 *--------------------------------------------------------------------------*/

extern HEMP_ELEMENT(hemp_element_punctuation);
extern HEMP_ELEMENT(hemp_element_delimiter);
extern HEMP_ELEMENT(hemp_element_separator);
extern HEMP_ELEMENT(hemp_element_terminator);
extern HEMP_ELEMENT(hemp_element_space);
extern HEMP_ELEMENT(hemp_element_tag_start);
extern HEMP_ELEMENT(hemp_element_tag_end);
extern HEMP_ELEMENT(hemp_element_eof);
extern HEMP_ELEMENT(hemp_element_comment);

extern HEMP_PREFIX(hemp_element_space_prefix);
extern HEMP_POSTFIX(hemp_element_space_postfix);
extern HEMP_PREFIX(hemp_element_space_body);
extern HEMP_POSTFIX(hemp_element_space_branch);

extern hemp_bool hemp_element_terminator_matches(hemp_fragment, hemp_string);

/*--------------------------------------------------------------------------
 * tag elements
 *--------------------------------------------------------------------------*/

extern HEMP_ELEMENT(hemp_element_tag_inline);
extern HEMP_SCANNER(hemp_element_tag_inline_scanner);



/*--------------------------------------------------------------------------
 * identity elements
 *--------------------------------------------------------------------------*/

extern HEMP_ELEMENT(hemp_element_identity_true);
extern HEMP_ELEMENT(hemp_element_identity_false);
extern HEMP_ELEMENT(hemp_element_identity_empty);
extern HEMP_ELEMENT(hemp_element_identity_nothing);
extern HEMP_ELEMENT(hemp_element_identity_missing);
extern HEMP_ELEMENT(hemp_element_identity_before);
extern HEMP_ELEMENT(hemp_element_identity_after);
extern HEMP_ELEMENT(hemp_element_identity_equal);


/*--------------------------------------------------------------------------
 * literal elements
 *--------------------------------------------------------------------------*/


// NOTE: added the 'extern' to a handful of functions that I'm trying to 
// use in the dynamically loaded language/test module.  It looks like these
// have to be declared as extern (and in fact, they should all be)

extern HEMP_ELEMENT(hemp_element_literal);
extern HEMP_PREFIX(hemp_element_literal_prefix);
extern HEMP_OUTPUT(hemp_element_literal_text);
extern HEMP_VALUE(hemp_element_literal_value);
extern HEMP_CLEANUP(hemp_element_literal_cleanup);

extern HEMP_ELEMENT(hemp_element_fragment);
extern HEMP_SCANNER(hemp_element_fragment_scanner);
extern hemp_bool hemp_match_end_fragment(hemp_fragment *fragptr, hemp_fragment start);



/*--------------------------------------------------------------------------
 * text elements
 *--------------------------------------------------------------------------*/

extern HEMP_ELEMENT(hemp_element_text);
extern HEMP_PREFIX(hemp_element_text_prefix);
extern HEMP_VALUE(hemp_element_text_value);
extern HEMP_CLEANUP(hemp_element_text_cleanup);


/*--------------------------------------------------------------------------
 * words
 *--------------------------------------------------------------------------*/

extern HEMP_ELEMENT(hemp_element_word);
extern HEMP_PREFIX(hemp_element_word_prefix);
extern HEMP_FIXUP(hemp_element_word_proto);
extern HEMP_FIXUP(hemp_element_word_lvalue);
extern HEMP_VALUE(hemp_element_word_value);
extern HEMP_INPUT(hemp_element_word_assign);
extern HEMP_CLEANUP(hemp_element_word_cleanup);


/*--------------------------------------------------------------------------
 * quoted strings
 *--------------------------------------------------------------------------*/

extern HEMP_ELEMENT(hemp_element_squote);
extern HEMP_SCANNER(hemp_element_squote_scanner);

extern HEMP_ELEMENT(hemp_element_dquote);
extern HEMP_SCANNER(hemp_element_dquote_scanner);

extern HEMP_OUTPUT(hemp_element_quoted_text);
extern HEMP_VALUE(hemp_element_quoted_value);


/*--------------------------------------------------------------------------
 * number/integer elements
 *--------------------------------------------------------------------------*/

extern HEMP_ELEMENT(hemp_element_number);
extern HEMP_VALUE(hemp_element_number_value);

extern HEMP_ELEMENT(hemp_element_integer);
extern HEMP_VALUE(hemp_element_integer_value);


/*--------------------------------------------------------------------------
 * block elements
 *--------------------------------------------------------------------------*/

extern HEMP_ELEMENT(hemp_element_block);
extern HEMP_VALUE(hemp_element_block_value);
extern HEMP_OUTPUT(hemp_element_block_token);
extern HEMP_OUTPUT(hemp_element_block_source);
extern HEMP_OUTPUT(hemp_element_block_text);
extern HEMP_OUTPUT(hemp_element_block_values);
extern HEMP_OUTPUT(hemp_element_block_params);
extern HEMP_OUTPUT(hemp_element_block_pairs);
extern HEMP_CLEANUP(hemp_element_block_cleanup);


/*--------------------------------------------------------------------------
 * brackets
 *--------------------------------------------------------------------------*/

extern HEMP_ELEMENT(hemp_element_brackets);
extern HEMP_ELEMENT(hemp_element_parens);
extern HEMP_ELEMENT(hemp_element_list);
extern HEMP_ELEMENT(hemp_element_hash);

extern HEMP_PREFIX(hemp_element_brackets_parse);
extern HEMP_PREFIX(hemp_element_brackets_prefix);
extern HEMP_CLEANUP(hemp_element_brackets_clean);

extern HEMP_POSTFIX(hemp_element_parens_postfix);
extern HEMP_FIXUP(hemp_element_parens_proto);
extern HEMP_VALUE(hemp_element_parens_value);

extern HEMP_VALUE(hemp_element_list_value);

extern HEMP_PREFIX(hemp_element_hash_prefix);
extern HEMP_PREFIX(hemp_element_hash_body);
extern HEMP_VALUE(hemp_element_hash_value);


/*--------------------------------------------------------------------------
 * generic operators
 *--------------------------------------------------------------------------*/

extern HEMP_ELEMENT(hemp_element_operator);
extern HEMP_ELEMENT(hemp_element_prefix);
extern HEMP_ELEMENT(hemp_element_postfix);
extern HEMP_ELEMENT(hemp_element_prepostfix);
extern HEMP_ELEMENT(hemp_element_infix);
extern HEMP_ELEMENT(hemp_element_infix_left);
extern HEMP_ELEMENT(hemp_element_infix_right);

extern HEMP_OUTPUT(hemp_element_prefix_source);
extern HEMP_OUTPUT(hemp_element_postfix_source);
extern HEMP_OUTPUT(hemp_element_prepostfix_source);
extern HEMP_OUTPUT(hemp_element_infix_source);


/*--------------------------------------------------------------------------
 * generic element comparison functions
 *--------------------------------------------------------------------------*/

extern HEMP_ELEMENT(hemp_element_compare);
extern HEMP_ELEMENT(hemp_element_compare_equal);
extern HEMP_ELEMENT(hemp_element_compare_not_equal);
extern HEMP_ELEMENT(hemp_element_compare_before);
extern HEMP_ELEMENT(hemp_element_compare_not_before);
extern HEMP_ELEMENT(hemp_element_compare_after);
extern HEMP_ELEMENT(hemp_element_compare_not_after);


extern HEMP_VALUE(hemp_element_compare_value);
extern HEMP_VALUE(hemp_element_compare_equal_value);
extern HEMP_VALUE(hemp_element_compare_not_equal_value);
extern HEMP_VALUE(hemp_element_compare_before_value);
extern HEMP_VALUE(hemp_element_compare_not_before_value);
extern HEMP_VALUE(hemp_element_compare_after_value);
extern HEMP_VALUE(hemp_element_compare_not_after_value);


/*--------------------------------------------------------------------------
 * boolean operators
 *--------------------------------------------------------------------------*/

extern HEMP_ELEMENT(hemp_element_boolean);
extern HEMP_ELEMENT(hemp_element_boolean_not);
extern HEMP_ELEMENT(hemp_element_boolean_and);
extern HEMP_ELEMENT(hemp_element_boolean_or);

extern HEMP_VALUE(hemp_element_boolean_value);
extern HEMP_VALUE(hemp_element_boolean_not_value);
extern HEMP_VALUE(hemp_element_boolean_and_value);
extern HEMP_VALUE(hemp_element_boolean_or_value);


/*--------------------------------------------------------------------------
 * number operator elements
 *--------------------------------------------------------------------------*/

extern HEMP_ELEMENT(hemp_element_number_autoinc);
extern HEMP_ELEMENT(hemp_element_number_autodec);
extern HEMP_ELEMENT(hemp_element_number_plus);
extern HEMP_ELEMENT(hemp_element_number_minus);
extern HEMP_ELEMENT(hemp_element_number_power);
extern HEMP_ELEMENT(hemp_element_number_multiply);
extern HEMP_ELEMENT(hemp_element_number_divide);
extern HEMP_ELEMENT(hemp_element_number_divint);
extern HEMP_ELEMENT(hemp_element_number_modulus);
extern HEMP_ELEMENT(hemp_element_number_compare);
extern HEMP_ELEMENT(hemp_element_number_equal);
extern HEMP_ELEMENT(hemp_element_number_not_equal);
extern HEMP_ELEMENT(hemp_element_number_before);
extern HEMP_ELEMENT(hemp_element_number_not_before);
extern HEMP_ELEMENT(hemp_element_number_not_after);
extern HEMP_ELEMENT(hemp_element_number_after);

extern HEMP_VALUE(hemp_element_number_autoinc_value);
extern HEMP_VALUE(hemp_element_number_autodec_value);
extern HEMP_VALUE(hemp_element_number_plus_value);
extern HEMP_VALUE(hemp_element_number_minus_value);
extern HEMP_VALUE(hemp_element_number_power_value);
extern HEMP_VALUE(hemp_element_number_multiply_value);
extern HEMP_VALUE(hemp_element_number_divide_value);
extern HEMP_VALUE(hemp_element_number_divint_value);
extern HEMP_VALUE(hemp_element_number_modulus_value);
extern HEMP_VALUE(hemp_element_number_compare_value);


/*--------------------------------------------------------------------------
 * text operator elements
 *--------------------------------------------------------------------------*/

extern HEMP_ELEMENT(hemp_element_text_concat);
extern HEMP_ELEMENT(hemp_element_text_compare);
extern HEMP_ELEMENT(hemp_element_text_equal);
extern HEMP_ELEMENT(hemp_element_text_not_equal);
extern HEMP_ELEMENT(hemp_element_text_before);
extern HEMP_ELEMENT(hemp_element_text_not_before);
extern HEMP_ELEMENT(hemp_element_text_after);
extern HEMP_ELEMENT(hemp_element_text_not_after);

extern HEMP_OUTPUT(hemp_element_text_concat_value);
extern HEMP_VALUE(hemp_element_text_compare_value);


/*--------------------------------------------------------------------------
 * assignment operator
 *--------------------------------------------------------------------------*/

extern HEMP_ELEMENT(hemp_element_assign);
extern HEMP_POSTFIX(hemp_element_assign_postfix);
extern HEMP_OUTPUT(hemp_element_assign_text);
extern HEMP_VALUE(hemp_element_assign_value);
extern HEMP_OUTPUT(hemp_element_assign_params);
extern HEMP_OUTPUT(hemp_element_assign_pairs);


/*--------------------------------------------------------------------------
 * other operators
 *--------------------------------------------------------------------------*/

extern HEMP_ELEMENT(hemp_element_dotop);
extern HEMP_POSTFIX(hemp_element_dotop_postfix);
extern HEMP_VALUE(hemp_element_dotop_value);
extern HEMP_INPUT(hemp_element_dotop_assign);
extern HEMP_CLEANUP(hemp_element_dotop_cleanup);

extern HEMP_ELEMENT(hemp_element_sub);
extern HEMP_PREFIX(hemp_element_sub_prefix);
extern HEMP_VALUE(hemp_element_sub_value);
extern HEMP_OUTPUT(hemp_element_sub_text);
extern HEMP_CLEANUP(hemp_element_sub_cleanup);

extern HEMP_ELEMENT(hemp_element_command_if);
extern HEMP_ELEMENT(hemp_element_command_else);
extern HEMP_ELEMENT(hemp_element_command_elsif);

extern HEMP_ELEMENT(hemp_element_apply);

/*--------------------------------------------------------------------------
 * basic values
 *--------------------------------------------------------------------------*/

extern HEMP_VALUE(hemp_element_value);
extern HEMP_VALUE(hemp_element_value_number);
extern HEMP_VALUE(hemp_element_value_integer);
extern HEMP_VALUE(hemp_element_value_boolean);
extern HEMP_VALUE(hemp_element_value_compare);
extern HEMP_OUTPUT(hemp_element_value_text);
extern HEMP_OUTPUT(hemp_element_value_values);



extern HEMP_SCANNER(hemp_element_comment_scanner);
extern HEMP_PREFIX(hemp_element_space_parse_expr);
extern HEMP_OUTPUT(hemp_element_eof_token);





#endif /* HEMP_ELEMENT_H */