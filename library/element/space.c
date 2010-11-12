#include <hemp/element.h>

/*--------------------------------------------------------------------------
 * global symbol types
 *--------------------------------------------------------------------------*/

hemp_symbol_p HempSymbolSpace     = NULL;
hemp_symbol_p HempSymbolTagStart  = NULL;
hemp_symbol_p HempSymbolTagEnd    = NULL;
hemp_symbol_p HempSymbolEOF       = NULL;


/*--------------------------------------------------------------------------
 * generic symbol constructor functions
 *--------------------------------------------------------------------------*/

HEMP_SYMBOL_FUNC(hemp_element_punctuation_symbol) {
    hemp_element_literal_symbol(hemp, symbol);
    symbol->fixed = hemp_element_decline;
    symbol->flags = HEMP_BE_SOURCE | HEMP_BE_FIXED | HEMP_BE_HIDDEN;
    return symbol;
}


/* Delimiters are "soft-skippable" delimiters that exist mainly as optional
 * syntactic sugar. The comma is considered a delimiter in the hemp language.
 * e.g. Both [a b c] and [a, b, c] are parsed unambiguously as having the same 
 * meaning.  Commas can be used for disambiguation purposes.  e.g. [5, -2]
 * has two items, whereas [5 -2] is a list with a single value, 3, which is 
 * the results of the numerical expression 5 - 2.  Tag start tokens are also 
 * considered delimiters as they can safely be ignored at the parsing stage.
 * In this case they have already served their purpose at the scanning stage
 * to indicate where embedded languages begin.
 */

HEMP_SYMBOL_FUNC(hemp_element_delimiter_symbol) {
    hemp_element_punctuation_symbol(hemp, symbol);
    symbol->flags |= HEMP_BE_DELIMITER;
    return symbol;
}


/* Separators are used to separate expressions.  The semi-colon is typically
 * used: a + b; c + d.  They can also be used in lists as a more definitive
 * version of a comma.  For example, consider a directive like 'with' that 
 * takes a variable number of parameters:
 *
 *     fill header with title="Hello World", author="Brian Badger"
 *
 * If we put this inside a list to capture the output then we get a single 
 * value in the list.  Not that the comma is separating parameters consumed
 * by the 'with' command (being used in side-effect fashion as a modified
 * to the 'fill' command), rather than delimiting items in the list.
 *
 *   [ fill header with title="Hello World", author="Brian Badger" ]
 *
 * If we want to capture the output from processing two different templates
 * then we can do something like this:
 * 
 *   [ 
 *      fill header with title="Hello World", author="Brian Badger";
 *      fill header with title="Hello World", author="Brian Badger" 
 *   ]
 *
 * Note the semi-colon being used to separate one expression from another.
 * It has the same end effect as using parenthesis around each expression:
 *   [ 
 *      (fill header with title="Hello World", author="Brian Badger")
 *      (fill header with title="Hello World", author="Brian Badger")
 *   ]
 *
 * Tag end tokens are also defined as separators.  Consider this example:
 *   [% foo; "some text"; bar %]
 * Which is the same as this:
 *   [% foo %]some text[% bar %]
 * Tag end tokens serve the same purpose as the semi-colon.  The tag start
 * token is a special case because it always appears after a chunk of literal
 * text.  Text, being static, can't have any postfix operators apply to it, 
 * so there's never any ambiguity about the elements following a text chunk
 * - they're always the start of a new expression.  So tag start tokens are 
 * implemented as the "less strict" delimiter symbol which get skipped more
 * automagically.
 */

HEMP_SYMBOL_FUNC(hemp_element_separator_symbol) {
    hemp_element_punctuation_symbol(hemp, symbol);
    symbol->flags |= HEMP_BE_SEPARATOR;
    return symbol;
}


/* Terminators are special symbols that never get automatically skipped.
 * For example, closing brackets: ) } and ] should only ever appear matched
 * with their opening counterpart.  It is the responsibility of the opening
 * token to parse the contents up to the relevant terminator and check that
 * it is the correct one.
 */ 

