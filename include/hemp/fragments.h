#ifndef HEMP_FRAGMENTS_H
#define HEMP_FRAGMENTS_H

#include <hemp/pool.h>
#include <hemp/document.h>
#include <hemp/fragment.h>


/*--------------------------------------------------------------------------
 * type definitions
 *--------------------------------------------------------------------------*/

struct hemp_fragments {
    HempDocument    document;
    HempPool        pool;
    HempFragment    head;
    HempFragment    tail;
};


/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

HempFragments
hemp_fragments_new(
    HempDocument    document,
    HempSize        capacity
);

HempFragment
hemp_fragments_create(
    HempFragments   fragments,
    HempElement     type,
    HempString      token,
    HempPos         position,
    HempSize        length
);

HempFragment
hemp_fragments_add_fragment(
    HempFragments   fragments,
    HempElement     type,
    HempString      token,
    HempPos         position,
    HempSize        length
);

HempFragment
hemp_fragments_add_eof(
    HempFragments   fragments,
    HempPos         position
);

void 
hemp_fragments_free(
    HempFragments   fragments
);

HempBool
hemp_fragments_free_fragment(
    HempMemory      item
);

HEMP_INLINE HempDocument
hemp_fragments_document(
    HempFragments   fragments
);

void 
hemp_fragments_dump(
    HempFragments   fragments
);


#endif /* HEMP_FRAGMENTS_H */
