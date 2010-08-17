#include <hemp/element.h>


/*--------------------------------------------------------------------------
 * space
 *--------------------------------------------------------------------------*/

static struct hemp_symbol_s
    hemp_symbol_space = { 
        "space",                                    /* name                 */
        NULL,                                       /* start token          */
        NULL,                                       /* end token            */
        HEMP_BE_WHITESPACE  |                       /* flags                */
        HEMP_BE_SOURCE      |
        HEMP_BE_STATIC      | 
        HEMP_BE_HIDDEN,
        0, 0,                                       /* l/r precedence       */
        NULL,                                       /* scanner callback     */
        NULL,                                       /* cleanup callback     */
        &hemp_element_next_prefix,                  /* prefix expression    */
        &hemp_element_next_postfix,                 /* postfix expression   */
        &hemp_element_literal_token,                /* source token         */
        &hemp_element_literal_source,               /* source code          */
        &hemp_element_literal_text,                 /* output text          */
        &hemp_element_literal_value,                /* output value         */
        &hemp_element_value_number,                 /* numeric conversion   */
        &hemp_element_value_integer,                /* integer conversion   */
        &hemp_element_value_boolean,                /* boolean conversion   */
    };


HEMP_SYMBOL_FUNC(hemp_element_space_symbol) {
    hemp_element_literal_symbol(hemp, symbol);
    symbol->prefix  = &hemp_element_next_prefix;
    symbol->postfix = &hemp_element_next_postfix;
    symbol->flags   = HEMP_BE_WHITESPACE | HEMP_BE_SOURCE | HEMP_BE_STATIC 
                    | HEMP_BE_HIDDEN;
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
        HEMP_BE_WHITESPACE  |                       /* flags                */
        HEMP_BE_SOURCE      |
        HEMP_BE_STATIC      | 
        HEMP_BE_HIDDEN      |
        HEMP_BE_SEPARATOR,
        0, 0,                                       /* l/r precedence       */
        NULL,                                       /* scanner callback     */
        NULL,                                       /* cleanup callback     */
        &hemp_element_next_prefix,                  /* prefix expression    */
        &hemp_element_next_postfix,                 /* postfix expression   */
        &hemp_element_literal_token,                /* source token         */
        &hemp_element_literal_source,               /* source code          */
        &hemp_element_not_text,                     /* output text          */
        &hemp_element_not_value,                    /* output value         */
        &hemp_element_not_number,                   /* numeric conversion   */
        &hemp_element_not_integer,                  /* integer conversion   */
        &hemp_element_not_boolean,                  /* boolean conversion   */
    };


HEMP_SYMBOL_FUNC(hemp_element_tag_start_symbol) {
    hemp_element_literal_symbol(hemp, symbol);
    symbol->prefix  = &hemp_element_next_prefix;
    symbol->postfix = &hemp_element_next_postfix;
    symbol->flags   = HEMP_BE_WHITESPACE | HEMP_BE_SOURCE | HEMP_BE_STATIC 
                    | HEMP_BE_HIDDEN | HEMP_BE_SEPARATOR;
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
        HEMP_BE_SOURCE      |                       /* flags                */
        HEMP_BE_STATIC      | 
        HEMP_BE_HIDDEN      |
        HEMP_BE_TERMINATOR,
        0, 0,                                       /* l/r precedence       */
        NULL,                                       /* scanner callback     */
        NULL,                                       /* cleanup callback     */
        &hemp_element_not_prefix,                   /* prefix expression    */
        &hemp_element_not_postfix,                  /* postfix expr         */
        &hemp_element_literal_token,                /* source token         */
        &hemp_element_literal_source,               /* source code          */
        &hemp_element_not_text,                     /* output text          */
        &hemp_element_not_value,                    /* output value         */
        &hemp_element_not_number,                   /* numeric conversion   */
        &hemp_element_not_integer,                  /* integer conversion   */
        &hemp_element_not_boolean,                  /* boolean conversion   */
    };


HEMP_SYMBOL_FUNC(hemp_element_tag_end_symbol) {
    hemp_element_literal_symbol(hemp, symbol);
    symbol->flags   = HEMP_BE_SOURCE | HEMP_BE_STATIC | HEMP_BE_HIDDEN
                    | HEMP_BE_TERMINATOR;
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
        HEMP_BE_SOURCE      |                       /* flags                */
        HEMP_BE_STATIC      | 
        HEMP_BE_HIDDEN      |
        HEMP_BE_EOF,
        0, 0,                                       /* l/r precedence       */
        NULL,                                       /* scanner callback     */
        NULL,                                       /* cleanup callback     */
        &hemp_element_not_prefix,                   /* prefix expression    */
        &hemp_element_not_postfix,                  /* postfix expression   */
        &hemp_element_eof_token,                    /* source token         */
        &hemp_element_not_source,                   /* source code          */
        &hemp_element_not_text,                     /* output text          */
        &hemp_element_not_value,                    /* output value         */
        &hemp_element_not_number,                   /* numeric conversion   */
        &hemp_element_not_integer,                  /* integer conversion   */
        &hemp_element_not_boolean,                  /* boolean conversion   */
    };


HEMP_SYMBOL_FUNC(hemp_element_eof_symbol) {
    symbol->token   = &hemp_element_eof_token;
    symbol->flags   = HEMP_BE_SOURCE | HEMP_BE_STATIC | HEMP_BE_HIDDEN
                    | HEMP_BE_EOF;
    return symbol;
}

HEMP_ETEXT_FUNC(hemp_element_eof_token) {
    hemp_debug_call("hemp_element_eof_token()\n");
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

