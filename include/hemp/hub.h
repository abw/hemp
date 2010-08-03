#ifndef HEMP_HUB_H
#define HEMP_HUB_H

#include "hemp/defaults.h"
#include "hemp/types.h"
#include "hemp/hash.h"
#include "hemp/tags.h"
#include "hemp/value.h"


/*--------------------------------------------------------------------------
 * data structures
 *--------------------------------------------------------------------------*/

struct hemp_hub {
    hemp_hash_t tags;
    hemp_hash_t elements;
    hemp_hash_t vtypes;

};


/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

hemp_hub_t  hemp_hub_init();
void        hemp_hub_free(hemp_hub_t);
void        hemp_hub_free_vtypes(hemp_hash_entry_t);
void        hemp_hub_prepare_vtypes_hemp(hemp_hub_t);


/*--------------------------------------------------------------------------
 * macros
 *--------------------------------------------------------------------------*/

#define hemp_hub_null(d) \
    hemp_hub_free(d);    \
    d = NULL;

#define hemp_hub_tag(hub, name) \
    (hemp_tag_t) hemp_hash_fetch(hub->tags, name)

#define hemp_hub_add_tag(hub, tag) \
    hemp_hash_store(hub->tags, (hemp_cstr_t) tag->name, (hemp_ptr_t) tag)

#define hemp_hub_vtypes(hub, name) \
    (hemp_vtypes_t) hemp_hash_fetch(hub->vtypes, name)

#define hemp_hub_add_vtypes(hub, vtypes) \
    hemp_hash_store(hub->vtypes, vtypes->name, (hemp_ptr_t) vtypes)

#define hemp_hub_element(hub, name) \
    (hemp_etype_t) hemp_hash_fetch(hub->elements, name)

#define hemp_hub_add_element(hub, element) \
    hemp_hash_store(hub->elements, (hemp_cstr_t) element->name, (hemp_ptr_t) element)


#endif /* HEMP_HUB_H */