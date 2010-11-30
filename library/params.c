#include <hemp/params.h>


hemp_proto
hemp_proto_init(
    hemp_proto params
) {
    HEMP_ALLOCATE(params);

    params->name = hemp_hash_new();
    params->item = hemp_list_new();
    params->list = NULL;
    params->hash = NULL;

    return params;
}


void
hemp_proto_free(
    hemp_proto params
) {
    hemp_list_each(params->item, &hemp_list_each_free);

    if (params->list)
        hemp_mem_free(params->list);

    if (params->hash)
        hemp_mem_free(params->hash);

    hemp_hash_free(params->name);
    hemp_list_free(params->item);
    hemp_mem_free(params);
}


hemp_bool
hemp_proto_add_item(
    hemp_proto   params,
    hemp_string      name
) {
    hemp_debug_call("hemp_proto_add_item(%s)\n", name);

    if (hemp_hash_fetch_string(params->name, name)) {
        hemp_fatal("duplicate parameter: %s", name);
        return HEMP_FALSE;
    }

    hemp_hash_store_string(
        params->name,
        name, 
        "item"
    );
    hemp_list_push(
        params->item,
        hemp_str_val(name)
    );

    return HEMP_TRUE;
}


hemp_bool
hemp_proto_add_list(
    hemp_proto   params,
    hemp_string      name
) {
    hemp_debug_call("hemp_proto_add_list(%s)\n", name);

    if (hemp_hash_fetch_string(params->name, name)) {
        hemp_fatal("duplicate parameter: %s", name);
        return HEMP_FALSE;
    }

    if (params->list) {
        hemp_fatal(
            "Params have already got a list item: %s\n", 
            params->list
        );
        return HEMP_FALSE;
    }

    hemp_hash_store_string(
        params->name,
        name, 
        "list"
    );

    params->list = name;
    return HEMP_TRUE;
}


hemp_bool
hemp_proto_add_hash(
    hemp_proto   params,
    hemp_string      name
) {
    hemp_debug_call("hemp_proto_add_hash(%s)\n", name);

    if (hemp_hash_fetch_string(params->name, name)) {
        hemp_fatal("duplicate parameter: %s", name);
        return HEMP_FALSE;
    }

    if (params->hash) {
        hemp_fatal(
            "Params have already got a hash item: %s\n", 
            params->hash
        );
        return HEMP_FALSE;
    }

    hemp_hash_store_string(
        params->name,
        name, 
        "hash"
    );

    params->hash = name;
    return HEMP_TRUE;
}
