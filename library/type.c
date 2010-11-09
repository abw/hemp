#include <hemp/type.h>
#include <hemp/type/hash.h>


hemp_type_p  HempType     = NULL;
hemp_type_p  HempValue    = NULL;
hemp_type_p  HempReserved = NULL;
hemp_type_p  HempUnused   = NULL;
hemp_type_p  HempNumber   = NULL;
hemp_type_p  HempInteger  = NULL;
hemp_type_p  HempString   = NULL;
hemp_type_p  HempText     = NULL;
hemp_type_p  HempList     = NULL;
hemp_type_p  HempHash     = NULL;
hemp_type_p  HempObject   = NULL;
hemp_type_p  HempIdentity = NULL;
hemp_type_p  hemp_global_types[HEMP_TYPES_SIZE];



/*--------------------------------------------------------------------------
 * General purpose functions for managing types
 *--------------------------------------------------------------------------*/

hemp_type_p 
hemp_type_init(
    hemp_int_t  id,
    hemp_str_p  name
) {
    hemp_type_p type = (hemp_type_p) hemp_mem_alloc(
        sizeof(struct hemp_type_s)
    );

    if (! type)
        hemp_mem_fail("type");

    type->type    = HempType;   /* allows types to be treated like objects  */
    type->id      = id;
    type->name    = hemp_string_clone(name, "type name");
    type->base    = NULL;
    type->methods = hemp_hash_init();
    type->value   = &hemp_value_self;
    type->apply   = &hemp_value_self;
    type->text    = &hemp_value_not_text; 
    type->number  = &hemp_value_not_number;
    type->integer = &hemp_value_not_integer;
    type->boolean = &hemp_value_not_boolean;
    type->compare = &hemp_value_not_compare;
    type->defined = &hemp_value_not_defined;
    type->fetch   = &hemp_value_not_fetch;
    type->store   = &hemp_value_not_store;
    type->dot     = &hemp_value_dot;

    return type;
}


hemp_type_p 
hemp_type_subtype(
    hemp_type_p base,
    hemp_int_t  id,
    hemp_str_p  name
) {
    hemp_type_p type = hemp_type_init(id, name);
    type->base = base;
    hemp_hash_attach(type->methods, base->methods);
    return type;
}


void 
hemp_type_free(
    hemp_type_p type
) {
    hemp_mem_free(type->name);
    hemp_hash_free(type->methods);
    hemp_mem_free(type);
}


/*--------------------------------------------------------------------------
 * Global type initialisation and cleanup
 *--------------------------------------------------------------------------*/

void
hemp_global_types_init() {
    hemp_debug_call("hemp_global_types_init()\n");
    hemp_int_t n;

    /* return silently if it looks like we've already done this step */
    if (HempValue)
        return;

    /* construct the global type objects */
    HempType     = hemp_type_type    ( HEMP_UBER_ID,     HEMP_STR_TYPE     );
    HempValue    = hemp_type_init    ( HEMP_VALUE_ID,    HEMP_STR_VALUE    );
    HempReserved = hemp_type_init    ( HEMP_RESERVED_ID, HEMP_STR_RESERVED );
    HempUnused   = hemp_type_init    ( HEMP_UNUSED_ID,   HEMP_STR_UNUSED   );
    HempNumber   = hemp_type_number  ( HEMP_NUMBER_ID,   HEMP_STR_NUMBER   );
    HempInteger  = hemp_type_integer ( HEMP_INTEGER_ID,  HEMP_STR_INTEGER  );
    HempString   = hemp_type_string  ( HEMP_STRING_ID,   HEMP_STR_STRING   );
    HempText     = hemp_type_text    ( HEMP_TEXT_ID,     HEMP_STR_TEXT     );
    HempList     = hemp_type_list    ( HEMP_LIST_ID,     HEMP_STR_LIST     );
    HempHash     = hemp_type_hash    ( HEMP_HASH_ID,     HEMP_STR_HASH     );
    HempObject   = hemp_type_object  ( HEMP_OBJECT_ID,   HEMP_STR_OBJECT   );
    HempIdentity = hemp_type_identity( HEMP_IDENTITY_ID, HEMP_STR_IDENTITY );

    /* add methods to base value types */
    hemp_type_extend(HempValue, "name",    &hemp_method_value_name);
    hemp_type_extend(HempValue, "type",    &hemp_method_value_type);
    hemp_type_extend(HempValue, "text",    &hemp_method_value_text);
    hemp_type_extend(HempValue, "num",     &hemp_method_value_number);
    hemp_type_extend(HempValue, "number",  &hemp_method_value_number);
    hemp_type_extend(HempValue, "int",     &hemp_method_value_integer);
    hemp_type_extend(HempValue, "integer", &hemp_method_value_integer);
    hemp_type_extend(HempValue, "true",    &hemp_method_value_boolean);
    hemp_type_extend(HempValue, "boolean", &hemp_method_value_boolean);


    /* The first 16 type entries are reserved for hemp's internal use */
    for (n = 0; n < HEMP_TYPES_RESERVED; n++) {
        hemp_global_types[n] = HempReserved;
    }

    /* The second 16 type entries are available for use, e.g. for alien data */
    for (n = HEMP_TYPES_RESERVED; n < HEMP_TYPES_SIZE; n++) {
        hemp_global_types[n] = HempUnused;
    }

    /* add the builtin types to the type table - we don't add HempValue 
     * because it's the uber base type and not directly instantiable 
     */
    hemp_global_types[ HempNumber->id   ] = HempNumber;
    hemp_global_types[ HempInteger->id  ] = HempInteger;
    hemp_global_types[ HempString->id   ] = HempString;
    hemp_global_types[ HempText->id     ] = HempText;
    hemp_global_types[ HempList->id     ] = HempList;
    hemp_global_types[ HempHash->id     ] = HempHash;
    hemp_global_types[ HempObject->id   ] = HempObject;
    hemp_global_types[ HempIdentity->id ] = HempIdentity;
}


