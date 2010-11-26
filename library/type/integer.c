#include <hemp/value.h>
#include <hemp/context.h>


HEMP_TYPE_FUNC(hemp_type_integer) {
    hemp_type_p type = hemp_type_subtype(HempValue, id, name);
    type->text       = &hemp_type_integer_text;    /* integer to text      */
    type->number     = &hemp_type_integer_number;  /* integer to number    */
    type->integer    = &hemp_value_self;            /* no-op returns int    */
    type->boolean    = &hemp_type_integer_boolean; /* number -> boolean    */
    type->compare    = &hemp_type_integer_compare; /* number -> comparison */
    type->defined    = &hemp_valuerue;            /* always defined       */
    return type;
};


HEMP_OUTPUT_FUNC(hemp_type_integer_text) {
    static hemp_char buffer[HEMP_BUFFER_SIZE];
    hemp_text text;

    snprintf(buffer, HEMP_BUFFER_SIZE, HEMP_FMT_INT, hemp_val_int(value));
    hemp_prepare_text_size(context, output, text, strlen(buffer));
    hemp_text_append_string(text, buffer);

    return output;
}


HEMP_VALUE_FUNC(hemp_type_integer_number) {
    return hemp_num_val((hemp_num) hemp_val_int(value));
}


HEMP_VALUE_FUNC(hemp_type_integer_boolean) {
    return hemp_val_int(value) == 0
        ? HempFalse
        : HempTrue;
}


HEMP_VALUE_FUNC(hemp_type_integer_compare) {
    hemp_num cmp = hemp_val_int(value);
    return  cmp < 0 ? HempBefore
        :   cmp > 0 ? HempAfter
        :             HempEqual; 
}


