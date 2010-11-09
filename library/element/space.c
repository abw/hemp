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

HEMP_SYMBOL_FUNC(hemp_element_terminator_symbol) {
    hemp_element_literal_symbol(hemp, symbol);
    symbol->prefix  = hemp_element_not_prefix;
    symbol->postfix = hemp_element_not_postfix;
    symbol->fixed   = hemp_element_decline;
    symbol->flags   = HEMP_BE_SOURCE | HEMP_BE_FIXED | HEMP_BE_HIDDEN
                    | HEMP_BE_TERMINATOR;
    return symbol;
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
    symbol->postfix = &hemp_element_next_postfix;
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
    symbol->postfix = &hemp_element_next_postfix;
    symbol->flags   = HEMP_BE_WHITESPACE | HEMP_BE_SOURCE | HEMP_BE_FIXED 
                    | HEMP_BE_HIDDEN | HEMP_BE_SEPARATOR;
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
    hemp_element_terminator_symbol(hemp, symbol);
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


