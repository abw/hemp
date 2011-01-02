#ifndef HEMP_SCANNER_H
#define HEMP_SCANNER_H

#include <hemp/document.h>


#define hemp_scan_while(src, cmp)       \
    do { src++; } while ( cmp(*src) );

#define hemp_scan_until(src, val)       \
    do { src++; } while ( cmp(*src) );

#define hemp_scan_to(src, c)            \
    do { src++; } while ( *src && *src != c );

#define hemp_is_char(c1, c2)            \
    (c1 == c2)

#define hemp_not_char(c1, c2)           \
    (c1 != c2)



/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

HempBool
hemp_scan_text(
    HempDocument   document
);

HEMP_INLINE void
hemp_scan_number(
    HempDocument   document
);

HEMP_INLINE void
hemp_scan_whitespace(
    HempDocument   document
);

//HempBool
//hemp_scan_unplugged(
//    HempDocument   document
//);



#endif /* HEMP_SCANNER_H */