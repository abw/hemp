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

hemp_bool
hemp_scan_text(
    hemp_document   document
);

HEMP_INLINE void
hemp_scan_number(
    hemp_document   document
);

//hemp_bool
//hemp_scan_unplugged(
//    hemp_document   document
//);



#endif /* HEMP_SCANNER_H */