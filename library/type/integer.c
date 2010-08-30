#include <hemp/value.h>
#include <hemp/context.h>


HEMP_TYPE_FUNC(hemp_type_integer) {
    hemp_type_p type = hemp_type_init(id, name);
    type->text       = &hemp_value_integer_text;    /* integer to text      */
    type->number     = &hemp_value_integer_number;  /* integer to number    */
    type->integer    = &hemp_value_self;            /* no-op returns int    */
    type->boolean    = &hemp_value_integer_boolean; /* number -> boolean    */
    type->compare    = &hemp_value_integer_compare; /* number -> comparison */
    type->defined    = &hemp_value_true;            /* always defined       */
    return type;
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


