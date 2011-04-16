#ifndef HEMP_SCHEME_H
#define HEMP_SCHEME_H

#include <hemp/core.h>
#include <hemp/factory.h>
#include <hemp/filesystem.h>
#include <hemp/module.h>
#include <hemp/source.h>
#include <hemp/type/hash.h>


/*--------------------------------------------------------------------------
 * typedefs: functions operating on HempSource data
 *--------------------------------------------------------------------------*/

typedef void        (* hemp_source_vop)(HempSource);
typedef HempBool    (* hemp_source_bop)(HempSource);
typedef HempString  (* hemp_source_sop)(HempSource);


/*--------------------------------------------------------------------------
 * type definitions
 *--------------------------------------------------------------------------*/

struct hemp_scheme {
    Hemp            hemp;
    HempString      name;
    hemp_source_sop namer;
    hemp_source_bop checker;
    hemp_source_sop reader;
    hemp_source_vop cleaner;
};


/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

HempScheme
hemp_scheme_new(
    Hemp            hemp,
    HempString      type
);

void
hemp_scheme_free(
    HempScheme      scheme
);

HempString 
hemp_scheme_namer( 
    HempSource      source
);

//HEMP_AUTOLOAD(hemp_scheme_autoload);


#endif /* HEMP_SCHEME_H */