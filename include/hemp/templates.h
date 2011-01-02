#ifndef HEMP_TEMPLATES_H
#define HEMP_TEMPLATES_H

#include <hemp/document.h>

// NOTE: may not be required after all - putting functionality straight into 
// hemp


/*--------------------------------------------------------------------------
 * type definitions
 *--------------------------------------------------------------------------*/

struct hemp_documents_s {
    Hemp       hemp;
    HempHash       documents;
    HempDialect    dialect;

    // TODO: cache
    // TODO: provider path
    // TODO: dialects map
    // TODO: file extension mappings, etc.
};


/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

hemp_documents_p
hemp_documents_init(
    Hemp       hemp
);

void
hemp_documents_free(
    hemp_documents  documents
);


#endif /* HEMP_TEMPLATES_H */