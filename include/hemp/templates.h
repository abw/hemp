#ifndef HEMP_TEMPLATES_H
#define HEMP_TEMPLATES_H

#include <hemp/template.h>

// NOTE: may not be required after all - putting functionality straight into 
// hemp


/*--------------------------------------------------------------------------
 * data structures
 *--------------------------------------------------------------------------*/

struct hemp_templates_s {
    hemp_p          hemp;
    hemp_hash_p     templates;
    hemp_dialect_p  dialect;

    // TODO: cache
    // TODO: provider path
    // TODO: dialects map
    // TODO: file extension mappings, etc.
};


/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

hemp_templates_p
    hemp_templates_init(
        hemp_p hemp
    );

void
    hemp_templates_free(
        hemp_templates_p
    );


#endif /* HEMP_TEMPLATES_H */