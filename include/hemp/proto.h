#ifndef HEMP_PROTO_H
#define HEMP_PROTO_H

#include <hemp/core.h>
#include <hemp/type/list.h>
#include <hemp/type/hash.h>


/*--------------------------------------------------------------------------
 * type definitions
 *--------------------------------------------------------------------------*/

struct hemp_proto {
    HempHash        name;
    HempList        item;
    HempString      list;
    HempString      hash;
};


/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

HempProto
hemp_proto_init(
    HempProto       params
);

void
hemp_proto_free(
    HempProto       params
);

HempBool
hemp_proto_add_item(
    HempProto       params,
    HempString      name
);

HempBool
hemp_proto_add_list(
    HempProto       params,
    HempString      name
);

HempBool
hemp_proto_add_hash(
    HempProto       params,
    HempString      name
);

void 
hemp_proto_dump(
    HempProto       proto
);


/*--------------------------------------------------------------------------
 * macros
 *--------------------------------------------------------------------------*/

#define hemp_proto_new()                       \
    hemp_proto_init(NULL)

#define hemp_proto_item(params, n)             \
    hemp_hash_fetch_string(params->name, n)



#endif /* HEMP_PROTO_H */