#ifndef HEMP_SCANNER_H
#define HEMP_SCANNER_H

#include <hemp/document.h>


/*--------------------------------------------------------------------------
 * Function prototypes
 *--------------------------------------------------------------------------*/

HempBool
hemp_scan_text(
    HempDocument    document
);

void
hemp_scan_number(
    HempDocument    document
);

void
hemp_scan_whitespace(
    HempDocument    document
);


/*--------------------------------------------------------------------------
 * Macros
 *--------------------------------------------------------------------------*/

#define hemp_is_char(c1, c2)            \
    (c1 == c2)

#define hemp_not_char(c1, c2)           \
    (c1 != c2)

#define hemp_is_ident(c)                \
    (isalnum(c) || c == HEMP_UNDERSCORE)

#define hemp_scan_while(src, cmp)       \
    do { src++; } while ( cmp(*src) );

#define hemp_scan_until(src, val)       \
    do { src++; } while ( cmp(*src) )

#define hemp_scan_to(src, c)            \
    do { src++; } while ( *src && *src != c )

#define hemp_scan_ident(src)            \
    do { src++; } while ( hemp_is_ident(*src) )






#endif /* HEMP_SCANNER_H */
