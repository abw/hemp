#include <hemp/element.h>


static struct hemp_symbol_s
    hemp_symbol_number = { 
        "number",                                   /* name                 */
        NULL,                                       /* start token          */
        NULL,                                       /* end token            */
        HEMP_IS_FIXED       |                       /* flags                */
        HEMP_IS_STATIC,
        0, 0,                                       /* l/r precedence       */
        NULL,                                       /* scanner callback     */
        NULL,                                       /* cleanup callback     */
        &hemp_element_number_expr,                  /* parse expression     */
        &hemp_element_not_infix,                    /* parse infix expr     */
        &hemp_element_literal_source,               /* source code          */
        &hemp_element_number_text,                  /* output text          */
        &hemp_element_number_number,                /* numeric conversion   */
        &hemp_element_number_integer,               /* integer conversion   */
        &hemp_element_number_boolean,               /* boolean conversion   */
    };

hemp_symbol_p HempSymbolNumber = &hemp_symbol_number;


HEMP_SYMBOL_FUNC(hemp_element_number_symbol) {
    symbol->expr    = &hemp_element_number_expr,
    symbol->source  = &hemp_element_literal_source;
    symbol->text    = &hemp_element_number_text;
    symbol->number  = &hemp_element_number_number;
    symbol->integer = &hemp_element_number_integer;
    symbol->boolean = &hemp_element_number_boolean;
    symbol->flags   = HEMP_IS_FIXED | HEMP_IS_STATIC;
    return symbol;
}


HEMP_PARSE_FUNC(hemp_element_number_expr) {
    debug_blue("hemp_element_number_parse_expr() precedence is %d, parg: %d\n", (*elemptr)->type->lprec, precedence);
    
    hemp_element_p element = *elemptr;
  
    if (hemp_has_next(elemptr)) {
        hemp_go_next(elemptr);
        hemp_skip_space(elemptr);
        hemp_symbol_p sym = (*elemptr)->type;

        if (sym->infix) {
//          return hemp_parse_infix(HEMP_PARSE_ARGS, element);
            return sym->infix(elemptr, scope, precedence, force, element);
        }
    }

    return element;
}


HEMP_OUTPUT_FUNC(hemp_element_number_text) {
    debug_call("hemp_element_numop_text()\n");

    static hemp_char_t buffer[HEMP_BUFFER_SIZE];
    hemp_text_p text;

    snprintf(
        buffer, HEMP_BUFFER_SIZE, "%lf", 
        HEMP_VAL_NUM(element->args.value)
    );
    hemp_prepare_output(output, text, strlen(buffer));
    hemp_text_append_cstr(text, buffer);

    return output;
}


HEMP_VALUE_FUNC(hemp_element_number_number) {
    debug_call("hemp_element_number_number()\n");
    return element->args.value;
}


HEMP_VALUE_FUNC(hemp_element_number_integer) {
    debug_call("hemp_element_number_integer()\n");
    hemp_value_t value = element->args.value;
    return HEMP_IS_INT(value)
        ? value
        : HEMP_INT_VAL((hemp_int_t) HEMP_VAL_NUM(value));
}


HEMP_VALUE_FUNC(hemp_element_number_boolean) {
    hemp_todo("hemp_element_number_boolean() : number element => boolean value\n");
}

