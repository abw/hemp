#include <hemp/element.h>


hemp_symbol_p HempSymbolWord = NULL;


HEMP_GLOBAL_SYMBOL(hemp_symbol_word) {
    hemp_debug_call("hemp_symbol_word()\n");
    return hemp_element_word_symbol(
        NULL,
        hemp_symbol_init("hemp.word", NULL, NULL)
    );
}


HEMP_SYMBOL_FUNC(hemp_element_word_symbol) {
    hemp_element_literal_symbol(hemp, symbol);
    /* these aren't right, but they'll do for now, for testing purposes */
    symbol->prefix   = &hemp_element_word_prefix;
    symbol->fixed    = &hemp_element_fixed;
    symbol->value    = &hemp_element_word_value;
    symbol->text     = &hemp_element_value_text;
    symbol->number   = &hemp_element_value_number;
    symbol->integer  = &hemp_element_value_integer;
    symbol->boolean  = &hemp_element_value_boolean;
//    symbol->flags    = HEMP_BE_SOURCE | HEMP_BE_FIXED;
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
//        hemp_skip_whitespace(elemptr);
        return hemp_parse_postfix(elemptr, scope, precedence, force, element);
    }

    return element;
}


HEMP_EVAL_FUNC(hemp_element_word_value) {
    hemp_debug_call("hemp_element_word_value()\n");
    hemp_debug("hemp_element_word_value()\n");

    if (hemp_has_flag(element, HEMP_BE_FIXED)) {
        return element->type->token(element, context, HempNothing);
    }
    else {
        return hemp_hash_fetch_keylen(
            context->vars, element->token, element->length
        );
    }
}




