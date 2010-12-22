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


HEMP_INLINE void
hemp_scan_whitespace(
    hemp_document   document
) {
    hemp_string     src = document->scanptr;

    if (isspace(*src)) {
        /* whitespace */
        hemp_scan_while(src, isspace);
        hemp_document_scanned_to(
            document, HempElementSpace, src
        );
    }
}

