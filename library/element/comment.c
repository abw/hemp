#include <hemp/element.h>


static struct hemp_symbol_s
    hemp_symbol_comment = { 
        "comment",                                  /* name                 */
        NULL,                                       /* start token          */
        NULL,                                       /* end token            */
        HEMP_BE_WHITESPACE       |                       /* flags                */
        HEMP_BE_FIXED       |
        HEMP_BE_STATIC      |
        HEMP_BE_HIDDEN,
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

hemp_symbol_p HempSymbolComment = &hemp_symbol_comment;


HEMP_SYMBOL_FUNC(hemp_element_comment_symbol) {
    symbol->scanner    = &hemp_element_comment_scanner;
    symbol->source     = &hemp_element_literal_source;
    symbol->flags      = HEMP_BE_WHITESPACE | HEMP_BE_FIXED | HEMP_BE_STATIC 
                       | HEMP_BE_HIDDEN;
    return symbol;
}


HEMP_SCAN_FUNC(hemp_element_comment_scanner) {
    hemp_cstr_p     src     = *srcptr;
    hemp_cstr_p     tag_end = tag->end;
    hemp_size_t     tag_len = strlen(tag->end);

    debug("hemp_element_comment_scanner()\n");

    /* walk to the end of line or end of tag */
    while (* ++src) {
        if (*src == HEMP_LF) {
            src++;
            break;
        }
        else if (*src == HEMP_CR) {
            src++;
            if (*src == HEMP_LF)
                src++;
            break;
        }
        else if (*src == *tag_end && hemp_cstrn_eq(src, tag_end, tag_len)) {
            break;
        }
    }

    /* update the source pointer past the text we've consumed */
    *srcptr = src;

    /* add a comment element to the list of scanned tokens */
    return hemp_elements_append(
        tmpl->elements, symbol,
        start, pos, src - start
    );
}

