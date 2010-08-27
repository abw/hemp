#include <hemp/value.h>
#include <hemp/context.h>


const struct hemp_type_s hemp_type_identity = {
    HEMP_IDENTITY_ID, "Identity",
    &hemp_value_identity_text,          /* identity->text conversion        */
    &hemp_value_self,                   /* no-op to return identity value   */
    &hemp_value_identity_number,        /* identity -> number conversion    */
    &hemp_value_identity_integer,       /* identity -> integer conversion   */
    &hemp_value_identity_boolean,       /* identity -> boolean check        */
    &hemp_value_identity_compare,       /* identity -> comparison check     */
    &hemp_value_identity_defined,       /* identity -> defined check        */
};


HEMP_DO_INLINE hemp_cstr_p
hemp_identity_name(
    hemp_value_t value
) {
    switch (HEMP_IDENT_ID(value)) {
        case 0:                     return HEMP_STR_INFINITY;
        case HEMP_IDENT_MISSING:    return HEMP_STR_MISSING;
        case HEMP_IDENT_NOTHING:    return HEMP_STR_NOTHING;
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
        HEMP_UNDEF_ERROR(
            context, 
            hemp_identity_name(value)
        );
    }
    hemp_cstr_p name = hemp_identity_name(value);
    hemp_text_p text;
    hemp_prepare_output(output, text, strlen(name));
    hemp_text_append_cstr(text, name);
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

