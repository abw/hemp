#ifndef HEMP_PARAMS_H
#define HEMP_PARAMS_H

#include <hemp/core.h>
#include <hemp/type/list.h>
#include <hemp/type/hash.h>


/*--------------------------------------------------------------------------
 * type definitions
 *--------------------------------------------------------------------------*/

struct hemp_params {
    hemp_hash       name;
    hemp_list       item;
    hemp_string     list;
    hemp_string     hash;
};


/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

hemp_params
hemp_params_init(
    hemp_params     params
);

void
hemp_params_free(
    hemp_params     params
);

hemp_bool
hemp_params_add_item(
    hemp_params     params,
    hemp_string     name
);

hemp_bool
hemp_params_add_list(
    hemp_params     params,
    hemp_string     name
);

hemp_bool
hemp_params_add_hash(
    hemp_params     params,
    hemp_string     name
);


/*--------------------------------------------------------------------------
 * macros
 *--------------------------------------------------------------------------*/

#define hemp_params_new()                       \
    hemp_params_init(NULL)

#define hemp_params_item(params, n)             \
    hemp_hash_fetch_string(params->name, n)



#endif /* HEMP_PARAMS_H */