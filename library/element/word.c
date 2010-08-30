#include <hemp/element.h>


static struct hemp_symbol_s
    hemp_symbol_word = { 
        "word",                                     /* name                 */
        NULL,                                       /* start token          */
        NULL,                                       /* end token            */
        0,                                          /* flags                */
        0, 0,                                       /* l/r precedence       */
        NULL,                                       /* scanner callback     */
        NULL,                                       /* cleanup callback     */
        &hemp_element_word_prefix,                  /* prefix expression    */
        &hemp_element_not_postfix,                  /* postfix expr         */
        &hemp_element_literal_token,                /* source token         */
        &hemp_element_literal_source,               /* source code          */
        &hemp_element_value_text,                   /* output text          */
        &hemp_element_word_value,                   /* output value         */
        &hemp_element_value_number,                 /* numeric conversion   */
        &hemp_element_value_integer,                /* integer conversion   */
        &hemp_element_value_boolean,                /* boolean conversion   */
    };

hemp_symbol_p HempSymbolWord = &hemp_symbol_word;


HEMP_SYMBOL_FUNC(hemp_element_word_symbol) {
    hemp_element_literal_symbol(hemp, symbol);
    /* these aren't right, but they'll do for now, for testing purposes */
    symbol->flags  = HEMP_BE_SOURCE | HEMP_BE_STATIC;
    symbol->prefix = &hemp_element_literal_prefix;
    return symbol;
}


HEMP_PREFIX_FUNC(hemp_element_word_prefix) {
    hemp_debug_call(
        "hemp_element_word_prefix() precedence is %d, parg: %d\n", 
        (*elemptr)->type->lprec, precedence
    );
    
    hemp_element_p element = *elemptr;

    if (hemp_has_next(elemptr)) {
        hemp_go_next(elemptr);
        hemp_skip_whitespace(elemptr);
        return hemp_parse_postfix(elemptr, scope, precedence, force, element);
    }

    return element;
}


HEMP_EVAL_FUNC(hemp_element_word_value) {
    hemp_debug_call("hemp_element_word_value()\n");
    return hemp_hash_fetch_keylen(
        context->vars, element->token, element->length
    );
}




