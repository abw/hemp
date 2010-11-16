#include <hemp/params.h>


hemp_params_p
hemp_params_init(
    hemp_params_p params
) {
    hemp_params_allocate(params);

    params->names     = hemp_hash_new();
    params->items     = hemp_list_new();
    params->list_item = HempMissing;
    params->hash_item = HempMissing;

    return params;
}


void
hemp_params_free(
    hemp_params_p params
) {
    hemp_hash_free(params->names);
    hemp_list_free(params->items);
    hemp_mem_free(params);
}

