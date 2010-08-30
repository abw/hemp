#include <hemp/value.h>
#include <hemp/context.h>


HEMP_TYPE_FUNC(hemp_type_string) {
    hemp_type_p type = hemp_type_init(id, name);
    type->text       = &hemp_value_string_text;     /* string -> text       */
    type->number     = &hemp_value_string_number;   /* string -> number     */
    type->integer    = &hemp_value_string_integer;  /* string -> integer    */
    type->boolean    = &hemp_value_string_boolean;  /* string -> boolean    */
    type->compare    = &hemp_value_not_compare;     /* cannot compare       */
    type->defined    = &hemp_value_true;            /* always defined       */
    return type;
};


HEMP_VTEXT_FUNC(hemp_value_string_text) {
    hemp_cstr_p str = hemp_val_str(value);
    hemp_text_p text;

    hemp_prepare_output(output, text, strlen(str));

    hemp_text_append_cstr(text, str);
    return output;
}


HEMP_VALUE_FUNC(hemp_value_string_number) {
    hemp_cstr_p str = hemp_val_str(value);
    hemp_cstr_p end;
    hemp_num_t  nval;
    
    if (! str || ! *str) {
        HEMP_CONVERT_ERROR(
            context, 
            HEMP_STR_NO_TEXT,
            HEMP_STR_NUMBER,
            HEMP_STR_BLANK
        );
    }

    errno = 0;
    nval  = strtod(str, &end);
    
    if (*end || (errno == EINVAL)) {
        HEMP_CONVERT_ERROR(
            context, 
            HEMP_STR_STRING, 
            HEMP_STR_NUMBER,
            str
        );
    }
    else if (errno == ERANGE) {
        HEMP_OVERFLOW_ERROR(
            context, 
            str
        );
    }
    else {
        return hemp_num_val(nval);
    }

    return hemp_num_val(0);
}


HEMP_VALUE_FUNC(hemp_value_string_integer) {
    hemp_value_t nval = hemp_value_string_number(value, context);
    return hemp_int_val((hemp_int_t) hemp_val_num(nval));
}


HEMP_VALUE_FUNC(hemp_value_string_boolean) {
    hemp_cstr_p str = hemp_val_str(value);
    /* TODO: check this is right: any non-zero length string is true.
     * What about "0" and "0.000" ?
     */
    return str && *str
        ? HempTrue
        : HempFalse;
}

