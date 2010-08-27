#include <hemp/value.h>
#include <hemp/context.h>


const struct hemp_type_s hemp_type_integer = {
    HEMP_INTEGER_ID, "Integer",
    &hemp_value_integer_text,           /* convert integer to text          */
    &hemp_value_self,                   /* no-op to return value            */
    &hemp_value_integer_number,         /* integer -> number conversion     */
    &hemp_value_self,                   /* no-op to return integer value    */
    &hemp_value_integer_boolean,        /* integer -> boolean conversion    */
    &hemp_value_integer_compare,        /* integer -> comparison conversion */
    &hemp_value_true,                   /* integer is always defined        */
};


HEMP_VTEXT_FUNC(hemp_value_integer_text) {
    static hemp_char_t buffer[HEMP_BUFFER_SIZE];
    hemp_text_p text;

    snprintf(buffer, HEMP_BUFFER_SIZE, HEMP_FMT_INT, hemp_val_int(value));
    hemp_prepare_output(output, text, strlen(buffer));
    hemp_text_append_cstr(text, buffer);

    return output;
}


HEMP_VALUE_FUNC(hemp_value_integer_number) {
    return hemp_num_val((hemp_num_t) hemp_val_int(value));
}


HEMP_VALUE_FUNC(hemp_value_integer_boolean) {
    return hemp_val_int(value) == 0
        ? HempFalse
        : HempTrue;
}


HEMP_VALUE_FUNC(hemp_value_integer_compare) {
    hemp_num_t cmp = hemp_val_int(value);
    return  cmp < 0 ? HempBefore
        :   cmp > 0 ? HempAfter
        :             HempEqual; 
}


