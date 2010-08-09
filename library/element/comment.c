#include <hemp/element.h>


static struct hemp_symbol_s
    hemp_symbol_comment = { 
        "comment",
        "comment:",NULL,
        HEMP_IS_FIXED | HEMP_IS_STATIC | HEMP_IS_HIDDEN,
        0, 0, NULL, NULL,
        &hemp_skip_all_vtable,
        NULL, NULL,
        &hemp_element_literal_text,
        &hemp_element_space_parse_expr
    };

hemp_symbol_p HempSymbolComment = &hemp_symbol_comment;



hemp_element_p
hemp_element_comment_scanner(
    hemp_template_p tmpl,
    hemp_tag_p      tag,
    hemp_cstr_p     start,
    hemp_pos_t      pos,
    hemp_cstr_p     *srcptr,
    hemp_symbol_p   symbol
) {
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


hemp_symbol_p
hemp_element_comment_symbol(
    hemp_p        hemp,
    hemp_symbol_p symbol
) {
    symbol->flags      = HEMP_IS_FIXED | HEMP_IS_STATIC | HEMP_IS_HIDDEN;
    symbol->scanner    = &hemp_element_comment_scanner;
    symbol->skip       = &hemp_skip_all_vtable;
    symbol->parse_expr = &hemp_element_space_parse_expr,
    symbol->source     = &hemp_element_literal_text;
    symbol->text       = &hemp_element_no_text;
    return symbol;
}

