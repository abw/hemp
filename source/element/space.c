#include "hemp/elements.h"
#include "hemp/debug.h"


/*--------------------------------------------------------------------------
 * static element structures
 *--------------------------------------------------------------------------*/

static struct hemp_etype
    hemp_element_space = { 
        "space",
        /* asking whitespace to skip_space forwards to next token */
        &hemp_element_next_skip_space,
        &hemp_element_next_skip_delimiter,
        &hemp_element_next_skip_separator,
        &hemp_element_space_parse_expr,
        &hemp_element_literal_text
    };


static struct hemp_etype 
    hemp_element_comment = { 
        "comment",
        &hemp_element_next_skip_space,
        &hemp_element_next_skip_delimiter,
        &hemp_element_next_skip_separator,
        &hemp_element_space_parse_expr,
        &hemp_element_literal_text
    };


static struct hemp_etype
    hemp_element_tag_start = { 
        "tag_start",
        &hemp_element_next_skip_space,
        &hemp_element_next_skip_delimiter,      /* tag start is a statement delimiter... */
        &hemp_element_dont_skip,                /* ...but not an expression separator    */
        &hemp_element_space_parse_expr,  // or force delimiter skip?
        &hemp_element_literal_text
    };

static struct hemp_etype
    hemp_element_tag_end = { 
        "tag_end",
        &hemp_element_dont_skip,                /* slightly different for tag end */
        &hemp_element_next_skip_delimiter,      
        &hemp_element_dont_skip,
        &hemp_element_dont_parse,
        &hemp_element_literal_text
    };


static struct hemp_etype
    hemp_element_eof = { 
        "EOF",
        &hemp_element_dont_skip,                /* can't skip over EOF */
        &hemp_element_dont_skip,
        &hemp_element_dont_skip,
        &hemp_element_dont_parse,
        &hemp_element_eof_text
    };


/*--------------------------------------------------------------------------
 * static element types
 *--------------------------------------------------------------------------*/

hemp_etype_t HempElementSpace     = &hemp_element_space;
hemp_etype_t HempElementTagStart  = &hemp_element_tag_start;
hemp_etype_t HempElementTagEnd    = &hemp_element_tag_end;
hemp_etype_t HempElementComment   = &hemp_element_comment;
hemp_etype_t HempElementEof       = &hemp_element_eof;


/*--------------------------------------------------------------------------
 * function definitions
 *--------------------------------------------------------------------------*/

hemp_element_t
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


hemp_text_t
hemp_element_eof_text(
    hemp_element_t  element,
    hemp_text_t     text
) {
    debug_call("hemp_element_eof_text()\n");
    
    if (! text)
        text = hemp_text_init(8);           // TODO: have a dedicate empty string

    hemp_text_append_cstr(text, "--EOF--");
    return text;
}
