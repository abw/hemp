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