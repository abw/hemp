#include <hemp/element.h>


static struct hemp_symbol_s
    hemp_symbol_dquote = { 
        "dquote",                                   /* name                 */
        NULL,                                       /* start token          */
        NULL,                                       /* end token            */
        0,                                          /* flags                */
        0, 0,                                       /* l/r precedence       */
        &hemp_element_dquote_scanner,               /* scanner callback     */
        &hemp_element_text_clean,                   /* cleanup callback     */
        &hemp_element_literal_expr,                 /* parse expression     */
        &hemp_element_not_infix,                    /* parse infix expr     */
        &hemp_element_literal_source,               /* source code          */
        &hemp_element_literal_text,                 /* output text          */
        &hemp_element_literal_number,               /* numeric conversion   */
        &hemp_element_literal_integer,              /* integer conversion   */
        &hemp_element_literal_boolean,              /* boolean conversion   */
    };

hemp_symbol_p HempSymbolDQuote = &hemp_symbol_dquote;

HEMP_SYMBOL_FUNC(hemp_element_dquote_symbol) {
    symbol->scanner    = &hemp_element_dquote_scanner;
    symbol->cleanup    = &hemp_element_text_clean;
    symbol->expr       = &hemp_element_literal_expr,
    symbol->source     = &hemp_element_literal_source;
    symbol->text       = &hemp_element_literal_text;
    symbol->number     = &hemp_element_literal_number;
    symbol->integer    = &hemp_element_literal_integer;
    symbol->boolean    = &hemp_element_literal_boolean;
    return symbol;
}


HEMP_SCAN_FUNC(hemp_element_dquote_scanner) {
    hemp_cstr_p     src      = *srcptr;
    hemp_bool_t     is_fixed = HEMP_TRUE;
    hemp_cstr_p     end      = symbol->end;
    hemp_size_t     endlen   = strlen(end);
    hemp_element_p  element;

    debug_call("hemp_element_dquote_scanner()\n");

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
        hemp_cstr_p dqfrom  = *srcptr;
        hemp_cstr_p dquote  = (hemp_cstr_p) hemp_mem_alloc(end - dqfrom + 1);        // CHECK ME
        element->args.value = HEMP_STR_VAL(dquote);
                    
        while (dqfrom < end) {
            if (*dqfrom == HEMP_BACKSLASH) {
                if (*(dqfrom + 1) == *end) {
                    /* end token is escaped, e.g. "18\" Stonehenge monument */
                    dqfrom++;
                    continue;
                }
                switch (*(dqfrom + 1)) {
                    case HEMP_BACKSLASH:
                        /* \" or \\  =>  " or \ */
                        dqfrom++;
                        break;

                    case 'n':
                        /* \n => newline (currently just LF) */
                        *dquote++ = HEMP_NL;
                        dqfrom += 2;
                        break;

                    case 't':
                        /* \t => tab */
                        *dquote++ = HEMP_TAB;
                        dqfrom += 2;
                        break;

                    default:
                        /* \X => X */
                        *dquote++ = *dqfrom++;
                }
            }
            else {
                *dquote++ = *dqfrom++;
            }
        }
        *dquote = HEMP_NUL;
    }

    *srcptr = src;
    return element;
}



