#include <hemp/value.h>
#include <hemp/context.h>


HEMP_TYPE_FUNC(hemp_type_identity) {
    hemp_type_p type = hemp_type_subtype(HempValue, id, name);
    type->text       = &hemp_value_identity_text;    /* identity -> text    */
    type->number     = &hemp_value_identity_number;  /* identity -> number  */
    type->integer    = &hemp_value_identity_integer; /* identity -> integer */
    type->boolean    = &hemp_value_identity_boolean; /* identity -> boolean */
    type->compare    = &hemp_value_identity_compare; /* identity -> compare */
    type->defined    = &hemp_value_identity_defined; /* identity -> defined */
    return type;
};



HEMP_DO_INLINE hemp_str_p
hemp_identity_name(
    hemp_value_t value
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



HEMP_VTEXT_FUNC(hemp_value_identity_text) {
    if (hemp_is_undefined(value)) {
        hemp_debug("throwing undefined entity error\n");
        HEMP_UNDEF_ERROR(
            context, 
            hemp_identity_name(value)
        );
    }
    hemp_str_p name = hemp_identity_name(value);
    hemp_text_p text;
    hemp_prepare_text_size(context, output, text, strlen(name));
    hemp_text_append_string(text, name);
    return output;
}


HEMP_VALUE_FUNC(hemp_value_identity_number) {
    /* might want to auto-convert true(1), false(0), before(-1), equal(0)
     * and after(1)
     */
    HEMP_CONVERT_ERROR(
        context, 
        HEMP_STR_IDENTITY,
        HEMP_STR_NUMBER, 
        hemp_identity_name(value)
    );
}


HEMP_VALUE_FUNC(hemp_value_identity_integer) {
    /* might want to auto-convert true(1), false(0), before(-1), equal(0)
     * and after(1)
     */
    HEMP_CONVERT_ERROR(
        context, 
        HEMP_STR_IDENTITY,
        HEMP_STR_INTEGER,
        hemp_identity_name(value)
    );
}


HEMP_VALUE_FUNC(hemp_value_identity_boolean) {
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
}


HEMP_VALUE_FUNC(hemp_value_identity_compare) {
    if (hemp_is_compare(value)) 
        return value;
    else 
        HEMP_CONVERT_ERROR(
            context, 
            HEMP_STR_IDENTITY,
            HEMP_STR_COMPARE, 
            hemp_identity_name(value)
        );
}


HEMP_VALUE_FUNC(hemp_value_identity_defined) {
    return hemp_is_defined(value) 
        ? HempTrue 
        : HempFalse;
}

