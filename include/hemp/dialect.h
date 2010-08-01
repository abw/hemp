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
    hemp_cstr_t     name;
    hemp_scan_fn    scanner;
    hemp_hash_t     tags;
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

hemp_bool_t
    hemp_dialect_add_tag(
        hemp_name_t name,
        hemp_tag_t  tag
    );


/*--------------------------------------------------------------------------
 * macros
 *--------------------------------------------------------------------------*/

#define hemp_dialect_null(d) \
    hemp_dialect_free(d);    \
    d = NULL;


#endif /* HEMP_DIALECT_H */