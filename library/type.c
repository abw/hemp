#include <hemp/core.h>
#include <hemp/type.h>
#include <hemp/type/hash.h>
#include <hemp/context.h>


HempType  HempTypeType     = NULL;
HempType  HempTypeValue    = NULL;
HempType  HempTypeReserved = NULL;
HempType  HempTypeUnused   = NULL;
HempType  HempTypeNumber   = NULL;
HempType  HempTypeInteger  = NULL;
HempType  HempTypePointer  = NULL;
HempType  HempTypeString   = NULL;
HempType  HempTypeText     = NULL;
HempType  HempTypeList     = NULL;
HempType  HempTypeHash     = NULL;
HempType  HempTypeCode     = NULL;
HempType  HempTypeParams   = NULL;
HempType  HempTypeObject   = NULL;
HempType  HempTypeElement  = NULL;
HempType  HempTypeIdentity = NULL;
HempType  hemp_global_types[HEMP_TYPES_SIZE];



/*--------------------------------------------------------------------------
 * General purpose functions for managing types
 *--------------------------------------------------------------------------*/

HempType
hemp_type_init(
    HempType    type,
    HempInt     id,
    HempString  name
) {
    HEMP_INSTANCE(type);

    hemp_debug_init("initialising type: %s\n", name);

    type->type      = HempTypeType;   /* allows types to be treated like objects  */
    type->id        = id;
    type->name      = hemp_string_clone(name, "type name");
    type->methods   = hemp_hash_new();
    type->namespace = NULL;
    type->base      = NULL;
    type->clean     = NULL;
    type->value     = &hemp_value_self;
    type->values    = &hemp_value_values;
    type->pairs     = &hemp_value_not_pairs;
    type->params    = &hemp_value_params;
    type->apply     = &hemp_value_apply;
    type->text      = &hemp_value_not_text;
    type->number    = &hemp_value_not_number;
    type->integer   = &hemp_value_not_integer;
    type->boolean   = &hemp_value_not_boolean;
    type->compare   = &hemp_value_not_compare;
    type->defined   = &hemp_value_not_defined;
    type->fetch     = &hemp_value_not_fetch;
    type->store     = &hemp_value_not_store;
    type->dot       = &hemp_value_dot;

    return type;
}


void
hemp_type_isa(
    HempType    type,
    HempType    base
) {
    hemp_debug_call("%s isa %s\n", type->name, base->name);
    type->base = base;
    hemp_hash_attach(type->methods, base->methods);
}


HempType
hemp_type_subtype(
    HempType    base,
    HempInt     id,
    HempString  name
) {
    HempType type = hemp_type_new(id, name);
    hemp_type_isa(type, base);
    return type;
}


void
hemp_type_wipe(
    HempType type
) {
    hemp_debug_call("wiping type: %s\n", type->name);
    hemp_mem_free(type->name);
    hemp_hash_free(type->methods);
}

void
hemp_type_free(
    HempType type
) {
    hemp_type_wipe(type);
    hemp_mem_free(type);
}



/*--------------------------------------------------------------------------
 * Global type initialisation and cleanup
 *--------------------------------------------------------------------------*/

