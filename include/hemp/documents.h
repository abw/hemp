#ifndef HEMP_DOCUMENTS_H
#define HEMP_DOCUMENTS_H

#include <hemp/core.h>
#include <hemp/document.h>


/*--------------------------------------------------------------------------
 * type definitions
 *--------------------------------------------------------------------------*/

struct hemp_documents {
    Hemp            hemp;
    HempDialect     dialect;
};


/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

HempDocuments
hemp_documents_new(
    Hemp            hemp
);

void    
hemp_documents_free(
    HempDocuments   document
);


/*--------------------------------------------------------------------------
 * macros
 *--------------------------------------------------------------------------*/


#endif /* HEMP_DOCUMENTS_H */