HEMP_SYMBOL_FUNC(hemp_element_terminator_symbol) {
    hemp_element_punctuation_symbol(hemp, symbol);
    symbol->flags |= HEMP_BE_TERMINATOR;
    return symbol;
}


hemp_bool_t 
hemp_element_terminator_matches(
    hemp_element_p element,
    hemp_str_p     match
) {
    return ( hemp_has_flag(element->type, HEMP_BE_TERMINATOR)
        &&   hemp_stringn_eq(element->token, match, strlen(match)) )
           ? HEMP_TRUE
           : HEMP_FALSE;
}

    



/*--------------------------------------------------------------------------
 * space
 *--------------------------------------------------------------------------*/

HEMP_GLOBAL_SYMBOL(hemp_symbol_space) {
    hemp_debug_call("hemp_symbol_space()\n");
    return hemp_element_space_symbol(
        NULL,
        hemp_symbol_init("hemp.space", NULL, NULL)
    );
}


HEMP_SYMBOL_FUNC(hemp_element_space_symbol) {
    hemp_element_literal_symbol(hemp, symbol);
    symbol->prefix  = &hemp_element_next_prefix;
    symbol->infix   = &hemp_element_next_infix;
    symbol->flags   = HEMP_BE_WHITESPACE | HEMP_BE_SOURCE | HEMP_BE_FIXED 
                    | HEMP_BE_HIDDEN;
    return symbol;
}


/*--------------------------------------------------------------------------
 * tag start
 *--------------------------------------------------------------------------*/

HEMP_GLOBAL_SYMBOL(hemp_symbol_tag_start) {
    hemp_debug_call("hemp_symbol_tag_start()\n");
    return hemp_element_tag_start_symbol(
        NULL,
        hemp_symbol_init("hemp.tag_start", NULL, NULL)
    );
}


HEMP_SYMBOL_FUNC(hemp_element_tag_start_symbol) {
    hemp_element_literal_symbol(hemp, symbol);
    symbol->prefix  = &hemp_element_next_prefix;
    symbol->flags   = HEMP_BE_WHITESPACE | HEMP_BE_SOURCE | HEMP_BE_FIXED 
                    | HEMP_BE_HIDDEN | HEMP_BE_DELIMITER;
    return symbol;
}


/*--------------------------------------------------------------------------
 * tag end
 *--------------------------------------------------------------------------*/

HEMP_GLOBAL_SYMBOL(hemp_symbol_tag_end) {
    hemp_debug_call("hemp_symbol_tag_end()\n");
    return hemp_element_tag_end_symbol(
        NULL,
        hemp_symbol_init("hemp.tag_end", NULL, NULL)
    );
}


HEMP_SYMBOL_FUNC(hemp_element_tag_end_symbol) {
    hemp_element_delimiter_symbol(hemp, symbol);
    return symbol;
}



/*--------------------------------------------------------------------------
 * end of file element
 *--------------------------------------------------------------------------*/

HEMP_GLOBAL_SYMBOL(hemp_symbol_eof) {
    hemp_debug_call("hemp_symbol_eof()\n");
    return hemp_element_eof_symbol(
        NULL,
        hemp_symbol_init("hemp.eof", NULL, NULL)
    );
}


HEMP_SYMBOL_FUNC(hemp_element_eof_symbol) {
    symbol->token   = &hemp_element_eof_token;
    symbol->flags   = HEMP_BE_SOURCE | HEMP_BE_FIXED | HEMP_BE_HIDDEN
                    | HEMP_BE_EOF;
    return symbol;
}


HEMP_ETEXT_FUNC(hemp_element_eof_token) {
    hemp_debug_call("hemp_element_eof_token()\n");
    hemp_text_p text;
    hemp_prepare_output(output, text, 6);
    hemp_text_append_string(text, "--EOF--");     // TMP
    return output;
}


