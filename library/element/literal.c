#include <hemp/element.h>


HEMP_ELEMENT(hemp_element_literal) {
    element->parse_fixed = &hemp_element_parse_fixed;
    element->token       = &hemp_element_literal_text;
    element->source      = &hemp_element_literal_text;
    element->text        = &hemp_element_literal_text;
    element->value       = &hemp_element_literal_value;
    element->cleanup     = &hemp_element_literal_cleanup;
    return element;
}


HEMP_PREFIX(hemp_element_literal_prefix) {
    hemp_debug_call("hemp_element_literal_prefix()\n"); 
    hemp_fragment fragment = *fragptr;

    return hemp_advance(fragptr)
        ? hemp_parse_postfix(fragptr, scope, precedence, force, fragment)
        : fragment;
}


HEMP_OUTPUT(hemp_element_literal_text) {
    hemp_debug_call("hemp_element_literal_text()\n");

    /* TODO: merge this with hemp_element_element().  They were 
     * different but are now the same
     */
    hemp_text text;
    hemp_fragment fragment = hemp_val_frag(value);
    hemp_prepare_text_size(context, output, text, fragment->length);
    hemp_text_append_stringn(text, fragment->token, fragment->length);
    return output;
}


HEMP_VALUE(hemp_element_literal_value) {
    return hemp_element_literal_text(value, context, HempNothing);
}


HEMP_CLEANUP(hemp_element_literal_cleanup) {
    hemp_debug_call("hemp_element_literal_cleanup(%p)\n", fragment);

    /* literal elements may be used as fixed values in which case they
     * have memory allocated (via hemp_element_parse_fixed()) which we must 
     * clean up.
     */

    if (hemp_has_flag(fragment, HEMP_BE_ALLOCATED)) {
        hemp_mem_free(
            hemp_val_str( 
                hemp_expr(fragment)
            )
        );
    }
}


/*--------------------------------------------------------------------------
 * fragment: #something
 *--------------------------------------------------------------------------*/

HEMP_ELEMENT(hemp_element_fragment) {
    element->parse_body  = NULL;
    element->scanner     = &hemp_element_fragment_scanner;
    element->token       = &hemp_element_literal_text;
    element->source      = &hemp_element_literal_text;
    hemp_set_flag(element, HEMP_BE_FRAGMENT);
    return element;
}


HEMP_SCANNER(hemp_element_fragment_scanner) {
    hemp_debug_call("hemp_element_fragment_scanner()\n");

    hemp_element element = (hemp_element) self;
    hemp_string  src     = document->scanptr;

    while (isalnum(*src) || *src == '_' || * src == '-') {
        src++;
    }

    if (src == document->scanptr) {
        HEMP_SCAN_ERROR(document, HEMP_ERROR_NOFRAG, element->start);
    }
    
    /* add a fragment element to the list of scanned tokens */
    hemp_document_scanned_to(
        document, element, src
    );

    return HEMP_TRUE;
}


HEMP_INLINE void
hemp_buffer_fragcpy(
    hemp_string     buffer, 
    hemp_fragment   fragment, 
    hemp_size       max
) {
    if (fragment->length < max)
        max = fragment->length;
    strncpy(buffer, fragment->token, max);
    *(buffer + max) = HEMP_NUL;
}


HEMP_INLINE void
hemp_buffer_fragcat(
    hemp_string     buffer, 
    hemp_fragment   fragment, 
    hemp_size       max
) {
    if (fragment->length < max)
        max = fragment->length;
    strncat(buffer, fragment->token, max);
}


hemp_bool
hemp_match_end_fragment(
    hemp_fragment * fragptr,
    hemp_fragment   start
) {
    static hemp_char    error_buffer[128];
    static hemp_string  err1 = (hemp_string) error_buffer;
    static hemp_string  err2 = (hemp_string) error_buffer + 64;

    hemp_fragment   end = *fragptr;
    hemp_fragment   startfrag, endfrag;

    hemp_advance(fragptr);

    /* see if there's a fragment on the end */
    if (hemp_not_flag(*fragptr, HEMP_BE_FRAGMENT))
        return HEMP_FALSE;
    
    endfrag = *fragptr;
    hemp_advance(fragptr);

    /* The fragment for the end tag must match the start tag fragment if it
     * has one or the start tag name if not.
     */

    if (hemp_has_flag(start->next, HEMP_BE_FRAGMENT)) {
        startfrag = start->next;
        if ( startfrag->length != endfrag->length 
          || ! hemp_stringn_eq(startfrag->token, endfrag->token, startfrag->length)
        ) {
            hemp_buffer_fragcpy(err1, start,     31);
            hemp_buffer_fragcat(err1, startfrag, 31);
            hemp_buffer_fragcpy(err2, end,       31);
            hemp_buffer_fragcat(err2, endfrag,   31);
            HEMP_PARSE_ERROR(endfrag, HEMP_ERROR_FRAGMATCH, err1, err2);
        }
    }
    else {
        hemp_size flen = strlen(endfrag->type->start);
        
        if ( endfrag->length - flen != start->length 
          || ! hemp_stringn_eq(endfrag->token + flen, start->token, start->length)
        ) {
            hemp_buffer_fragcpy(err1, start,     31);
            hemp_buffer_fragcpy(err2, end,       31);
            hemp_buffer_fragcat(err2, endfrag,   31);
            HEMP_PARSE_ERROR(endfrag, HEMP_ERROR_FRAGMATCH, err1, err2);
        }
    }
    return HEMP_TRUE;
}

