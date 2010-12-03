#include <hemp/element.h>


hemp_symbol HempSymbolComment = NULL;


HEMP_GLOBAL_SYMBOL(hemp_symbol_comment) {
    hemp_debug_call("hemp_symbol_comment()\n");
    return hemp_element_comment_symbol(
        NULL,
        hemp_symbol_new("hemp.comment", NULL, NULL)
    );
}


HEMP_SYMBOL(hemp_element_comment_symbol) {
    symbol->scanner    = &hemp_element_comment_scanner;
    symbol->token      = &hemp_element_literal_token;
    symbol->source     = &hemp_element_literal_source;
    symbol->flags      = HEMP_BE_WHITESPACE | HEMP_BE_SOURCE | HEMP_BE_FIXED 
                       | HEMP_BE_HIDDEN;
    return symbol;
}


HEMP_SCAN_FUNC(hemp_element_comment_scanner) {
    hemp_debug_call("hemp_element_comment_scanner()\n");

    /* update the source pointer past the text we've consumed */
    *srcptr = tag->to_eol(tag, *srcptr);

    /* add a comment element to the list of scanned tokens */
    return hemp_elements_append(
        tmpl->elements, symbol,
        start, pos, *srcptr - start
    );
}

