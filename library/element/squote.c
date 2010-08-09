#include <hemp/element.h>


struct hemp_symbol_s
    hemp_symbol_squote = { 
        "squote",
        "squote:", NULL,
        HEMP_IS_STATIC, 0, 0,
        NULL,
        (hemp_clean_f) &hemp_element_text_clean,
        &hemp_skip_none_vtable,
        NULL, NULL,
        &hemp_element_quoted_text,
        &hemp_element_literal_parse_expr
    };

hemp_symbol_p HempSymbolSQuote = &hemp_symbol_squote;


hemp_element_p
hemp_element_squote_scanner(
    hemp_template_p tmpl,
    hemp_tag_p      tag,
    hemp_cstr_p     start,
    hemp_pos_t      pos,
    hemp_cstr_p     *srcptr,
    hemp_symbol_p   symbol
) {
    hemp_cstr_p     src      = *srcptr;
    hemp_bool_t     is_fixed = HEMP_TRUE;
    hemp_element_p  element;

    debug_call("hemp_element_squote_scanner()\n");

    /* walk to the end */
    while ( * ++src && *src != HEMP_SQUOTE ) {
        if (*src == HEMP_BACKSLASH) {
            src++;
            is_fixed = HEMP_FALSE;
        }
    }

    /* check we hit a quote and not the end of string */
    if (! *src)
        hemp_fatal("unterminated single quoted string: %s", start);

    src++;

    debug_token("SQUOTE", start, src-start);

    element = hemp_elements_append(
        tmpl->elements, symbol,
        start, pos, src - start
    );

    if (is_fixed) {
        /* we can generate the output text from the source token */
        hemp_set_flag(element, HEMP_IS_FIXED);
    }
    else {
        /* we need to create a new string with escapes resolved */
        hemp_cstr_p sqfrom  = start + 1;
        hemp_cstr_p squote  = 
        element->value.text = (hemp_cstr_p) hemp_mem_alloc(src - start - 1);
                    
        while (sqfrom < src) {
            /* skip past the '\' if we've got "\\" or "\'" */
            if (*sqfrom == HEMP_BACKSLASH 
            && ( *(sqfrom + 1) == HEMP_SQUOTE 
            ||   *(sqfrom + 1) == HEMP_BACKSLASH ))
                sqfrom++;

            *squote++ = *sqfrom++;
        }
        *--squote = HEMP_NUL;
    }

    *srcptr = src;
    return element;
}


hemp_symbol_p
hemp_element_squote_symbol(
    hemp_p        hemp,
    hemp_symbol_p symbol
) {
    symbol->flags      = HEMP_IS_STATIC;
    symbol->scanner    = &hemp_element_squote_scanner;
    symbol->cleanup    = (hemp_clean_f) &hemp_element_text_clean;
    symbol->parse_expr = &hemp_element_literal_parse_expr,
    symbol->source     = &hemp_element_literal_text;
    symbol->text       = &hemp_element_quoted_text;
    return symbol;
}