void
hemp_global_types_free() {
    hemp_debug_call("hemp_global_types_free()\n");
    hemp_int_t  n;
    hemp_type_p t;

    /* clear out all the reserved type up to and including IDENTITY @ 16 */
    for (n = 0; n <= HEMP_TYPES_RESERVED; n++) {
        hemp_global_types[n] = NULL;
    }

    /* anything after that is a user-registered custom type */
    for ( ; n < HEMP_TYPES_SIZE; n++) {
        t = hemp_global_types[n];
        if (t && t != HempUnused) {
            hemp_debug_init("Releasing custom type #%d: %s\n", t->id, t->name);
            hemp_type_free(t);
        }
        hemp_global_types[n] = NULL;
    }

    /* now we can safely free the inbuilt types (HempUnused is used above) */
    hemp_type_free(HempType);       HempType     = NULL;
    hemp_type_free(HempValue);      HempValue    = NULL;
    hemp_type_free(HempReserved);   HempReserved = NULL;
    hemp_type_free(HempUnused);     HempUnused   = NULL;
    hemp_type_free(HempNumber);     HempNumber   = NULL;
    hemp_type_free(HempInteger);    HempInteger  = NULL;
    hemp_type_free(HempString);     HempString   = NULL;
    hemp_type_free(HempText);       HempText     = NULL;
    hemp_type_free(HempList);       HempList     = NULL;
    hemp_type_free(HempHash);       HempHash     = NULL;
    hemp_type_free(HempObject);     HempObject   = NULL;
    hemp_type_free(HempIdentity);   HempIdentity = NULL;
}


hemp_type_p
hemp_use_type(
    hemp_str_p  name,
    hemp_type_f constructor
) {
    hemp_int_t  n;
    hemp_int_t  empty = 0;
    hemp_type_p type;
    
    for (n = HEMP_TYPES_RESERVED + 1; n < HEMP_TYPES_SIZE; n++) {
        /* look for non-empty slot matching name */
        if (hemp_global_types[n] == HempUnused) {
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
        type->name
    );
}


/*--------------------------------------------------------------------------
 * Base type definitions
 *--------------------------------------------------------------------------*/

HEMP_TYPE_FUNC(hemp_type_type) {
    hemp_type_p type = hemp_type_init(id, name);

    /* add text method to display the type name, e.g. foo.type */
    type->text = &hemp_value_type_text;

    /* add methods to lookup the type name or id, e.g. foo.type.name  */
    hemp_type_extend(type, "name", &hemp_method_type_name);
    hemp_type_extend(type, "id",   &hemp_method_type_id);

    return type;
};


HEMP_VTEXT_FUNC(hemp_value_type_text) {
    hemp_type_p type = (hemp_type_p) hemp_val_ptr(value);
    hemp_text_p text;
    hemp_prepare_output(output, text, strlen(type->name));
    hemp_text_append_string(text, type->name);
    return output;
}


HEMP_VALUE_FUNC(hemp_method_type_name) {
    return hemp_str_val( ((hemp_type_p) hemp_val_ptr(value))->name );
}


HEMP_VALUE_FUNC(hemp_method_type_id) {
    return hemp_int_val( ((hemp_type_p) hemp_val_ptr(value))->id );
}


HEMP_VALUE_FUNC(hemp_method_value_name) {
    return hemp_str_val( hemp_type(value)->name );
}


HEMP_VALUE_FUNC(hemp_method_value_type) {
    return hemp_obj_val( (hemp_object_p) hemp_type(value) );
}


HEMP_VALUE_FUNC(hemp_method_value_text) {
    return hemp_to_text(value, context);
}


HEMP_VALUE_FUNC(hemp_method_value_number) {
    return hemp_to_number(value, context);
}


HEMP_VALUE_FUNC(hemp_method_value_integer) {
    return hemp_to_integer(value, context);
}


HEMP_VALUE_FUNC(hemp_method_value_boolean) {
    return hemp_to_boolean(value, context);
}


HEMP_VALUE_FUNC(hemp_method_value_defined) {
    return hemp_call(value, defined, context);
}
