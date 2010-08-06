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

static struct hemp_symbol_s
    hemp_symbol_space = { 
        "space",                                            /* name  */
        "space:",                                           /* token */
        HEMP_IS_FIXED | HEMP_IS_STATIC | HEMP_IS_HIDDEN,    /* flags */
        0, 0,                                               /* precedence */
        NULL, NULL,                                         /* scanner, cleaner */
        &hemp_skip_all_vtable,                              /* skip */
        NULL, NULL,                                         /* parse, source */
        &hemp_element_literal_text,                         /* text */
        &hemp_element_space_parse_expr                      /* parse */
    };



static struct hemp_symbol_s
    hemp_symbol_comment = { 
        "comment",
        "comment:",
        HEMP_IS_FIXED | HEMP_IS_STATIC | HEMP_IS_HIDDEN,
        0, 0, NULL, NULL,
        &hemp_skip_all_vtable,
        NULL, NULL,
        &hemp_element_literal_text,
        &hemp_element_space_parse_expr
    };


static struct hemp_symbol_s
    hemp_symbol_tag_start = { 
        "tag_start",
        "tag_start:",
        HEMP_IS_FIXED | HEMP_IS_STATIC | HEMP_IS_HIDDEN,
        0, 0, NULL, NULL,
        &hemp_skip_nonsep_vtable,
        NULL, NULL,
        &hemp_element_literal_text,
        &hemp_element_space_parse_expr
    };

static struct hemp_symbol_s
    hemp_symbol_tag_end = { 
        "tag_end",
        "tag_start:",
        HEMP_IS_FIXED | HEMP_IS_STATIC | HEMP_IS_HIDDEN,
        0, 0, NULL, NULL,
        &hemp_skip_delimiter_vtable,
        NULL, NULL,
        &hemp_element_literal_text,
        &hemp_element_dont_parse
    };


static struct hemp_symbol_s
    hemp_symbol_eof = { 
        "EOF",
        "--EOF--",
        HEMP_IS_FIXED | HEMP_IS_STATIC | HEMP_IS_HIDDEN | HEMP_IS_EOF,
        0, 0, NULL, NULL,
        &hemp_skip_none_vtable,
        NULL, NULL,
        &hemp_element_eof_text,
        &hemp_element_dont_parse
    };


/*--------------------------------------------------------------------------
 * static symbol types
 *--------------------------------------------------------------------------*/

hemp_symbol_p HempSymbolSpace     = &hemp_symbol_space;
hemp_symbol_p HempSymbolTagStart  = &hemp_symbol_tag_start;
hemp_symbol_p HempSymbolTagEnd    = &hemp_symbol_tag_end;
hemp_symbol_p HempSymbolComment   = &hemp_symbol_comment;
hemp_symbol_p HempSymbolEof       = &hemp_symbol_eof;


/*--------------------------------------------------------------------------
 * function definitions
 *--------------------------------------------------------------------------*/

hemp_element_p
hemp_element_space_parse_expr(
    HEMP_PARSE_PROTO
) {
    debug("hemp_element_space_parse_expr()\n");
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
