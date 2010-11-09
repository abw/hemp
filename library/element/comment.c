#include <hemp/element.h>


hemp_symbol_p HempSymbolComment = NULL;


HEMP_GLOBAL_SYMBOL(hemp_symbol_comment) {
    hemp_debug_call("hemp_symbol_comment()\n");
    return hemp_element_comment_symbol(
        NULL,
        hemp_symbol_init("hemp.comment", NULL, NULL)
    );
}


HEMP_SYMBOL_FUNC(hemp_element_comment_symbol) {
    symbol->scanner    = &hemp_element_comment_scanner;
    symbol->token      = &hemp_element_literal_token;
    symbol->source     = &hemp_element_literal_source;
    symbol->flags      = HEMP_BE_WHITESPACE | HEMP_BE_SOURCE | HEMP_BE_FIXED 
                       | HEMP_BE_HIDDEN;
    return symbol;
}


HEMP_SCAN_FUNC(hemp_element_comment_scanner) {
    hemp_str_p  tag_end = tag->end;
    hemp_size_t tag_len = strlen(tag->end);

    hemp_debug_call("hemp_element_comment_scanner()\n");

    /* update the source pointer past the text we've consumed */
    *srcptr = tag->to_end_of_line(tag, *srcptr);

    /* add a comment element to the list of scanned tokens */
    return hemp_elements_append(
        tmpl->elements, symbol,
        start, pos, *srcptr - start
    );
}

