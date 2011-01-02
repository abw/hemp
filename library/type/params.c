#include <hemp/type/params.h>


HEMP_TYPE_FUNC(hemp_type_params) {
    HempType type   = hemp_type_subtype(HempTypeValue, id, name);
    type->fetch     = &hemp_type_params_fetch;
    type->store     = &hemp_type_params_store;
    type->defined   = &hemp_value_true;
    return type;
};


/*--------------------------------------------------------------------------
 * General purpose list functions
 *--------------------------------------------------------------------------*/

HEMP_INLINE HempParams
hemp_params_init(
    HempParams params
) {
    HEMP_INSTANCE(params);
    params->ordinals = hemp_list_new();
    params->nominals = hemp_hash_new();
    return params;
}


HEMP_INLINE void
hemp_params_release(
    HempParams params
) {
    hemp_list_free(params->ordinals);
    hemp_hash_free(params->nominals);
    params->ordinals = NULL;
    params->nominals = NULL;
}


void
hemp_params_free(
    HempParams params
) {
    hemp_params_release(params);
    hemp_mem_free(params);
}


/*--------------------------------------------------------------------------
 * runtime evaluation functions
 *--------------------------------------------------------------------------*/

HEMP_FETCH_FUNC(hemp_type_params_fetch) {
    hemp_debug_msg("hemp_type_params_fetch()\n");

    HempInt    index;
    HempParams params  = hemp_val_params(container);
    HempBool   found   = hemp_list_index(context, key, &index);

    if (found) {
        return hemp_type_list_fetch(
            hemp_list_val(params->ordinals), context, key
        );
    }
    else {
        return hemp_type_hash_fetch(
            hemp_hash_val(params->nominals), context, key
        );
    }
}



HEMP_STORE_FUNC(hemp_type_params_store) {
    hemp_debug_msg("hemp_type_params_store()\n");

    HempInt    index;
    HempParams params  = hemp_val_params(container);
    HempBool   found   = hemp_list_index(context, key, &index);

    if (found) {
        return hemp_type_list_store(
            hemp_list_val(params->ordinals), context, key, value
        );
    }
    else {
        return hemp_type_hash_store(
            hemp_hash_val(params->nominals), context, key, value
        );
    }
}


void 
hemp_params_dump(
    HempParams params
) {
    hemp_debug_msg("hemp params at %p:\n", params);
    if (params->ordinals) {
        hemp_debug("%d ordinals:\n", params->ordinals->length);
        HempText text = hemp_list_dump(params->ordinals);
        hemp_debug(text->string);
        hemp_text_free(text);
    }
    else {
        hemp_debug("no ordinals\n");
    }
    if (params->nominals) {
        hemp_debug("%d nominals:\n", params->nominals->size);
        HempText text = hemp_hash_dump(params->nominals);
        hemp_debug(text->string);
        hemp_text_free(text);
    }
    else {
        hemp_debug("no nominals\n");
    }
}


/*--------------------------------------------------------------------------
 * Params pseudo-object methods
 *--------------------------------------------------------------------------*/

//HEMP_VALUE(hemp_method_list_length) {
//    return hemp_int_val( hemp_val_list(value)->length );
//}
//
//
//HEMP_VALUE(hemp_method_list_text) {
//    return hemp_type_list_text(value, context, HempNothing);
//}
//

