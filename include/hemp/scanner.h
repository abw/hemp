#ifndef HEMP_SCANNER_H
#define HEMP_SCANNER_H

#include <hemp/template.h>


/* not sure about the name/structure of this... might be better to be more 
 * generic so we can store parse-time scan and/or runtime error position.
 */

struct hemp_scan_pos_s {
    hemp_template_p tmpl;
    hemp_tag_p      tag;
    hemp_str_p      start;
    hemp_pos_t      pos;
    hemp_str_p      current;
    hemp_symbol_p   symbol;
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

hemp_scan_pos_p
    hemp_scan_pos_init(
        HEMP_SCAN_ARGS
    );

void 
    hemp_scan_pos_free(
        hemp_scan_pos_p scan_pos
    );

hemp_bool_t
    hemp_scan_text(
        hemp_template_p
    );

//hemp_tag_p
//    hemp_scan_tag_start(
//        hemp_pnode_p, 
//        hemp_str_p *
//    );



#endif /* HEMP_SCANNER_H */