#include <hemp/value.h>
#include <hemp/context.h>


const struct hemp_type_s hemp_type_text = {
    HEMP_TEXT_ID, "Text",
    &hemp_value_text_text,              /* return or append text            */
    &hemp_value_self,                   /* no-op to return text value       */
    &hemp_value_text_number,            /* text -> number conversion        */
    &hemp_value_text_integer,           /* text -> integer conversion       */
    &hemp_value_text_boolean,           /* text -> boolean conversion       */
    /* we could possible try and convert the text to a number, so that values
     * of "-1", "0" and "+1" work, but I don't think there's much point...
     */
    &hemp_value_cannot_compare,         /* text -> comparison conversion    */
    &hemp_value_true,                   /* number is always defined         */  /* what about NaN / Infinity? */
};


HEMP_VTEXT_FUNC(hemp_value_text_text) {
    /* FIXME: can we safely return value?  Should we return a copy?  What 
     * happens when the caller is done and tries to free it?  Ref counts, 
     * here we come.... :-(
     */

    /* if we haven't been passed an output buffer then we return self */
    if (hemp_is_undef(output))
        return value;

    /* if we have been passed an output buffer then we append the value
     * text onto the end of it 
     */
         hemp_debug("hemp_value_text_text\n");
//    hemp_text_p text = hemp_val_text(output);
    hemp_text_append_text(hemp_val_text(output), hemp_val_text(value));
    return output;
}


HEMP_VALUE_FUNC(hemp_value_text_number) {
    hemp_text_p text = hemp_val_text(value);
    hemp_cstr_p end;
    hemp_num_t  nval;
    
    if (! text->length) {
        HEMP_CONVERT_ERROR(
            context, 
            HEMP_STR_NO_TEXT,
            HEMP_STR_NUMBER,
            HEMP_STR_BLANK
        );
    }

    errno = 0;
    nval  = strtod(text->string, &end);
    
    if (*end || (errno == EINVAL)) {
        HEMP_CONVERT_ERROR(
            context, 
            HEMP_STR_TEXT, 
            HEMP_STR_NUMBER,
            text->string
        );
    }
    else if (errno == ERANGE) {
        HEMP_OVERFLOW_ERROR(
            context, 
            text->string
        );
    }
    else {
        return hemp_num_val(nval);
    }

    return hemp_num_val(0);
}


HEMP_VALUE_FUNC(hemp_value_text_integer) {
    hemp_value_t nval = hemp_value_text_number(value, context);
    return hemp_int_val((hemp_int_t) hemp_val_num(nval));
}


HEMP_VALUE_FUNC(hemp_value_text_boolean) {
    /* hmmm... must be careful here... I think the best approach is to say 
     * that any non-zero length string is true, but unlike Perl, we won't 
     * perform any implicit text->number conversion.  The end result is that 
     * things like "0" and "false" (both strings) are true values, while 0 
     * (integer zero), 0.0 (float zero), "" (empty string) and HempFalse 
     * (explicit false) are all false.
     */
    return hemp_val_text(value)->length
        ? HempTrue
        : HempFalse;
}

