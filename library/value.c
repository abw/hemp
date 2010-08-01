#include "hemp/value.h"
#include "hemp/debug.h"



/*--------------------------------------------------------------------------
 * vtypes: factory for value types
 *--------------------------------------------------------------------------*/

hemp_vtypes_t
hemp_vtypes_init(
    hemp_cstr_t name
) {
    hemp_vtypes_t vtypes;

    if ((vtypes = (hemp_vtypes_t) hemp_mem_init(sizeof(struct hemp_vtypes)))) {
        vtypes->name   = strdup(name);
        vtypes->types  = hemp_hash_init();
        vtypes->text   = 
        vtypes->hash   = 
        vtypes->list   = NULL;

        if (vtypes->name && vtypes->types) {
            debug_mem(
                "Allocated vtypes '%s' at %p with hash at %p\n", 
                vtypes->name, vtypes, vtypes->types
            );
        }
        else {
            hemp_vtypes_free(vtypes);
            vtypes = NULL;
        }
    }

    // TODO handle vtypes == NULL
    return vtypes;
}


void
hemp_vtypes_free(
    hemp_vtypes_t vtypes
) {
    debug_mem("Releasing vtypes '%s' at %p\n", vtypes->name, vtypes);

    if (vtypes->name) 
        hemp_mem_free(vtypes->name);

    if (vtypes->text)
        hemp_vtype_free(vtypes->text);

    if (vtypes->list)
        hemp_vtype_free(vtypes->list);

    if (vtypes->hash)
        hemp_vtype_free(vtypes->hash);

    if (vtypes->types) {
        hemp_hash_each(vtypes->types, &hemp_vtypes_free_vtype);
        hemp_hash_free(vtypes->types);
    }

    hemp_mem_free(vtypes);
}


void hemp_vtypes_free_vtype(
    hemp_hash_entry_t   entry
) {
    hemp_vtype_free(entry->value);
}



hemp_vtype_t
hemp_vtypes_new_type(
    hemp_vtypes_t   vtypes,
    hemp_cstr_t     name
) {
    hemp_vtype_t vtype;

    if (vtype = (hemp_vtype_t) hemp_hash_fetch(vtypes->types, name)) {
        hemp_fatal("A value type is already defined for %s", name);
    }

    vtype = hemp_vtype_init(vtypes, name);

    if (vtype) {
        hemp_hash_store(vtypes->types, vtype->name, vtype);
    }
    
    return vtype;
}


hemp_vtype_t
hemp_vtypes_type(
    hemp_vtypes_t   vtypes,
    hemp_cstr_t     name
) {
    hemp_vtype_t vtype = (hemp_vtype_t) hemp_hash_fetch(vtypes->types, name);
    
    if (! vtype) {
        hemp_fatal("No value type defined for %s", name);
    }

    return vtype;
}



/*--------------------------------------------------------------------------
 * vtype: value type
 *--------------------------------------------------------------------------*/


hemp_vtype_t
hemp_vtype_init(
    hemp_vtypes_t   vtypes,
    hemp_cstr_t     name
) {
    hemp_vtype_t vtype;

    if ((vtype = (hemp_vtype_t) hemp_mem_init(sizeof(struct hemp_vtype)))) {
        vtype->vtypes = vtypes;
        vtype->name   = strdup(name);

        if (vtype->name) {
            debug_mem(
                "Allocated vtype '%s' at %p\n", 
                vtype->name, vtype
            );
        }
        else {
            hemp_vtype_free(vtype);
            vtype = NULL;
        }
    }

    // TODO handle vtype == NULL
    return vtype;
}


void
hemp_vtype_free(
    hemp_vtype_t vtype
) {
    debug_mem("Releasing vtype '%s' at %p\n", vtype->name, vtype);

    if (vtype->name) {
        hemp_mem_free(vtype->name);
    }

    hemp_mem_free(vtype);
}



/*--------------------------------------------------------------------------
 * value
 *--------------------------------------------------------------------------*/

hemp_value_t
hemp_value_init(
    hemp_vtype_t    vtype,
    hemp_cstr_t     name, 
    hemp_data_t     data,
    hemp_value_t    parent
) {
    hemp_value_t    value;

    if ((value = (hemp_value_t) hemp_mem_init(sizeof(struct hemp_value)))) {
        value->vtype  = vtype;
        value->name   = strdup(name);           // TODO
        value->data   = data;
        value->parent = parent;

        if (value->name) {
            debug_mem(
                "Allocated %s value '%s' at %p\n", 
                vtype->name, name, value
            );
//          if (vtype->init) {
//              vtype->init(value);
//          }
        }
        else {
            hemp_value_free(value);
            value = NULL;
        }
    }

    // TODO handle value == NULL
    return value;
}


void
hemp_value_free(
    hemp_value_t value
) {
    debug_mem("Releasing %s value '%s' at %p\n", value->vtype->name, value->name, value);

//    if (value->vtype->wipe) {
//        value->vtype->wipe(value);
//    }

    if (value->name) {
        hemp_mem_free(value->name);
    }

    hemp_mem_free(value);
}
