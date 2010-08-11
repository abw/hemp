#include <hemp/element.h>


static struct hemp_symbol_s
    hemp_symbol_integer = { 
        "integer",                                  /* name                 */
        NULL,                                       /* start token          */
        NULL,                                       /* end token            */
        HEMP_BE_FIXED       |                       /* flags                */
        HEMP_BE_STATIC,
        0, 0,                                       /* l/r precedence       */
        NULL,                                       /* scanner callback     */
        NULL,                                       /* cleanup callback     */
        &hemp_element_number_expr,                  /* parse expression     */
        &hemp_element_not_infix,                    /* parse infix expr     */
        &hemp_element_literal_source,               /* source code          */
        &hemp_element_integer_text,                 /* output text          */
        &hemp_element_integer_number,               /* numeric conversion   */
        &hemp_element_integer_integer,              /* integer conversion   */
        &hemp_element_integer_boolean,              /* boolean conversion   */
    };
    
hemp_symbol_p HempSymbolInteger = &hemp_symbol_integer;


HEMP_SYMBOL_FUNC(hemp_element_integer_symbol) {
    symbol->expr    = &hemp_element_number_expr;
    symbol->expr    = &hemp_element_number_expr,
    symbol->source  = &hemp_element_literal_source;
    symbol->text    = &hemp_element_integer_text;
    symbol->number  = &hemp_element_integer_number;
    symbol->integer = &hemp_element_integer_integer;
    symbol->boolean = &hemp_element_integer_boolean;
    symbol->flags   = HEMP_BE_FIXED | HEMP_BE_STATIC;
    return symbol;
}


HEMP_OUTPUT_FUNC(hemp_element_integer_text) {
    debug_call("hemp_element_integer_text()\n");

    static hemp_char_t buffer[HEMP_BUFFER_SIZE];
    hemp_text_p text;

    snprintf(
        buffer, HEMP_BUFFER_SIZE, "%d", 
        HEMP_VAL_INT(element->args.value)
    );
    hemp_prepare_output(output, text, strlen(buffer));
    hemp_text_append_cstr(text, buffer);

    return output;
}


HEMP_VALUE_FUNC(hemp_element_integer_number) {
    debug_call("hemp_element_integer_number()\n");
    hemp_value_t value = element->args.value;
    return value;
    return HEMP_IS_NUM(value)
        ? value
        : HEMP_NUM_VAL((hemp_num_t) HEMP_VAL_INT(value));
}


HEMP_VALUE_FUNC(hemp_element_integer_integer) {
    debug_call("hemp_element_number_integer()\n");
    return element->args.value;
}


HEMP_VALUE_FUNC(hemp_element_integer_boolean) {
    hemp_todo("hemp_element_number_boolean() : number element => boolean value\n");
}

