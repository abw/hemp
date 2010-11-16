#ifndef HEMP_PARAMS_H
#define HEMP_PARAMS_H

#include <hemp/core.h>
#include <hemp/type/list.h>
#include <hemp/type/hash.h>


/*--------------------------------------------------------------------------
 * data structures
 *--------------------------------------------------------------------------*/

struct hemp_params_s {
    hemp_hash_p     name;
    hemp_list_p     item;
    hemp_str_p      list;
    hemp_str_p      hash;
};


/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

hemp_params_p
    hemp_params_init(
        hemp_params_p
    );

void
    hemp_params_free(
        hemp_params_p
    );

hemp_bool_t
    hemp_params_add_item(
        hemp_params_p   params,
        hemp_str_p      name
    );

hemp_bool_t
    hemp_params_add_list(
        hemp_params_p   params,
        hemp_str_p      name
    );

hemp_bool_t
    hemp_params_add_hash(
        hemp_params_p   params,
        hemp_str_p      name
    );


/*--------------------------------------------------------------------------
 * macros
 *--------------------------------------------------------------------------*/

#define hemp_params_new()                       \
    hemp_params_init(NULL)

#define hemp_params_allocate(name)              \
    hemp_allocate_type_name(params, name)

#define hemp_params_item(params, n)             \
    hemp_hash_fetch_string(params->name, n)



#endif /* HEMP_PARAMS_H */