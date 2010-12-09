#include <hemp/scanner.h>


HEMP_INLINE void
hemp_scan_number(
    hemp_document   document
) {
    hemp_string     src     = document->scanptr;
    hemp_num        num_val = 0;
    hemp_int        int_val = 0;
    hemp_bool       is_int  = HEMP_FALSE;
    hemp_fragment   fragment;

//  hemp_debug_scan("scanning number: %s\n", document->scanptr);

    /* number - try integer first */
    errno   = 0;
    int_val = strtol(document->scanptr, &src, 0);
    is_int  = HEMP_TRUE;

    /* If there's a decimal point and a digit following then it's a 
    ** floating point number.  We also look out for e/E which also
    ** indicate fp numbers in scientific notation, e.g. 1.23e6.
    ** Note that we don't accept the decimal point if the next 
    ** character is not a digit.  This is required to support methods
    ** called against numeric constants, e.g. 12345.hex 
    */
    if ( ( *src == '.' && isdigit(*(src + 1)) )
      || ( *src == 'e' || *src == 'E' )
    )  {
        is_int  = HEMP_FALSE;
        num_val = strtod(document->scanptr, &src);
    }

    if (errno == ERANGE) {
        /* TODO: trim next token out of text */
        hemp_throw(document->dialect->hemp, HEMP_ERROR_OVERFLOW, document->scanptr);
    }
    else if (errno) {
        /* should never happen (famous last words) as we pre-check 
        ** that there is at least one valid digit available to be 
        ** parsed, but we check anyway
        */
        hemp_fatal("Unknown number parsing error: %d", errno);
    }
    else if (is_int) {
        fragment = hemp_document_scanned_to(
            document, HempElementInteger, src
        );
        fragment->args.value = hemp_int_val(int_val);
    }
    else {
        fragment = hemp_document_scanned_to(
            document, HempElementNumber, src
        );
        fragment->args.value = hemp_num_val(num_val);
    }
}



/*
hemp_bool
hemp_scan_unplugged(
    hemp_document document
) {
    hemp_fragments   fragments = document->fragments;
    hemp_tagset     tagset   = document->tagset;
    hemp_tag        tag      = tagset->unplugged_tag;
    hemp_string     text     = hemp_source_read(document->source),
                    src      = text,
                    from     = text;
    hemp_pos        pos      = 0;

    if (! tag)
        hemp_fatal("No unplugged tag is defined in tagset\n");

    tag->scan(document, tag, src, pos, &src);
    pos += src - from;
    from = src;

    while (*src)
        src++;

    if (src > from) {
        hemp_fragments_add_fragment(
            fragments, tagset->text_symbol,
            from, pos, src - from
        );
        pos += src - from;
    }
    
    hemp_fragments_add_eof(fragments, pos);

    return HEMP_TRUE;
}

*/


