#include <hemp/element.h>


/*--------------------------------------------------------------------------
 * static element structures
 *--------------------------------------------------------------------------*/

/* NOTE: I'm experimenting with replacing these with constructor functions
 * (see language/hemp) which generate the element types (now called symbols)
 * on demand when they are added to a grammar.  This allows us to also define
 * precedence values at the same time without having to hard-code them for 
 * all possible uses
 */

static struct hemp_etype_s
    hemp_element_space = { 
        "space",
        HEMP_IS_FIXED | HEMP_IS_STATIC | HEMP_IS_HIDDEN,
        NULL,
        &hemp_element_next_skip_space,
        &hemp_element_next_skip_delimiter,
        &hemp_element_next_skip_separator,
        &hemp_element_space_parse_expr,
        &hemp_element_literal_text
    };


static struct hemp_etype_s
    hemp_element_comment = { 
        "comment",
        HEMP_IS_FIXED | HEMP_IS_STATIC | HEMP_IS_HIDDEN,
        NULL,
        &hemp_element_next_skip_space,
        &hemp_element_next_skip_delimiter,
        &hemp_element_next_skip_separator,
        &hemp_element_space_parse_expr,
        &hemp_element_literal_text
    };


static struct hemp_etype_s
    hemp_element_tag_start = { 
        "tag_start",
        HEMP_IS_FIXED | HEMP_IS_STATIC | HEMP_IS_HIDDEN,
        NULL,
        &hemp_element_next_skip_space,
        &hemp_element_next_skip_delimiter,      /* tag start is a statement delimiter... */
        &hemp_element_dont_skip,                /* ...but not an expression separator    */
        &hemp_element_space_parse_expr,  // or force delimiter skip?
        &hemp_element_literal_text
    };

static struct hemp_etype_s
    hemp_element_tag_end = { 
        "tag_end",
        HEMP_IS_FIXED | HEMP_IS_STATIC | HEMP_IS_HIDDEN,
        NULL,
        &hemp_element_dont_skip,                /* slightly different for tag end */
        &hemp_element_next_skip_delimiter,      
        &hemp_element_dont_skip,
        &hemp_element_dont_parse,
        &hemp_element_literal_text
    };


static struct hemp_etype_s
    hemp_element_eof = { 
        "EOF",
        HEMP_IS_FIXED | HEMP_IS_STATIC | HEMP_IS_HIDDEN | HEMP_IS_EOF,
        NULL,
        &hemp_element_dont_skip,                /* can't skip over EOF */
        &hemp_element_dont_skip,
        &hemp_element_dont_skip,
        &hemp_element_dont_parse,
        &hemp_element_eof_text
    };


/*--------------------------------------------------------------------------
 * static element types
 *--------------------------------------------------------------------------*/

hemp_etype_p HempElementSpace     = &hemp_element_space;
hemp_etype_p HempElementTagStart  = &hemp_element_tag_start;
hemp_etype_p HempElementTagEnd    = &hemp_element_tag_end;
hemp_etype_p HempElementComment   = &hemp_element_comment;
hemp_etype_p HempElementEof       = &hemp_element_eof;


/*--------------------------------------------------------------------------
 * function definitions
 *--------------------------------------------------------------------------*/

hemp_element_p
hemp_element_space_parse_expr(
    HEMP_PARSE_PROTO
) {
    debug_call("hemp_element_space_parse_expr()\n");

    /* Advance the pointer to the next element after this one and then skip
     * any further whitespace tokens.  Then ask the next token to return 
     * an expression.
     */
    hemp_element_next_skip_space(elemptr);
    return hemp_parse_expr(elemptr, scope, precedence, force);
}


hemp_text_p
hemp_element_eof_text(
    hemp_element_p  element,
    hemp_text_p     text
) {
    debug_call("hemp_element_eof_text()\n");
    
    if (! text)
        text = hemp_text_init(8);           // TODO: have a dedicate empty string

    hemp_text_append_cstr(text, "--EOF--");
    return text;
}

hemp_text_p
hemp_element_no_text(
    hemp_element_p  element,
    hemp_text_p     text
) {
    debug_call("hemp_element_no_text()\n");
    
    if (! text)
        text = hemp_text_init(0);           // TODO: have a dedicate empty string

    return text;
}
