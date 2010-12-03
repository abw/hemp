#ifndef HEMP_PARAMS_H
#define HEMP_PARAMS_H

#include <hemp/core.h>
#include <hemp/value.h>
#include <hemp/context.h>
#include <hemp/type/list.h>
#include <hemp/type/hash.h>


/*--------------------------------------------------------------------------
 * type definitions
 *--------------------------------------------------------------------------*/

struct hemp_params {
    hemp_list       ordinals;
    hemp_hash       nominals;
}; 


/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

hemp_params
hemp_params_init(
    hemp_params     params
);

HEMP_INLINE void
hemp_params_release(
    hemp_params params
);

void
hemp_params_free(
    hemp_params     params
);


/*--------------------------------------------------------------------------
 * type functions and methods
 *--------------------------------------------------------------------------*/

HEMP_TYPE_FUNC(hemp_type_params);
HEMP_FETCH_FUNC(hemp_type_params_fetch);
HEMP_STORE_FUNC(hemp_type_params_store);

//HEMP_OUTPUT_FUNC(hemp_type_params_text);
//HEMP_VALUE_FUNC(hemp_method_list_length);
//HEMP_VALUE_FUNC(hemp_method_list_text);

/*--------------------------------------------------------------------------
 * macros
 *--------------------------------------------------------------------------*/

#define hemp_params_new()               \
    hemp_params_init(NULL)

#define hemp_params_push(params, value) \
    hemp_list_push(params->ordinals, value)

#endif /* HEMP_PARAMS_H */
