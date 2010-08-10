#include <hemp/element.h>


/*--------------------------------------------------------------------------
 * space
 *--------------------------------------------------------------------------*/

static struct hemp_symbol_s
    hemp_symbol_space = { 
        "space",                                    /* name                 */
        NULL,                                       /* start token          */
        NULL,                                       /* end token            */
        HEMP_IS_SPACE       |                       /* flags                */
        HEMP_IS_FIXED       |
        HEMP_IS_STATIC      | 
        HEMP_IS_HIDDEN,
        0, 0,                                       /* l/r precedence       */
        NULL,                                       /* scanner callback     */
        NULL,                                       /* cleanup callback     */
        &hemp_element_next_expr,                    /* parse expression     */
        &hemp_element_next_infix,                   /* parse infix expr     */
        &hemp_element_literal_source,               /* source code          */
        &hemp_element_literal_text,                 /* output text          */
        &hemp_element_literal_number,               /* numeric conversion   */
        &hemp_element_literal_integer,              /* integer conversion   */
        &hemp_element_literal_boolean,              /* boolean conversion   */
    };


HEMP_SYMBOL_FUNC(hemp_element_space_symbol) {
    symbol->expr    = &hemp_element_next_expr;
    symbol->infix   = &hemp_element_next_infix;
    symbol->source  = &hemp_element_literal_source;
    symbol->text    = &hemp_element_not_text;
    symbol->number  = &hemp_element_not_number;
    symbol->integer = &hemp_element_not_integer;
    symbol->boolean = &hemp_element_not_boolean;
    symbol->flags   = HEMP_IS_SPACE | HEMP_IS_FIXED | HEMP_IS_STATIC 
                    | HEMP_IS_HIDDEN;
    return symbol;
}


/*--------------------------------------------------------------------------
 * tag start
 *--------------------------------------------------------------------------*/

static struct hemp_symbol_s
    hemp_symbol_tag_start = { 
        "tag_start",                                /* name                 */
        NULL,                                       /* start token          */
        NULL,                                       /* end token            */
        HEMP_IS_SPACE       |                       /* flags                */
        HEMP_IS_FIXED       |
        HEMP_IS_STATIC      | 
        HEMP_IS_HIDDEN      |
        HEMP_IS_SEPARATOR,
        0, 0,                                       /* l/r precedence       */
        NULL,                                       /* scanner callback     */
        NULL,                                       /* cleanup callback     */
        &hemp_element_next_expr,                    /* parse expression     */
        &hemp_element_next_infix,                   /* parse infix expr     */
        &hemp_element_literal_source,               /* source code          */
        &hemp_element_not_text,                     /* output text          */
        &hemp_element_not_number,                   /* numeric conversion   */
        &hemp_element_not_integer,                  /* integer conversion   */
        &hemp_element_not_boolean,                  /* boolean conversion   */
    };


HEMP_SYMBOL_FUNC(hemp_element_tag_start_symbol) {
    symbol->expr    = &hemp_element_next_expr;
    symbol->infix   = &hemp_element_next_infix;
    symbol->source  = &hemp_element_literal_source;
    symbol->flags   = HEMP_IS_SPACE | HEMP_IS_FIXED | HEMP_IS_STATIC 
                    | HEMP_IS_HIDDEN | HEMP_IS_SEPARATOR;
    return symbol;
}


/*--------------------------------------------------------------------------
 * tag end
 *--------------------------------------------------------------------------*/

static struct hemp_symbol_s
    hemp_symbol_tag_end = { 
        "tag_end",                                  /* name                 */
        NULL,                                       /* start token          */
        NULL,                                       /* end token            */
        HEMP_IS_FIXED       |                       /* flags                */
        HEMP_IS_STATIC      | 
        HEMP_IS_HIDDEN      |
        HEMP_IS_TERMINATOR,
        0, 0,                                       /* l/r precedence       */
        NULL,                                       /* scanner callback     */
        NULL,                                       /* cleanup callback     */
        &hemp_element_not_expr,                     /* parse expression     */
        &hemp_element_not_infix,                    /* parse infix expr     */
        &hemp_element_literal_source,               /* source code          */
        &hemp_element_not_text,                     /* output text          */
        &hemp_element_not_number,                   /* numeric conversion   */
        &hemp_element_not_integer,                  /* integer conversion   */
        &hemp_element_not_boolean,                  /* boolean conversion   */
    };


HEMP_SYMBOL_FUNC(hemp_element_tag_end_symbol) {
    symbol->source  = &hemp_element_literal_source;
    symbol->flags   = HEMP_IS_FIXED | HEMP_IS_STATIC | HEMP_IS_HIDDEN
                    | HEMP_IS_TERMINATOR;
    return symbol;
}



/*--------------------------------------------------------------------------
 * end of file element
 *--------------------------------------------------------------------------*/

static struct hemp_symbol_s
    hemp_symbol_eof = { 
        "eof",                                      /* name                 */
        "--EOF--",                                  /* start token          */
        NULL,                                       /* end token            */
        HEMP_IS_FIXED       |                       /* flags                */
        HEMP_IS_STATIC      | 
        HEMP_IS_HIDDEN      |
        HEMP_IS_EOF,
        0, 0,                                       /* l/r precedence       */
        NULL,                                       /* scanner callback     */
        NULL,                                       /* cleanup callback     */
        &hemp_element_not_expr,                     /* parse expression     */
        &hemp_element_not_infix,                    /* parse infix expr     */
        &hemp_element_eof_text,                     /* source code          */   // FIX ME
        &hemp_element_eof_text,                     /* output text          */
        &hemp_element_not_number,                   /* numeric conversion   */
        &hemp_element_not_integer,                  /* integer conversion   */
        &hemp_element_not_boolean,                  /* boolean conversion   */
    };


HEMP_SYMBOL_FUNC(hemp_element_eof_symbol) {
    symbol->source  = &hemp_element_eof_text;
    symbol->text    = &hemp_element_eof_text;
    symbol->flags   = HEMP_IS_FIXED | HEMP_IS_STATIC | HEMP_IS_HIDDEN
                    | HEMP_IS_EOF;
    return symbol;
}

HEMP_OUTPUT_FUNC(hemp_element_eof_text) {
    debug_call("hemp_element_eof_text()\n");
    hemp_text_p text;
    hemp_prepare_output(output, text, 6);
    hemp_text_append_cstr(text, "--EOF--");     // TMP
    return output;
}



/*--------------------------------------------------------------------------
 * static symbol types
 *--------------------------------------------------------------------------*/

hemp_symbol_p HempSymbolSpace     = &hemp_symbol_space;
hemp_symbol_p HempSymbolTagStart  = &hemp_symbol_tag_start;
hemp_symbol_p HempSymbolTagEnd    = &hemp_symbol_tag_end;
hemp_symbol_p HempSymbolEof       = &hemp_symbol_eof;

