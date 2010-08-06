#ifndef HEMP_SCANNER_H
#define HEMP_SCANNER_H

#include <hemp/template.h>


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

hemp_bool_t
    hemp_scan_text(
        hemp_template_p
    );

//hemp_tag_p
//    hemp_scan_tag_start(
//        hemp_pnode_p, 
//        hemp_cstr_p *
//    );



#endif /* HEMP_SCANNER_H */