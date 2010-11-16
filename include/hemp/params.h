#ifndef HEMP_PARAMS_H
#define HEMP_PARAMS_H

#include <hemp/core.h>
#include <hemp/type/list.h>
#include <hemp/type/hash.h>


/*--------------------------------------------------------------------------
 * data structures
 *--------------------------------------------------------------------------*/

struct hemp_params_s {
    hemp_hash_p     names;
    hemp_list_p     items;
    hemp_value_t    list_item;
    hemp_value_t    hash_item;
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


/*--------------------------------------------------------------------------
 * macros
 *--------------------------------------------------------------------------*/

#define hemp_params_new()                   \
    hemp_params_init(NULL)

#define hemp_params_allocate(name)          \
    hemp_allocate_type_name(params, name)

#endif /* HEMP_PARAMS_H */