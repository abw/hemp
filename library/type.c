#include <hemp/type.h>


hemp_type_p  HempNumber   = NULL;
hemp_type_p  HempInteger  = NULL;
hemp_type_p  HempString   = NULL;
hemp_type_p  HempText     = NULL;
hemp_type_p  HempIdentity = NULL;
hemp_type_p  HempReserved = NULL;
hemp_type_p  HempUnused   = NULL;
hemp_type_p  hemp_global_types[HEMP_TYPES_SIZE];



hemp_type_p 
hemp_type_init(
    hemp_int_t  id,
    hemp_cstr_p name
) {
    hemp_type_p type = (hemp_type_p) hemp_mem_alloc(
        sizeof(struct hemp_type_s)
    );

    if (! type)
        hemp_mem_fail("type");

    type->id      = id;
    type->name    = hemp_cstr_clone(name, "type name");
    type->value   = &hemp_value_self;
    type->text    = &hemp_value_not_text; 
    type->number  = &hemp_value_not_number;
    type->integer = &hemp_value_not_integer;
    type->boolean = &hemp_value_not_boolean;
    type->compare = &hemp_value_not_compare;
    type->defined = &hemp_value_not_defined;

    // TODO: vtable

    return type;
}


void 
hemp_type_free(
    hemp_type_p type
) {
    hemp_mem_free(type->name);
    hemp_mem_free(type);
}


void
hemp_global_types_init() {
    hemp_debug_call("hemp_global_types_init()\n");
    hemp_int_t n;

    /* return silently if it looks like we've already done this step */
    if (HempNumber)
        return;

    /* construct the global type objects */
    HempNumber   = hemp_type_number  ( HEMP_NUMBER_ID,   HEMP_STR_NUMBER   );
    HempInteger  = hemp_type_integer ( HEMP_INTEGER_ID,  HEMP_STR_INTEGER  );
    HempString   = hemp_type_string  ( HEMP_STRING_ID,   HEMP_STR_STRING   );
    HempText     = hemp_type_text    ( HEMP_TEXT_ID,     HEMP_STR_TEXT     );
    HempIdentity = hemp_type_identity( HEMP_IDENTITY_ID, HEMP_STR_IDENTITY );
    HempReserved = hemp_type_reserved( HEMP_RESERVED_ID, HEMP_STR_RESERVED );
    HempUnused   = hemp_type_unused  ( HEMP_UNUSED_ID,   HEMP_STR_UNUSED   );

    /* The first 16 type entries are reserved for hemp's internal use */
    for (n = 0; n < HEMP_TYPES_RESERVED; n++) {
        hemp_global_types[n] = HempReserved;
    }

    /* The second 16 type entries are available for use, e.g. for alien data */
    for (n = HEMP_TYPES_RESERVED; n < HEMP_TYPES_SIZE; n++) {
        hemp_global_types[n] = HempUnused;
    }

    /* add the builtin types to the type table */
    hemp_global_types[ HempNumber->id   ] = HempNumber;
    hemp_global_types[ HempInteger->id  ] = HempInteger;
    hemp_global_types[ HempString->id   ] = HempString;
    hemp_global_types[ HempText->id     ] = HempText;
    hemp_global_types[ HempIdentity->id ] = HempIdentity;
}


void
hemp_global_types_free() {
    hemp_debug_call("hemp_global_types_free()\n");
    hemp_int_t n;

    for (n = 0; n < HEMP_TYPES_SIZE; n++) {
        hemp_global_types[n] = NULL;
    }

    hemp_type_free(HempNumber);     HempNumber   = NULL;
    hemp_type_free(HempInteger);    HempInteger  = NULL;
    hemp_type_free(HempString);     HempString   = NULL;
    hemp_type_free(HempText);       HempText     = NULL;
    hemp_type_free(HempIdentity);   HempIdentity = NULL;
    hemp_type_free(HempReserved);   HempReserved = NULL;
    hemp_type_free(HempUnused);     HempUnused   = NULL;
}


HEMP_TYPE_FUNC(hemp_type_reserved) {
    hemp_type_p type = hemp_type_init(HEMP_RESERVED_ID, HEMP_STR_RESERVED);
    return type;
};


HEMP_TYPE_FUNC(hemp_type_unused) {
    hemp_type_p type = hemp_type_init(HEMP_UNUSED_ID, HEMP_STR_UNUSED);
    return type;
};

