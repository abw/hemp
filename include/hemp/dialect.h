#ifndef HEMP_DIALECT_H
#define HEMP_DIALECT_H

#include "hemp/memory.h"
#include "hemp/tags.h"
#include "hemp/ptree.h"
#include "hemp/elements.h"
#include "hemp/sources.h"


/*--------------------------------------------------------------------------
 * data structures
 *--------------------------------------------------------------------------*/

struct hemp_dialect {
    hemp_name_t     name;
    hemp_scan_fn    scanner;
    hemp_tagset_t   tagset;
};


/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

hemp_dialect_t  
    hemp_dialect_init(
        hemp_name_t
    );

hemp_dialect_t  
    hemp_dialect_tt3();
    
void
    hemp_dialect_free(
        hemp_dialect_t
    );


/*--------------------------------------------------------------------------
 * macros
 *--------------------------------------------------------------------------*/

#define hemp_dialect_null(d) \
    hemp_dialect_free(d);    \
    d = NULL;


#endif /* HEMP_DIALECT_H */