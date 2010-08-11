#include <hemp/element.h>


static struct hemp_symbol_s
    hemp_symbol_squote = { 
        "squote",                                   /* name                 */
        NULL,                                       /* start token          */
        NULL,                                       /* end token            */
        HEMP_BE_STATIC,
        0, 0,                                       /* l/r precedence       */
        &hemp_element_squote_scanner,               /* scanner callback     */
        &hemp_element_text_clean,                   /* cleanup callback     */
        &hemp_element_literal_expr,                 /* parse expression     */
        &hemp_element_not_infix,                    /* parse infix expr     */
        &hemp_element_literal_source,               /* source code          */
        &hemp_element_literal_text,                 /* output text          */
        &hemp_element_literal_number,               /* numeric conversion   */
        &hemp_element_literal_integer,              /* integer conversion   */
        &hemp_element_literal_boolean,              /* boolean conversion   */
    };

hemp_symbol_p HempSymbolSQuote = &hemp_symbol_squote;

HEMP_SYMBOL_FUNC(hemp_element_squote_symbol) {
    symbol->flags      = HEMP_BE_STATIC;
    symbol->scanner    = &hemp_element_squote_scanner;
    symbol->cleanup    = &hemp_element_text_clean;
    symbol->expr       = &hemp_element_literal_expr,
    symbol->source     = &hemp_element_literal_source;
    symbol->text       = &hemp_element_literal_text;
    symbol->number     = &hemp_element_literal_number;
    symbol->integer    = &hemp_element_literal_integer;
    symbol->boolean    = &hemp_element_literal_boolean;

    return symbol;
}


HEMP_SCAN_FUNC(hemp_element_squote_scanner) {
    hemp_cstr_p     src      = *srcptr;
    hemp_bool_t     is_fixed = HEMP_TRUE;
    hemp_cstr_p     end      = symbol->end;
    hemp_size_t     endlen   = strlen(end);
    hemp_element_p  element;

    debug_call("hemp_element_squote_scanner()\n");

    if (! end)
        end = start;

//  debug("end token: %s  length: %d\n", end, endlen);

    /* walk to the end */
    while (1) {
        if (*src == *end
        && (endlen == 1 || hemp_cstrn_eq(src, end, endlen)))
            break;
        if (*src == HEMP_BACKSLASH) {
            src++;
            is_fixed = HEMP_FALSE;
        }
        if (! *++src)
            hemp_fatal("unterminated single quoted string: %s", start);     // FIXME
    }

    /* skip over the terminating character(s) */
    end  = src;
    src += endlen;

    debug_token("SQUOTE", start, src-start);

    element = hemp_elements_append(
        tmpl->elements, symbol,
        start, pos, src - start
    );

    if (is_fixed) {
        /* we can generate the output text from the source token */
        hemp_set_flag(element, HEMP_BE_FIXED);
    }
    else {
        /* we need to create a new string with escapes resolved */
        hemp_cstr_p sqfrom  = *srcptr;
        hemp_cstr_p squote  = (hemp_cstr_p) hemp_mem_alloc(end - sqfrom + 1);        // CHECK ME
        element->args.value = HEMP_STR_VAL(squote);
                    
        while (sqfrom < end) {
            /* skip past the '\' if we've got "\\" or "\'" */
            if (*sqfrom == HEMP_BACKSLASH 
            && ( *(sqfrom + 1) == HEMP_BACKSLASH 
            ||   *(sqfrom + 1) == *end ))
                sqfrom++;

            *squote++ = *sqfrom++;
        }
        *squote = HEMP_NUL;
    }

    *srcptr = src;
    return element;
}

