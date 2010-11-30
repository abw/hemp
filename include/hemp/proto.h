#ifndef HEMP_PARAMS_H
#define HEMP_PARAMS_H

#include <hemp/core.h>
#include <hemp/type/list.h>
#include <hemp/type/hash.h>


/*--------------------------------------------------------------------------
 * type definitions
 *--------------------------------------------------------------------------*/

struct hemp_proto {
    hemp_hash       name;
    hemp_list       item;
    hemp_string     list;
    hemp_string     hash;
};


/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

hemp_proto
hemp_proto_init(
    hemp_proto     params
);

void
hemp_proto_free(
    hemp_proto     params
);

hemp_bool
hemp_proto_add_item(
    hemp_proto     params,
    hemp_string     name
);

hemp_bool
hemp_proto_add_list(
    hemp_proto     params,
    hemp_string     name
);

hemp_bool
hemp_proto_add_hash(
    hemp_proto     params,
    hemp_string     name
);


/*--------------------------------------------------------------------------
 * macros
 *--------------------------------------------------------------------------*/

#define hemp_proto_new()                       \
    hemp_proto_init(NULL)

#define hemp_proto_item(params, n)             \
    hemp_hash_fetch_string(params->name, n)



#endif /* HEMP_PARAMS_H */