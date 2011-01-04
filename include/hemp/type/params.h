#ifndef HEMP_PARAMS_H
#define HEMP_PARAMS_H

#include <hemp/core.h>
#include <hemp/value.h>
#include <hemp/context.h>
#include <hemp/type/list.h>
#include <hemp/type/hash.h>


/*--------------------------------------------------------------------------
 * Type definitions
 *--------------------------------------------------------------------------*/

struct hemp_params {
    HempList        ordinals;
    HempHash        nominals;
}; 


/*--------------------------------------------------------------------------
 * Function prototypes
 *--------------------------------------------------------------------------*/

HempParams
hemp_params_init(
    HempParams      params
);

HEMP_INLINE void
hemp_params_release(
    HempParams      params
);

void
hemp_params_free(
    HempParams      params
);


void 
hemp_params_dump(
    HempParams params
);


/*--------------------------------------------------------------------------
 * Type functions and methods
 *--------------------------------------------------------------------------*/

HEMP_TYPE(hemp_type_params);
HEMP_FETCH(hemp_type_params_fetch);
HEMP_STORE(hemp_type_params_store);


/*--------------------------------------------------------------------------
 * Macros
 *--------------------------------------------------------------------------*/

#define hemp_params_new()                   \
    hemp_params_init(NULL)

#define hemp_params_push(params, value)     \
    hemp_list_push(params->ordinals, value)


#endif /* HEMP_PARAMS_H */
