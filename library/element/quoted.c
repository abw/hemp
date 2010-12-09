#include <hemp/element.h>


/*--------------------------------------------------------------------------
 * single quoted strings
 *--------------------------------------------------------------------------*/

HEMP_ELEMENT(hemp_element_squote) {
    element->flags          = HEMP_BE_FIXED;
    element->scanner        = &hemp_element_squote_scanner;
    element->cleanup        = &hemp_element_text_cleanup;
    element->parse_prefix   = &hemp_element_literal_prefix;
    element->token          = &hemp_element_literal_text;
    element->source         = &hemp_element_literal_text;
    element->text           = &hemp_element_quoted_text;
    element->value          = &hemp_element_quoted_value;
    element->values         = &hemp_element_value_values;
    return element;
}


HEMP_SCANNER(hemp_element_squote_scanner) {
    hemp_element    element   = (hemp_element) self;
    hemp_string     src       = template->scanptr;
    hemp_string     start     = template->scanptr;
    hemp_string     end       = element->end;
    hemp_size       endlen    = strlen(end);
    hemp_bool       is_source = HEMP_TRUE;
    hemp_fragment   fragment;

    hemp_debug_call("hemp_element_squote_scanner()\n");

    /* walk to the end */
    while (1) {
        if (*src == *end
        && (endlen == 1 || hemp_stringn_eq(src, end, endlen)))
            break;
        if (*src == HEMP_BACKSLASH) {
            src++;
            is_source = HEMP_FALSE;
        }
        if (! *++src) {
            hemp_fatal("Unterminated single quote: %s\n", start);  
            // FIXME
            // HEMP_SCANNER_ERROR(template, BADQUOTE, HEMP_STR_QUOTED, start);
        }
    }

    /* skip over the terminating character(s) */
    end  = src;
    src += endlen;

    fragment = hemp_template_scanned_to(
        template, element, src
    );

    if (is_source) {
        /* we can generate the output text from the source token */
        hemp_set_flag(fragment, HEMP_BE_SOURCE);
    }
    else {
        /* we need to create a new string with escapes resolved */
        hemp_string sqfrom   = start;
        hemp_string squote   = (hemp_string) hemp_mem_alloc(end - sqfrom + 1);
        fragment->args.value = hemp_str_val(squote);
                    
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

    return HEMP_TRUE;
}


/*--------------------------------------------------------------------------
 * double quoted strings
 *--------------------------------------------------------------------------*/

HEMP_ELEMENT(hemp_element_dquote) {
    element->scanner        = &hemp_element_dquote_scanner;
    element->cleanup        = &hemp_element_text_cleanup;
    element->parse_prefix   = &hemp_element_literal_prefix;
    element->token          = &hemp_element_literal_text;
    element->source         = &hemp_element_literal_text;
    element->text           = &hemp_element_quoted_text;
    element->value          = &hemp_element_quoted_value;
    element->values         = &hemp_element_value_values;
    return element;
}


HEMP_SCANNER(hemp_element_dquote_scanner) {
    hemp_element    element   = (hemp_element) self;
    hemp_string     src       = template->scanptr;
    hemp_string     start     = template->scanptr;
    hemp_string     end       = element->end;
    hemp_size       endlen    = strlen(end);
    hemp_bool       is_source = HEMP_TRUE;
    hemp_fragment   fragment;

    hemp_debug_call("hemp_element_dquote_scanner()\n");

    /* walk to the end */
    while (1) {
        if (*src == *end
        && (endlen == 1 || hemp_stringn_eq(src, end, endlen)))
            break;
        if (*src == HEMP_BACKSLASH) {
            src++;
            is_source = HEMP_FALSE;
        }
        if (! *++src) {
            hemp_fatal("bad quoted string: %s\n", start);
            // FIXME
            // HEMP_SCAN_ERROR(BADQUOTE, HEMP_STR_QUOTED, start);
        }
    }

    /* skip over the terminating character(s) */
    end  = src;
    src += endlen;

    fragment = hemp_template_scanned_to(
        template, element, src
    );

    if (is_source) {
        /* we can generate the output text from the source token */
        hemp_set_flag(fragment, HEMP_BE_SOURCE);
    }
    else {
        /* we need to create a new string with escapes resolved */
        hemp_string dqfrom   = start;
        hemp_string dquote   = (hemp_string) hemp_mem_alloc(end - dqfrom + 1);        // CHECK ME
        fragment->args.value = hemp_str_val(dquote);
                    
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

    return HEMP_TRUE;
}


/*--------------------------------------------------------------------------
 * generic text output function for quoted strings
 *--------------------------------------------------------------------------*/

HEMP_OUTPUT(hemp_element_quoted_text) {

    hemp_fragment fragment = hemp_val_frag(value);
    hemp_debug_call("hemp_element_quoted_text(%p) [%s]\n", fragment, fragment->type->name);
    hemp_text text;
    hemp_prepare_text_size(context, output, text, fragment->length);

    if (hemp_has_flag(fragment, HEMP_BE_SOURCE)) {
        /* quoted string can be regenerated from source */
        hemp_element element = fragment->type;
        hemp_size    slen    = element->start ? strlen(element->start) : 0;
        hemp_size    elen    = element->end   ? strlen(element->end)   : 0;

        /* "pinch" in the ends of the token range to avoid the quotes */
        hemp_text_append_stringn(
            text, 
            fragment->token  + slen,         /* skip past start              */
            fragment->length - slen - elen   /* shorten by start/end lengths */
        );
    }
    else {
        /* TODO: check that it's OK to assume we always have a value */
        hemp_text_append_string(text, hemp_val_str(fragment->args.value));
    }

    return output;
}


HEMP_VALUE(hemp_element_quoted_value) {
    return hemp_element_quoted_text(value, context, HempNothing);
}

