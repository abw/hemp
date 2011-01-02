#include <hemp/value.h>
#include <hemp/context.h>


HEMP_TYPE_FUNC(hemp_type_identity) {
    HempType type   = hemp_type_subtype(HempTypeValue, id, name);
    type->text      = &hemp_type_identity_text;    /* identity -> text    */
    type->number    = &hemp_type_identity_number;  /* identity -> number  */
    type->integer   = &hemp_type_identity_integer; /* identity -> integer */
    type->boolean   = &hemp_type_identity_boolean; /* identity -> boolean */
    type->compare   = &hemp_type_identity_compare; /* identity -> compare */
    type->defined   = &hemp_type_identity_defined; /* identity -> defined */
    return type;
};



HEMP_DO_INLINE HempString
hemp_identity_name(
    HempValue value
) {
    switch (HEMP_IDENT_ID(value)) {
        case 0:                     return HEMP_STR_INFINITY;
        case HEMP_IDENT_MISSING:    return HEMP_STR_MISSING;
        case HEMP_IDENT_NOTHING:    return HEMP_STR_NOTHING;
        case HEMP_IDENT_EMPTY:      return HEMP_STR_EMPTY;
        case HEMP_IDENT_FALSE:      return HEMP_STR_FALSE;
        case HEMP_IDENT_TRUE:       return HEMP_STR_TRUE;
        case HEMP_IDENT_BEFORE:     return HEMP_STR_BEFORE;
        case HEMP_IDENT_AFTER:      return HEMP_STR_AFTER;
        case HEMP_IDENT_EQUAL:      return HEMP_STR_EQUAL;
        default:                    return HEMP_STR_UNKNOWN;
    }
}



HEMP_OUTPUT(hemp_type_identity_text) {
    if (hemp_is_undefined(value)) {
        hemp_debug("identity(%s) text throwing undefined entity error\n", hemp_identity_name(value));
        HEMP_UNDEF_ERROR(
            context, 
            hemp_identity_name(value)
        );
    }
    HempText text;
    HempString name = hemp_identity_name(value);
    hemp_prepare_text_size(context, output, text, strlen(name));

    /* HempEmpty silently bypassed generating any text output */
    if (! hemp_is_empty(value))
        hemp_text_append_string(text, hemp_identity_name(value) );

    return output;
}


HEMP_VALUE(hemp_type_identity_number) {
    /* might want to auto-convert true(1), false(0), before(-1), equal(0)
     * and after(1)
     */
    HEMP_CONVERT_ERROR(
        context, 
        HEMP_STR_IDENTITY,
        HEMP_STR_NUMBER, 
        hemp_identity_name(value)
    );
    return HempMissing;
}


HEMP_VALUE(hemp_type_identity_integer) {
    /* might want to auto-convert true(1), false(0), before(-1), equal(0)
     * and after(1)
     */
    HEMP_CONVERT_ERROR(
        context, 
        HEMP_STR_IDENTITY,
        HEMP_STR_INTEGER,
        hemp_identity_name(value)
    );
    return HempMissing;
}


HEMP_VALUE(hemp_type_identity_boolean) {
    if (hemp_is_boolean(value)) 
        return hemp_is_true(value) 
            ? HempTrue 
            : HempFalse;
    else
        HEMP_CONVERT_ERROR(
            context, 
            HEMP_STR_IDENTITY,
            HEMP_STR_BOOLEAN, 
            hemp_identity_name(value)
        );

    return HempMissing;
}


HEMP_VALUE(hemp_type_identity_compare) {
    if (hemp_is_compare(value)) 
        return value;
    else 
        HEMP_CONVERT_ERROR(
            context, 
            HEMP_STR_IDENTITY,
            HEMP_STR_COMPARE, 
            hemp_identity_name(value)
        );

    return HempMissing;
}


HEMP_VALUE(hemp_type_identity_defined) {
    return hemp_is_defined(value) 
        ? HempTrue 
        : HempFalse;
}

