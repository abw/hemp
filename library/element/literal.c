#include <hemp/element.h>

HempElement HempElementLiteral = NULL;


HEMP_GLOBAL_ELEMENT(hemp_global_element_literal) {
    return hemp_element_word(
        NULL,
        hemp_element_new("hemp.literal", NULL, NULL)
    );
}


HEMP_ELEMENT(hemp_element_literal) {
    element->parse_fixed = &hemp_element_literal_fixed;
    element->token       = &hemp_element_literal_text;
    element->source      = &hemp_element_literal_text;
    element->text        = &hemp_element_literal_text;
    element->value       = &hemp_element_literal_value;
    element->cleanup     = &hemp_element_literal_cleanup;
    return element;
}


HEMP_PREFIX(hemp_element_literal_fixed) {
    hemp_debug_call("hemp_element_parse_fixed()\n");
    HempFragment fragment = hemp_element_parse_fixed(HEMP_PREFIX_ARG_NAMES);

//  hemp_debug_msg("reblessing %s to word\n", fragment->type->name);

    /* "re-bless" fragment into the literal element class.  This allows
     * keywords to be used in places where fixed words are expected.
     * e.g. "list.each(...)".  Here the 'each' keyword has the fixed()
     * method called on it by the dotop and, assuming that the element
     * vtable for 'each' maps the prefix() method to this function, the
     * fragment representing the 'each' word will be downgraded to a plain
     * old literal word token.
     */
    fragment->type = HempElementLiteral;

    return fragment;
}


HEMP_PREFIX(hemp_element_literal_prefix) {
    hemp_debug_call("hemp_element_literal_prefix()\n"); 
    HempFragment fragment = *fragptr;

    return hemp_advance(fragptr)
        ? hemp_parse_postfix(fragptr, scope, precedence, force, fragment)
        : fragment;
}


HEMP_OUTPUT(hemp_element_literal_text) {
    hemp_debug_call("hemp_element_literal_text()\n");

    /* TODO: merge this with hemp_element_element().  They were 
     * different but are now the same
     */
    HempText text;
    HempFragment fragment = hemp_val_frag(value);
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

    HempElement element = (HempElement) self;
    HempString  src     = document->scanptr;

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
    HempString     buffer, 
    HempFragment   fragment, 
    HempSize       max
) {
    if (fragment->length < max)
        max = fragment->length;
    strncpy(buffer, fragment->token, max);
    *(buffer + max) = HEMP_NUL;
}


HEMP_INLINE void
hemp_buffer_fragcat(
    HempString     buffer, 
    HempFragment   fragment, 
    HempSize       max
) {
    if (fragment->length < max)
        max = fragment->length;
    strncat(buffer, fragment->token, max);
}


HempBool
hemp_match_end_fragment(
    HempFragment * fragptr,
    HempFragment   start
) {
    static HempChar    error_buffer[128];
    static HempString  err1 = (HempString) error_buffer;
    static HempString  err2 = (HempString) error_buffer + 64;

    HempFragment   end = *fragptr;
    HempFragment   startfrag, endfrag;

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
        HempSize flen = strlen(endfrag->type->start);
        
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

