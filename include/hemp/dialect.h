#ifndef HEMP_DIALECT_H
#define HEMP_DIALECT_H

#include <hemp/tagset.h>
#include <hemp/document.h>


/*--------------------------------------------------------------------------
 * type definitions
 *--------------------------------------------------------------------------*/

typedef hemp_dialect
(*  hemp_dialect_f) (
    hemp_hemp       hemp,
    hemp_string     name
);

struct hemp_dialect {
    hemp_hemp       hemp;
    hemp_string     name;
    hemp_hash       tags;
    hemp_prep_f     prepare;
//  hemp_scan_f     scanner;
    hemp_clean_f    cleanup;
//  hemp_tagset_t   tagset;
};

struct hemp_dialects {
    hemp_string     name;
    hemp_dialect_f  ctor;
};


/*--------------------------------------------------------------------------
 * function prototypes for generic dialects
 *--------------------------------------------------------------------------*/

hemp_dialect
hemp_dialect_new(
    hemp_hemp       hemp,
    hemp_string     name
);

void
hemp_dialect_free(
    hemp_dialect    dialect
);

hemp_bool
hemp_dialect_add_tag(
    hemp_string     name,
    hemp_tag        tag
);

hemp_document
hemp_dialect_document(
    hemp_dialect    dialect,
    hemp_source     source
);


#endif /* HEMP_DIALECT_H */