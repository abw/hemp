#include <hemp/value.h>
#include <hemp/context.h>


HEMP_TYPE_FUNC(hemp_type_text) {
    hemp_type_p type = hemp_type_init(id, name);
    type->text       = &hemp_value_text_text;       /* return/append text   */
    type->number     = &hemp_value_text_number;     /* text -> number       */
    type->integer    = &hemp_value_text_integer;    /* text -> integer      */
    type->boolean    = &hemp_value_text_boolean;    /* text -> boolean      */
    type->compare    = &hemp_value_not_compare;     /* can't compare        */
    type->defined    = &hemp_value_true;            /* always defined       */
    return type;
};


HEMP_VTEXT_FUNC(hemp_value_text_text) {
     hemp_debug_call("hemp_value_text_text\n");

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

