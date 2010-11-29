#ifndef HEMP_SCANNER_H
#define HEMP_SCANNER_H

#include <hemp/template.h>


/* not sure about the name/structure of this... might be better to be more 
 * generic so we can store parse-time scan and/or runtime error position.
 */

struct hemp_scan_pos {
    hemp_template   tmpl;
    hemp_tag        tag;
    hemp_string     start;
    hemp_pos        pos;
    hemp_string     current;
    hemp_symbol     symbol;
};


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

hemp_scan_pos
hemp_scan_pos_init(
    HEMP_SCAN_ARGS
);

void 
hemp_scan_pos_free(
    hemp_scan_pos scan_pos
);

hemp_bool
hemp_scan_text(
    hemp_template
);



#endif /* HEMP_SCANNER_H */