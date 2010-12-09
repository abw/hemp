#include <hemp/element.h>


/*--------------------------------------------------------------------------
 * global element types
 *--------------------------------------------------------------------------*/

hemp_element HempElementSpace     = NULL;
hemp_element HempElementComment   = NULL;
hemp_element HempElementTagStart  = NULL;
hemp_element HempElementTagEnd    = NULL;
hemp_element HempElementEOF       = NULL;


HEMP_GLOBAL_ELEMENT(hemp_global_element_space) {
    return hemp_element_space(
        NULL,
        hemp_element_new("hemp.space", NULL, NULL)
    );
}


HEMP_GLOBAL_ELEMENT(hemp_global_element_comment) {
    return hemp_element_comment(
        NULL,
        hemp_element_new("hemp.comment", NULL, NULL)
    );
}


HEMP_GLOBAL_ELEMENT(hemp_global_element_tag_start) {
    return hemp_element_tag_start(
        NULL,
        hemp_element_new("hemp.tag_start", NULL, NULL)
    );
}


HEMP_GLOBAL_ELEMENT(hemp_global_element_tag_end) {
    return hemp_element_tag_end(
        NULL,
        hemp_element_new("hemp.tag_end", NULL, NULL)
    );
}


HEMP_GLOBAL_ELEMENT(hemp_global_element_eof) {
    return hemp_element_eof(
        NULL,
        hemp_element_new("hemp.eof", NULL, NULL)
    );
}



/*--------------------------------------------------------------------------
 * generic punctuation element - a base class of sorts
 *--------------------------------------------------------------------------*/

HEMP_ELEMENT(hemp_element_punctuation) {
    hemp_element_literal(hemp, element);
    element->parse_fixed    = NULL;
    element->parse_body     = NULL;
    element->flags          = HEMP_BE_SOURCE | HEMP_BE_FIXED | HEMP_BE_HIDDEN;
    return element;
}


/*--------------------------------------------------------------------------
 * Delimiters are "soft-skippable" delimiters that exist mainly as optional
 * syntactic sugar. The comma is considered a delimiter in the hemp language.
 * e.g. Both [a b c] and [a, b, c] are parsed unambiguously as having the same 
 * meaning.  Commas can be used for disambiguation purposes.  e.g. [5, -2]
 * has two items, whereas [5 -2] is a list with a single value, 3, which is 
 * the results of the numerical expression 5 - 2.  Tag start tokens are also 
 * considered delimiters as they can safely be ignored at the parsing stage.
 * In this case they have already served their purpose at the scanning stage
 * to indicate where embedded languages begin.
 *--------------------------------------------------------------------------*/

HEMP_ELEMENT(hemp_element_delimiter) {
    hemp_element_punctuation(hemp, element);
    hemp_set_flag(element, HEMP_BE_DELIMITER);
    return element;
}


/*--------------------------------------------------------------------------
 * Separators are used to separate expressions.  The semi-colon is typically
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
 * If we want to capture the output from processing two different documents
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
 * 
 *--------------------------------------------------------------------------*/

HEMP_ELEMENT(hemp_element_separator) {
    hemp_element_punctuation(hemp, element);
    element->parse_body = &hemp_element_parse_body_block;
    hemp_set_flag(element, HEMP_BE_SEPARATOR);
    return element;
}



/*--------------------------------------------------------------------------
 * Terminators are special symbols that never get automatically skipped.
 * For example, closing brackets: ) } and ] should only ever appear matched
 * with their opening counterpart.  It is the responsibility of the opening
 * token to parse the contents up to the relevant terminator and check that
 * it is the correct one.
 *--------------------------------------------------------------------------*/

HEMP_ELEMENT(hemp_element_terminator) {
    hemp_element_punctuation(hemp, element);
    hemp_set_flag(element, HEMP_BE_TERMINATOR);
    return element;
}


hemp_bool 
hemp_element_terminator_matches(
    hemp_fragment fragment,
    hemp_string   match
) {
    return ( hemp_has_flag(fragment, HEMP_BE_TERMINATOR)
        &&   hemp_stringn_eq(fragment->token, match, strlen(match)) )
           ? HEMP_TRUE
           : HEMP_FALSE;
}



/*--------------------------------------------------------------------------
 * space
 *--------------------------------------------------------------------------*/

HEMP_ELEMENT(hemp_element_space) {
    hemp_element_punctuation(hemp, element);
    element->parse_prefix   = &hemp_element_space_prefix;
    element->parse_postfix  = &hemp_element_space_postfix;
    element->parse_body     = &hemp_element_space_body;
    hemp_set_flag(element, HEMP_BE_WHITESPACE);
    return element;
}


