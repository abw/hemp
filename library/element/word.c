#include <hemp/element.h>


/*--------------------------------------------------------------------------
 * global element type
 *--------------------------------------------------------------------------*/

hemp_element HempElementWord = NULL;


HEMP_GLOBAL_ELEMENT(hemp_global_element_word) {
    return hemp_element_word(
        NULL,
        hemp_element_new("hemp.word", NULL, NULL)
    );
}


HEMP_ELEMENT(hemp_element_word) {
    hemp_element_literal(hemp, element);
    /* these aren't right, but they'll do for now, for testing purposes */
    element->parse_prefix    = &hemp_element_word_prefix;
    element->parse_fixed     = &hemp_element_parse_fixed;
    element->parse_proto     = &hemp_element_word_proto;
    element->parse_lvalue    = &hemp_element_word_lvalue;
    element->parse_pair      = &hemp_element_parse_prefix_pair;
    element->value           = &hemp_element_word_value;
    element->text            = &hemp_element_value_text;
    element->number          = &hemp_element_value_number;
    element->integer         = &hemp_element_value_integer;
    element->boolean         = &hemp_element_value_boolean;
    element->assign          = &hemp_element_word_assign;
    element->cleanup         = &hemp_element_word_cleanup;
    hemp_set_flag(element, HEMP_BE_LVALUE);
    return element;
}


HEMP_PREFIX(hemp_element_word_prefix) {
    hemp_debug_call("hemp_element_word_prefix()\n");
    
    hemp_fragment fragment = hemp_element_parse_fixed(
        fragptr, scope, precedence, force
    );
    hemp_clear_flag(fragment, HEMP_BE_FIXED);

    return hemp_parse_postfix(fragptr, scope, precedence, force, fragment);
}


HEMP_FIXUP(hemp_element_word_lvalue) {
    hemp_debug_call("hemp_element_word_lvalue()\n");
    hemp_set_flag(fragment, HEMP_BE_LVALUE|HEMP_BE_FIXED);
    return hemp_val_frag(fixative);
}


HEMP_FIXUP(hemp_element_word_proto) {
    hemp_debug_call("hemp_element_word_proto()\n");
    hemp_proto  proto   = (hemp_proto) hemp_val_ptr(fixative);
    hemp_string name    = hemp_string_extract(
        fragment->token, 
        fragment->token + fragment->length
    );
    hemp_proto_add_item(proto, name);
    return fragment;
}


HEMP_VALUE(hemp_element_word_value) {
    hemp_debug_call("hemp_element_word_value()\n");
    hemp_fragment fragment = hemp_val_frag(value);
    hemp_value    word     = hemp_expr(fragment);

    /* Words can be used in two different contexts.  When they appear at the
     * start of an expression (prefix) they represent a variable.  However,
     * they can also be used as fixed values, e.g. on the RHS of a dotop.
     * In "foo.bar", "foo" is a prefix word (variable reference) and "bar" is
     * a fixed word (static string).
     */
    if (hemp_has_flag(fragment, HEMP_BE_FIXED)) {
//      hemp_debug_msg("word has fixed value: %s\n", hemp_val_str(word));
        return word;
    }
    else {
//      hemp_debug_msg("fetching value for word %s\n", hemp_val_str(word));
        return hemp_hash_fetch_keylen(
            context->vars, hemp_val_str(word), fragment->length
        );
    }
}


HEMP_INPUT(hemp_element_word_assign) {
    hemp_fragment fragment = hemp_val_frag(value);
    hemp_value    word     = hemp_expr(fragment);

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


HEMP_CLEANUP(hemp_element_word_cleanup) {
    hemp_debug_call("hemp_element_word_cleanup(%p)\n", fragment);

    /* memory should always be allocated via hemp_element_parse_fixed() which
     * gets called regardless of how the word is being used (either way
     * we need a fixed string for the word, either to return as a fixed
     * value or to use as a variable hash key
     */
    if (hemp_has_flag(fragment, HEMP_BE_ALLOCATED)) {
//      hemp_debug_msg("freeing allocated memory: %p\n", hemp_val_ptr(hemp_expr(fragment)));
        hemp_mem_free(
            hemp_val_str( 
                hemp_expr(fragment)
            )
        );
    }
    else {
//      hemp_debug_msg("memory not allocated memory\n");
    }
}



