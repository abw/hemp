#include <hemp/value.h>
#include <hemp/context.h>


const struct hemp_type_s hemp_type_number = {
    HEMP_NUMBER_ID, "Number",
    &hemp_value_number_text,            /* convert number to text           */
    &hemp_value_self,                   /* no-op to return value            */
    &hemp_value_self,                   /* no-op to return number           */
    &hemp_value_number_integer,         /* number -> integer conversion     */
    &hemp_value_number_boolean,         /* number -> boolean conversion     */
    &hemp_value_number_compare,         /* number -> comparison conversion  */
    &hemp_value_true,                   /* number is always defined         */  /* what about NaN / Infinity? */
};


HEMP_VTEXT_FUNC(hemp_value_number_text) {
    static hemp_char_t buffer[HEMP_BUFFER_SIZE];
    hemp_text_p text;

    snprintf(buffer, HEMP_BUFFER_SIZE, HEMP_FMT_NUM, hemp_val_num(value));
    hemp_prepare_output(output, text, strlen(buffer));
    hemp_text_append_cstr(text, buffer);

    return output;
}


HEMP_VALUE_FUNC(hemp_value_number_integer) {
    return hemp_int_val((hemp_int_t) hemp_val_num(value));
}


HEMP_VALUE_FUNC(hemp_value_number_boolean) {
    /* TODO: decide if this is the right thing to do */
    return hemp_val_num(value) == 0.0
        ? HempFalse
        : HempTrue;
}


HEMP_VALUE_FUNC(hemp_value_number_compare) {
    hemp_num_t cmp = hemp_val_num(value);
    return  cmp < 0 ? HempBefore
        :   cmp > 0 ? HempAfter
        :             HempEqual; 
}