void
hemp_global_types_init(
    HempGlobal   global
) {
    hemp_debug_call("hemp_global_types_init()\n");
    HempInt n;

    /* return silently if it looks like we've already done this step */
    if (HempTypeValue)
        return;

    /* construct the global type objects */
    HempTypeType     = hemp_type_type    ( HEMP_UBER_ID,     HEMP_STR_TYPE     );
    HempTypeValue    = hemp_type_new     ( HEMP_VALUE_ID,    HEMP_STR_VALUE    );
    HempTypeReserved = hemp_type_new     ( HEMP_RESERVED_ID, HEMP_STR_RESERVED );
    HempTypeUnused   = hemp_type_new     ( HEMP_UNUSED_ID,   HEMP_STR_UNUSED   );
    HempTypePointer  = hemp_type_new     ( HEMP_POINTER_ID,  HEMP_STR_POINTER  );
    HempTypeNumber   = hemp_type_number  ( HEMP_NUMBER_ID,   HEMP_STR_NUMBER   );
    HempTypeInteger  = hemp_type_integer ( HEMP_INTEGER_ID,  HEMP_STR_INTEGER  );
    HempTypeString   = hemp_type_string  ( HEMP_STRING_ID,   HEMP_STR_STRING   );
    HempTypeText     = hemp_type_text    ( HEMP_TEXT_ID,     HEMP_STR_TEXT     );
    HempTypeList     = hemp_type_list    ( HEMP_LIST_ID,     HEMP_STR_LIST     );
    HempTypeHash     = hemp_type_hash    ( HEMP_HASH_ID,     HEMP_STR_HASH     );
    HempTypeCode     = hemp_type_code    ( HEMP_CODE_ID,     HEMP_STR_CODE     );
    HempTypeParams   = hemp_type_code    ( HEMP_PARAMS_ID,   HEMP_STR_PARAMS   );
    HempTypeObject   = hemp_type_object  ( HEMP_OBJECT_ID,   HEMP_STR_OBJECT   );
    HempTypeIdentity = hemp_type_identity( HEMP_IDENTITY_ID, HEMP_STR_IDENTITY );

    /* HempTypeElement is something of a special case.  It is a subclass of
     * HempTypeObject and uses the same object->type mechanism to resolve the
     * type from an object pointer.  It doesn't need an entry in the global
     * types table (further below).
     */
    HempTypeElement  = hemp_type_element ( HEMP_OBJECT_ID,   HEMP_STR_ELEMENT  );
    hemp_type_isa(HempTypeElement, HempTypeObject);

    /* add methods to base value types */
    hemp_type_extend(HempTypeValue, "name",    &hemp_method_value_name);
    hemp_type_extend(HempTypeValue, "type",    &hemp_method_value_type);
    hemp_type_extend(HempTypeValue, "text",    &hemp_method_value_text);
    hemp_type_extend(HempTypeValue, "num",     &hemp_method_value_number);
    hemp_type_extend(HempTypeValue, "number",  &hemp_method_value_number);
    hemp_type_extend(HempTypeValue, "int",     &hemp_method_value_integer);
    hemp_type_extend(HempTypeValue, "integer", &hemp_method_value_integer);
    hemp_type_extend(HempTypeValue, "true",    &hemp_method_value_boolean);
    hemp_type_extend(HempTypeValue, "boolean", &hemp_method_value_boolean);
    hemp_type_extend(HempTypeValue, "each",    &hemp_method_value_each);


    /* The first 16 type entries are reserved for hemp's internal use */
    for (n = 0; n < HEMP_TYPES_RESERVED; n++) {
        hemp_global_types[n] = HempTypeReserved;
    }

    /* The second 16 type entries are available for use, e.g. for alien data */
    for (n = HEMP_TYPES_RESERVED; n < HEMP_TYPES_SIZE; n++) {
        hemp_global_types[n] = HempTypeUnused;
    }

    /* Add the builtin types to the type table - we don't add HempValue
     * because it's the uber base type and not directly instantiable.  Nor
     * do we add HempTypeElement because it's essentially a "subclass" of
     * HempTypeObject (for a somewhat loose definition of "subclass").
     */
    hemp_global_types[ HempTypeNumber->id   ] = HempTypeNumber;
    hemp_global_types[ HempTypeInteger->id  ] = HempTypeInteger;
    hemp_global_types[ HempTypeString->id   ] = HempTypeString;
    hemp_global_types[ HempTypePointer->id  ] = HempTypePointer;
    hemp_global_types[ HempTypeText->id     ] = HempTypeText;
    hemp_global_types[ HempTypeList->id     ] = HempTypeList;
    hemp_global_types[ HempTypeHash->id     ] = HempTypeHash;
    hemp_global_types[ HempTypeCode->id     ] = HempTypeCode;
    hemp_global_types[ HempTypeParams->id   ] = HempTypeParams;
    hemp_global_types[ HempTypeObject->id   ] = HempTypeObject;
    hemp_global_types[ HempTypeIdentity->id ] = HempTypeIdentity;
}


void
hemp_global_types_free(
    HempGlobal   global
) {
    hemp_debug_call("hemp_global_types_free()\n");
    HempInt  n;
    HempType t;

    /* clear out all the reserved type up to and including IDENTITY @ 16 */
    for (n = 0; n <= HEMP_TYPES_RESERVED; n++) {
        hemp_global_types[n] = NULL;
    }

    /* anything after that is a user-registered custom type */
    for ( ; n < HEMP_TYPES_SIZE; n++) {
        t = hemp_global_types[n];
        if (t && t != HempTypeUnused) {
            hemp_debug_init("Releasing custom type #%d: %s\n", t->id, t->name);
            hemp_type_free(t);
        }
        hemp_global_types[n] = NULL;
    }

    /* now we can safely free the inbuilt types (HempUnused is used above) */
    hemp_type_free(HempTypeType);       HempTypeType     = NULL;
    hemp_type_free(HempTypeValue);      HempTypeValue    = NULL;
    hemp_type_free(HempTypeReserved);   HempTypeReserved = NULL;
    hemp_type_free(HempTypeUnused);     HempTypeUnused   = NULL;
    hemp_type_free(HempTypePointer);    HempTypePointer  = NULL;
    hemp_type_free(HempTypeNumber);     HempTypeNumber   = NULL;
    hemp_type_free(HempTypeInteger);    HempTypeInteger  = NULL;
    hemp_type_free(HempTypeString);     HempTypeString   = NULL;
    hemp_type_free(HempTypeText);       HempTypeText     = NULL;
    hemp_type_free(HempTypeList);       HempTypeList     = NULL;
    hemp_type_free(HempTypeHash);       HempTypeHash     = NULL;
    hemp_type_free(HempTypeCode);       HempTypeCode     = NULL;
    hemp_type_free(HempTypeParams);     HempTypeParams   = NULL;
    hemp_type_free(HempTypeObject);     HempTypeObject   = NULL;
    hemp_type_free(HempTypeElement);    HempTypeElement  = NULL;
    hemp_type_free(HempTypeIdentity);   HempTypeIdentity = NULL;
}


