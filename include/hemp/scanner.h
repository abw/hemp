#ifndef HEMP_SCANNER_H
#define HEMP_SCANNER_H

#include "hemp/element.h"
#include "hemp/elements.h"
#include "hemp/tags.h"
#include "hemp/debug.h"

#define CR 13
#define LF 10

void hemp_scan_text(
    hemp_text_t     text, 
    hemp_tagset_t   tags
);

/*
ATTIC_TAG attic_scan_text_pnode(
    ATTIC_PNODE pnode, 
    ATTIC_TEXT *srcptr
);
*/

#endif /* HEMP_SCANNER_H */