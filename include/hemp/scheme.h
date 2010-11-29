#ifndef HEMP_SCHEME_H
#define HEMP_SCHEME_H

#include <hemp/core.h>
#include <hemp/source.h>
#include <hemp/type/hash.h>


/*--------------------------------------------------------------------------
 * typedefs: functions operating on hemp_source data
 *--------------------------------------------------------------------------*/

typedef void        (* hemp_source_vop)(hemp_source);
typedef hemp_bool   (* hemp_source_bop)(hemp_source);
typedef hemp_string (* hemp_source_sop)(hemp_source);


/*--------------------------------------------------------------------------
 * data structures
 *--------------------------------------------------------------------------*/

struct hemp_scheme {
    hemp_hemp           hemp;
    hemp_string         name;
    hemp_source_sop     namer;
    hemp_source_bop     checker;
    hemp_source_sop     reader;
    hemp_source_vop     cleaner;
};


/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

hemp_scheme
hemp_scheme_new(
    hemp_hemp           hemp,
    hemp_string         type
);

void
hemp_scheme_free(
    hemp_scheme         scheme
);

hemp_string 
hemp_scheme_namer( 
    hemp_source         source
);


#endif /* HEMP_SCHEME_H */