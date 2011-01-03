#ifndef HEMP_DIALECT_H
#define HEMP_DIALECT_H

#include <hemp/tagset.h>
#include <hemp/document.h>


/*--------------------------------------------------------------------------
 * type definitions
 *--------------------------------------------------------------------------*/

typedef HempDialect
(*  hemp_dialect_f) (
    Hemp            hemp,
    HempString      name
);

struct hemp_dialect {
    Hemp            hemp;
    HempString      name;
    hemp_doc_prep   prepare;
    hemp_doc_scan   scanner;
    hemp_doc_clean  cleanup;
//  HempHash        tags;
//  hemp_tagset_t   tagset;
};

struct hemp_dialects {
    HempString      name;
    hemp_dialect_f  ctor;
};


/*--------------------------------------------------------------------------
 * Factory prototypes
 *--------------------------------------------------------------------------*/

HEMP_FACTORY(hemp_dialect_factory);
HEMP_HASH_ITERATOR(hemp_dialect_cleaner);


/*--------------------------------------------------------------------------
 * function prototypes for generic dialects
 *--------------------------------------------------------------------------*/

HempDialect
hemp_dialect_new(
    Hemp            hemp,
    HempString      name
);

void
hemp_dialect_free(
    HempDialect     dialect
);

HempBool
hemp_dialect_add_tag(
    HempString      name,
    HempTag         tag
);

HempDocument
hemp_dialect_document(
    HempDialect     dialect,
    HempSource      source
);


#endif /* HEMP_DIALECT_H */