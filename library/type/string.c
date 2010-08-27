#include <hemp/value.h>
#include <hemp/context.h>


const struct hemp_type_s hemp_type_string = {
    HEMP_STRING_ID, "String",
    &hemp_value_string_text,            /* convert/append to text           */
    &hemp_value_self,                   /* return self as value             */
    &hemp_value_string_number,          /* text -> number conversion        */
    &hemp_value_string_integer,         /* text -> integer conversion       */
    &hemp_value_string_boolean,         /* text -> boolean conversion       */
    /* we could possible try and convert the text to a number, so that values
     * of "-1", "0" and "+1" work, but I don't think there's much point...
     */
    &hemp_value_cannot_compare,         /* string is not a comparison       */
    &hemp_value_true,                   /* string is always defined         */  /* what about NULL? */
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