HEMP_PREFIX(hemp_element_space_prefix) {
    hemp_debug_call("hemp_element_space_prefix()\n");

    return hemp_advance(fragptr)
        ? hemp_parse_prefix(fragptr, scope, precedence, force)
        : NULL;
}


HEMP_POSTFIX(hemp_element_space_postfix) {
    hemp_debug_call("hemp_element_space_postfix()\n");

    /* Some elements can appear in a postfix position, but must not have 
     * any intervening space between them and the preceeding element.  e.g.
     * the postfix parens in a function call must be touch the function name,
     * "foo(10)" is OK, but "foo (10)" isn't.  These elements set the 
     * HEMP_BE_POSTBOUND flag.  
     * 
     * This function is invoked when whitespace appears after an element 
     * which accepts postfix operators.  In the usual case, the whitespace
     * can be skipped and the postfix operator called on the next 
     * non-whitespace element.  However, if that element is HEMP_BE_POSTBOUND
     * then we rewind the element pointer back to the start and return the 
     * lhs element, effectively declining the postfix parsing opportunity.
     */
    hemp_fragment head = *fragptr;
    
    /* skip the first (current) whitespace element */
    hemp_advance(fragptr);

    /* skip any further whitespace elements */
    hemp_skip_whitespace(fragptr);

    if ((*fragptr)->type->parse_postfix 
    &&  hemp_not_flag((*fragptr)->type, HEMP_BE_POSTBOUND)) {
//      hemp_debug_msg("%s element has a postfix operator\n", (*elemptr)->type->name);
        return (*fragptr)->type->parse_postfix(fragptr, scope, precedence, force, lhs);
    }

    *fragptr = head;
    return lhs;
}


HEMP_PREFIX(hemp_element_space_body) {
    hemp_debug_call("hemp_element_space_body()\n");

    return hemp_advance(fragptr)
        ? hemp_parse_body(fragptr, scope, precedence, force)
        : NULL;
}


/*--------------------------------------------------------------------------
 * comment
 *--------------------------------------------------------------------------*/

HEMP_ELEMENT(hemp_element_comment) {
    hemp_element_space(hemp, element);
    element->scanner    = &hemp_element_comment_scanner;
    return element;
}


HEMP_SCANNER(hemp_element_comment_scanner) {
    hemp_debug_call("hemp_element_comment_scanner()\n");

    hemp_element element = (hemp_element) self;
    hemp_string  src     = document->scanptr;
    hemp_tag     tag     = hemp_document_current_tag(document);

    if (! tag)
        hemp_fatal("Can't scan comment without a tag in scope\n");

    if (! tag->to_eol)
        hemp_fatal("No line scanner for current '%s' tag\n", tag->name);
    
    /* update the source pointer past the text we've consumed */
    src = tag->to_eol(tag, src);

    /* add a comment element to the list of scanned tokens */
    hemp_document_scanned_to(
        document, element, src
    );

    return HEMP_TRUE;
}


/*--------------------------------------------------------------------------
 * tag start
 *--------------------------------------------------------------------------*/

HEMP_ELEMENT(hemp_element_tag_start) {
    hemp_element_literal(hemp, element);
    element->parse_prefix = &hemp_element_space_prefix;
    element->flags        = HEMP_BE_WHITESPACE | HEMP_BE_SOURCE | HEMP_BE_FIXED 
                          | HEMP_BE_HIDDEN     | HEMP_BE_DELIMITER;
    return element;
}



/*--------------------------------------------------------------------------
 * tag end
 *--------------------------------------------------------------------------*/

HEMP_ELEMENT(hemp_element_tag_end) {
    hemp_element_delimiter(hemp, element);
    element->parse_body = &hemp_element_parse_body_block;
    return element;
}



/*--------------------------------------------------------------------------
 * end of file element
 *--------------------------------------------------------------------------*/

HEMP_ELEMENT(hemp_element_eof) {
    element->parse_body  = NULL;
    element->token       = &hemp_element_eof_token;
    element->flags       = HEMP_BE_SOURCE | HEMP_BE_FIXED 
                         | HEMP_BE_HIDDEN | HEMP_BE_EOF;
    return element;
}


HEMP_OUTPUT(hemp_element_eof_token) {
    hemp_debug_call("hemp_element_eof_token()\n");
    hemp_text text;
    hemp_prepare_text_size(context, output, text, 6);
    hemp_text_append_string(text, "--EOF--");     // TMP
    return output;
}



