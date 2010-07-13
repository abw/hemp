#ifndef HEMP_SCANNER_H
#define HEMP_SCANNER_H

#include "hemp/element.h"
#include "hemp/elements.h"
#include "hemp/ptree.h"
#include "hemp/tags.h"
#include "hemp/template.h"
#include "hemp/debug.h"

#define CR 13
#define LF 10
#define HEMP_CHAR_SQUOTE    '\''
#define HEMP_CHAR_DQUOTE    '"'


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
        hemp_template_t
    );

hemp_tag_t
    hemp_scan_tag_start(
        hemp_pnode_t, 
        hemp_cstr_t *
    );



#endif /* HEMP_SCANNER_H */