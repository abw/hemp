#include <hemp/element.h>


hemp_symbol HempSymbolWord = NULL;


HEMP_GLOBAL_SYMBOL(hemp_symbol_word) {
    hemp_debug_call("hemp_symbol_word()\n");
    return hemp_element_word_symbol(
        NULL,
        hemp_symbol_new("hemp.word", NULL, NULL)
    );
}


HEMP_SYMBOL(hemp_element_word_symbol) {
    hemp_element_literal_symbol(hemp, symbol);
    /* these aren't right, but they'll do for now, for testing purposes */
    symbol->parse_prefix    = &hemp_element_word_prefix;
    symbol->parse_fixed     = &hemp_element_fixed;
    symbol->parse_proto     = &hemp_element_word_proto;
    symbol->parse_lvalue    = &hemp_element_word_lvalue;
    symbol->value           = &hemp_element_word_value;
    symbol->text            = &hemp_element_value_text;
    symbol->number          = &hemp_element_value_number;
    symbol->integer         = &hemp_element_value_integer;
    symbol->boolean         = &hemp_element_value_boolean;
    symbol->assign          = &hemp_element_word_assign;
    symbol->cleanup         = &hemp_element_word_clean;
    symbol->flags          |= HEMP_BE_LVALUE;
//    symbol->flags    = HEMP_BE_SOURCE | HEMP_BE_FIXED;
    return symbol;
}


HEMP_PREFIX_FUNC(hemp_element_word_prefix) {
    hemp_debug_call(
        "hemp_element_word_prefix() precedence is %d, parg: %d\n", 
        (*elemptr)->type->lprec, precedence
    );
    
    hemp_element element = hemp_element_fixed(
        elemptr, scope, precedence, force
    );
    hemp_clear_flag(element, HEMP_BE_FIXED);

    return hemp_parse_postfix(elemptr, scope, precedence, force, element);
}


HEMP_FIXUP_FUNC(hemp_element_word_lvalue) {
    hemp_debug_call("hemp_element_word_lvalue()\n");
    hemp_set_flag(element, HEMP_BE_LVALUE|HEMP_BE_FIXED);
    return hemp_val_elem(fixative);
}


HEMP_FIXUP_FUNC(hemp_element_word_proto) {
    hemp_debug_call("hemp_element_word_proto()\n");
    hemp_proto  proto   = (hemp_proto) hemp_val_ptr(fixative);
    hemp_string name    = hemp_string_extract(
        element->token, 
        element->token + element->length
    );
//  hemp_debug_msg("adding word to prototype: %s\n", name);
    hemp_proto_add_item(proto, name);
//    hemp_mem_free(name);
    return element;
}


HEMP_VALUE_FUNC(hemp_element_word_value) {
    hemp_debug_call("hemp_element_word_value()\n");
    hemp_element  element = hemp_val_elem(value);
    hemp_value    word    = hemp_expr(element);

    /* Words can be used in two different contexts.  When they appear at the
     * start of an expression (prefix) they represent a variable.  However,
     * they can also be used as fixed values, e.g. on the RHS of a dotop.
     * In "foo.bar", "foo" is a prefix word (variable reference) and "bar" is
     * a fixed word (static string).
     */
    if (hemp_has_flag(element, HEMP_BE_FIXED)) {
//      hemp_debug_msg("word has fixed value: %s\n", hemp_val_str(word));
        return word;
    }
    else {
//      hemp_debug_msg("fetching value for word %s\n", hemp_val_str(word));
        return hemp_hash_fetch_keylen(
            context->vars, hemp_val_str(word), element->length
        );
    }
}


HEMP_INPUT_FUNC(hemp_element_word_assign) {
    hemp_element  element = hemp_val_elem(value);
    hemp_value    word    = hemp_expr(element);

    hemp_debug_call(
        "hemp_element_word_assign() %s <- %s\n", 
        hemp_val_str(word),
        hemp_type_name(input)
    );
    
    /* The value we're passed as an operand is an element that should be
     * evaluated to yield a value.  Not sure if this is the best approach,
     * but it allows LHS expressions that are "lazy" (i.e. have parens on
     * the end) to delay evaluation and instead store a reference to the 
     * RHS element that can be evaluated later.
     */

    hemp_hash_store(
        context->vars, hemp_val_str(word), 
        hemp_call(input, value, context)
    );
    return input;
}


void
hemp_element_word_clean(
    hemp_element element
) {
    hemp_debug_call("hemp_element_word_clean(%p)\n", element);

    /* memory should always be allocated via hemp_element_fixed() which
     * gets called regardless of how the word is being used (either way
     * we need a fixed string for the word, either to return as a fixed
     * value or to use as a variable hash key
     */
    if (hemp_has_flag(element, HEMP_BE_ALLOCATED)) {
        hemp_mem_free(
            hemp_val_str( 
                hemp_expr(element)
            )
        );
    }
}