HempType
hemp_use_type(
    HempString  name,
    HempTypedef constructor
) {
    HempInt  n;
    HempInt  empty = 0;

    for (n = HEMP_TYPES_RESERVED + 1; n < HEMP_TYPES_SIZE; n++) {
        /* look for non-empty slot matching name */
        if (hemp_global_types[n] == HempTypeUnused) {
            if (! empty)
                empty = n;
        }
        else if (hemp_string_eq(name, hemp_global_types[n]->name)) {
            return hemp_global_types[n];
        }
    }

    if (empty) {
        /* construct the type and install it in the first empty slot */
        return (hemp_global_types[empty] = constructor(empty, name));
    }

    hemp_fatal(
        "Failed to register %s type (global type registry is full)",
        name
    );

    return NULL;
}


/*--------------------------------------------------------------------------
 * Base type definitions
 *--------------------------------------------------------------------------*/

HEMP_TYPE(hemp_type_type) {
    HempType type = hemp_type_new(id, name);

    /* add text method to display the type name, e.g. foo.type */
    type->text = &hemp_method_type_text;

    /* add methods to lookup the type name or id, e.g. foo.type.name  */
    hemp_type_extend(type, "name", &hemp_method_type_name);
    hemp_type_extend(type, "id",   &hemp_method_type_id);

    return type;
};


HEMP_OUTPUT(hemp_method_type_text) {
    HempType type = (HempType) hemp_val_ptr(value);
    HempText text;

//    hemp_prepare_text_size(context, output, text, strlen(type->name));

    if (hemp_is_undef(output)) {
        text   = hemp_context_tmp_text_size(context, strlen(type->name));
        output = hemp_text_val(text);
    }
    else {
        text   = hemp_val_text(output);
    }

    hemp_text_append_string(text, type->name);
    return output;
}


HEMP_VALUE(hemp_method_type_name) {
    return hemp_str_val( ((HempType) hemp_val_ptr(value))->name );
}


HEMP_VALUE(hemp_method_type_id) {
    return hemp_int_val( ((HempType) hemp_val_ptr(value))->id );
}


HEMP_VALUE(hemp_method_value_name) {
    return hemp_str_val( hemp_vtype(value)->name );
}


HEMP_VALUE(hemp_method_value_type) {
    return hemp_obj_val( (HempObject) hemp_vtype(value) );
}


HEMP_VALUE(hemp_method_value_text) {
    return hemp_to_text(value, context);
}


HEMP_VALUE(hemp_method_value_number) {
    return hemp_to_number(value, context);
}


HEMP_VALUE(hemp_method_value_integer) {
    return hemp_to_integer(value, context);
}


HEMP_VALUE(hemp_method_value_boolean) {
    return hemp_to_boolean(value, context);
}


HEMP_VALUE(hemp_method_value_defined) {
    return hemp_call(value, defined, context);
}


HEMP_VALUE(hemp_method_value_each) {
    HempList    results = hemp_context_tmp_list(context);
    HempParams  params  = context->frame->params;
    HempValue   result;
    HempValue   callback;

    hemp_debug_call("hemp_method_value_each()\n");

    if (! params)
        hemp_fatal("No params for each (TODO)\n");

//  hemp_params_dump(params);

    if (params->ordinals->length < 1)
        hemp_fatal("No callback for each (TODO)\n");

    callback = hemp_list_item(params->ordinals, 0);

    /* This is horribly wasteful compared to more efficient approaches that
     * are used in "traditional" programming language (e.g. a single caller
     * stack onto which frames/arguments are pushed.  On the other hand, I
     * would think nothing of creating a hash/list on the fly in Perl, say,
     * to capture the arguments passed to a subroutine/method, so why should
     * I be so cautious in C?  In the end, this works today and can be made
     * better tomorrow.
     */
    HempFrame frame = hemp_context_enter(context, NULL);
    hemp_params_push(frame->params, value);

    hemp_debug_call("calling %s->apply with value: %s\n", hemp_type_name(callback), hemp_type_name(value));

    result = hemp_call(callback, apply, context);
    hemp_list_push(results, result);

    hemp_context_leave(context);

    return hemp_list_val(results);
}
