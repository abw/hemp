#ifndef HEMP_TEMPLATE_H
#define HEMP_TEMPLATE_H

#include "hemp/dialect.h"
#include "hemp/sources.h"
#include "hemp/elements.h"
#include "hemp/debug.h"


/*--------------------------------------------------------------------------
 * data structures
 *--------------------------------------------------------------------------*/

struct hemp_template {
    hemp_dialect_t  dialect;
    hemp_source_t   source;
    hemp_elements_t elements;
};


/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

hemp_template_t 
    hemp_template_init(
        hemp_cstr_t     scheme, 
        hemp_cstr_t     source, 
        hemp_dialect_t  dialect
    );

void    
    hemp_template_free(
        hemp_template_t
    );

hemp_bool_t 
    hemp_template_compile(
        hemp_template_t
    );


/*--------------------------------------------------------------------------
 * macros
 *--------------------------------------------------------------------------*/

#define hemp_template_null(t)            \
    hemp_template_free(t);               \
    t = NULL


#endif /* HEMP_TEMPLATE_H */