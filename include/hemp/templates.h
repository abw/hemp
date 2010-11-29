#ifndef HEMP_TEMPLATES_H
#define HEMP_TEMPLATES_H

#include <hemp/template.h>

// NOTE: may not be required after all - putting functionality straight into 
// hemp


/*--------------------------------------------------------------------------
 * data structures
 *--------------------------------------------------------------------------*/

struct hemp_templates_s {
    hemp_hemp       hemp;
    hemp_hash       templates;
    hemp_dialect    dialect;

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
    hemp_hemp       hemp
);

void
hemp_templates_free(
    hemp_templates  templates
);


#endif /* HEMP_TEMPLATES_H */