#include <hemp/value.h>
#include <hemp/context.h>
#include <hemp/type/code.h>

HEMP_OUTPUT(hemp_type_code_text);
HEMP_INPUT(hemp_type_code_apply);

HEMP_TYPE_FUNC(hemp_type_code) {
    hemp_type type  = hemp_type_subtype(HempValue, id, name);
    type->text      = &hemp_type_code_text; 
    type->apply     = &hemp_type_code_apply;
    type->value     = &hemp_value_self;
    type->values    = &hemp_value_values;
    type->params    = &hemp_value_params;
    type->defined   = &hemp_value_true;
    type->dot       = &hemp_value_dot;

    return type;

//    hemp_type_extend(type, "length", &hemp_method_text_length);

    return type;
};



/*--------------------------------------------------------------------------
 * constructor/destructor functions.
 *--------------------------------------------------------------------------*/

HEMP_INLINE hemp_code
hemp_code_init(
    hemp_code code
) {
    hemp_debug_call("hemp_code_init()\n");
    HEMP_INSTANCE(code);
    code->body  = HempNothing;
    code->proto = NULL;
//    code->args  = HempNothing;
    return code;
}


HEMP_INLINE void
hemp_code_release(
    hemp_code code
) {
    hemp_debug_call("hemp_code_release()\n");
    if (code->proto)
        hemp_proto_free(code->proto);
}


HEMP_INLINE void
hemp_code_free(
    hemp_code code
) {
    hemp_debug_call("hemp_code_free(%p)\n", code);
    hemp_code_release(code);
    hemp_mem_free(code);
}


HEMP_INLINE hemp_proto
hemp_code_proto(
    hemp_code code
) {
    if (! code->proto)
        code->proto = hemp_proto_new();

    return code->proto;
}


/*--------------------------------------------------------------------------
 * runtime evaluation methods
 *--------------------------------------------------------------------------*/

void 
hemp_type_code_resolve_proto(
    hemp_code       code,
    hemp_context    context,
    hemp_params     params
) {
    if (! code->proto)
        return;

    if (! params)
        hemp_fatal("no parameters supplied to code with prototype"); // TODO


    hemp_proto  proto = code->proto;
    hemp_list   items = proto->item;
    hemp_size   ord   = 0;
    hemp_size   nom   = 0;
    hemp_value  item, value;
    hemp_string name;
    hemp_size   n;
    
//  hemp_debug("proto:\n");
//  hemp_proto_dump(proto);
    
    for (n = 0; n < items->length; n++) {
        item    = hemp_list_item(items, n);
        name    = hemp_val_str(item);
        value   = hemp_hash_fetch(params->nominals, name);
        
        if (hemp_is_found(value)) {
            nom++;
//          hemp_debug_msg("found nominal value for %s: %s\n", name, hemp_type_name(value));
        }
        else if (ord < params->ordinals->length) {
            value = hemp_list_item(params->ordinals, ord++);
//          value = hemp_list_shift(params->ordinals);
//          hemp_debug_msg("found ordinal value for %s: %s\n", name, hemp_type_name(value));
        }
        else {
            hemp_fatal("No value specified for %s parameter", name);
        }
        hemp_hash_store(context->vars, name, value);
    }
    
    if (ord < params->ordinals->length) {
        if (proto->list) {
            hemp_todo("store remaining ordinal params in %s\n", proto->list);
        }
        else {
            hemp_size x = params->ordinals->length - ord;
            hemp_fatal("%d extra ordinal parameter%s provided", x, x == 1 ? "" : "s");
        }
    }
    
    if (nom < params->nominals->size) {
        if (proto->hash) {
            hemp_todo("store remaining nominal params in %s\n", proto->hash);
        }
        else {
            hemp_size x = params->nominals->size - nom;
            hemp_fatal("%d extra nominal parameter%s provided", x, x == 1 ? "" : "s");
        }
    }

}


HEMP_INPUT(hemp_type_code_apply) {
    hemp_debug_call("hemp_type_code_apply()\n");
    hemp_code   code    = hemp_val_code(value);
    hemp_value  body    = code->body;

    /* We may have been passed params as the input value, otherwise we use
     * the params for the current context frame - but be warned that this 
     * could also be NULL
     */
    if (code->proto) {
        hemp_type_code_resolve_proto(
            code, context, 
            hemp_is_defined(input)
                ? hemp_val_params(input)
                : context->frame->params
        );
    }

    hemp_debug_call("evaluating code body: %s\n", hemp_type_name(body));
    return hemp_call(body, value, context);
}


HEMP_OUTPUT(hemp_type_code_text) {
    hemp_debug_call("hemp_type_code_text()\n");
    hemp_code   code    = hemp_val_code(value);
    hemp_value  body    = code->body;

    if (code->proto) {
        hemp_type_code_resolve_proto(
            code, context, NULL
        );
    }

//  hemp_debug_msg("evaluating code body as text: %s\n", hemp_type_name(body));
    hemp_call(body, text, context, output);
    return output;
}

