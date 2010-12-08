#include <hemp/language/test.h>


static struct hemp_elements hemp_elements_test[] = {
    { "test.test",   &hemp_element_test_test     },
    { "test.expect", &hemp_element_test_expect   },
    { NULL, NULL },
};


HEMP_LANGUAGE(hemp_language_test) {
    hemp_debug("hemp_language_test(%p, %s)\n", hemp, name);

    hemp_language language = hemp_language_new(
        hemp, name, HEMP_TEST_VERSION
    );
    
    hemp_register_dialect(hemp, HEMP_TEST, &hemp_dialect_test);
    hemp_register_grammar(hemp, HEMP_TEST, &hemp_grammar_test);
    hemp_register_elements(hemp, hemp_elements_test);

    return language;
}


/*--------------------------------------------------------------------------
 * dialect
 *--------------------------------------------------------------------------*/

HEMP_DIALECT(hemp_dialect_test) {
    hemp_dialect dialect = hemp_dialect_new(hemp, name);
    dialect->prepare = &hemp_dialect_test_prepare;
    dialect->cleanup = &hemp_dialect_test_cleanup;
    return dialect;
}


hemp_template
hemp_dialect_test_prepare(
    hemp_template tmpl
) {
    hemp_debug("hemp_dialect_test_prepare(%p)\n", tmpl);
/*
    hemp_hemp    hemp    = tmpl->dialect->hemp;
    hemp_tagset  tagset  = tmpl->tagset;
    hemp_grammar grammar = hemp_grammar_instance(hemp, HEMP_TEST);
*/
// broke while reworking tags
//    hemp_tagset_add_tag(
//        tagset, 
//        hemp_tag_init(
//            HEMP_TEST,
//            HEMP_OUTLINE_TAG,
//            "--", "NULL",
//            &hemp_outline_tag_scanner,
//            grammar
//        )
//    );

    return tmpl;
}


void
hemp_dialect_test_cleanup(
    hemp_template tmpl
) {
    hemp_debug_call("hemp_dialect_test_cleanup(%p)\n", tmpl);
}



/*--------------------------------------------------------------------------
 * grammar
 *--------------------------------------------------------------------------*/

HEMP_GRAMMAR(hemp_grammar_test) {
    hemp_debug("hemp_grammar_test(%p, %s)\n", hemp, name);
    hemp_grammar grammar = hemp_grammar_hemp_alpha(hemp, name);
    HEMP_USE_BLOCK("test.test",   "test",   11);
    HEMP_USE_BLOCK("test.expect", "expect", 11);
    return grammar;
}


/*--------------------------------------------------------------------------
 * elements
 *--------------------------------------------------------------------------*/

HEMP_ELEMENT(hemp_element_test_test) {
    element->scanner         = &hemp_element_test_test_scanner;
    element->cleanup         = &hemp_element_test_test_cleanup,
    element->value           = &hemp_element_test_test_value,
    element->text            = &hemp_element_value_text,
    element->token           = &hemp_element_literal_text;
    element->source          = &hemp_element_literal_text;
    element->parse_prefix    = &hemp_element_test_test_prefix;
    element->flags           = HEMP_BE_SOURCE | HEMP_BE_FIXED;
    return element;
}


HEMP_SCAN_FUNC(hemp_element_test_test_scanner) {
    hemp_string src = *srcptr;

    hemp_debug_call("hemp_element_test_test_scanner()\n");

    /* TODO: add to_tag_end() method to scan all text, accounting for
     * inline/outline tags
     */

    /* walk to the end of line or end of tag */
    src = tag->to_eol(tag, src);

    hemp_debug_token("TEST", *srcptr, src - *srcptr);

    /* update the source pointer past the text we've consumed */
    *srcptr = src;
    
    /* add a comment element to the list of scanned tokens */
    return hemp_fragments_add_fragment(
        tmpl->fragments, element,
        start, pos, src - start
    );
}


HEMP_PREFIX(hemp_element_test_test_prefix) {
    hemp_fragment self = *fragptr;
    hemp_element  type = self->type;
    hemp_fragment block;

    hemp_debug("hemp_element_test_test_prefix()\n");
    hemp_debug("my precedence: %d   request precedence: %d\n", type->rprec, precedence);

    HEMP_PREFIX_PRECEDENCE;
    hemp_advance(fragptr);

    block = hemp_fragment_parse_block(fragptr, scope, type->rprec, 1);

    if (! block)
        hemp_fatal("missing block for %s\n", type->start);
    
    hemp_set_expr_fragment(self, block);
    hemp_skip_whitespace(fragptr);

    hemp_debug("parsed test, next element is %s:\n", (*fragptr)->type->name);

    return self;
}


HEMP_VALUE(hemp_element_test_test_value) {
//  hemp_text text = hemp_context_tmp_text(context);
    hemp_todo("hemp_element_test_test_value()\n");
    return HempMissing;
}


HEMP_CLEANUP(hemp_element_test_test_cleanup) {
    hemp_debug_call("hemp_element_test_test_cleanup()\n");
}


/*--------------------------------------------------------------------------
 * expect
 *--------------------------------------------------------------------------*/

HEMP_ELEMENT(hemp_element_test_expect) {
//  element->scanner        = &hemp_element_test_expect_scanner;
    element->cleanup        = &hemp_element_test_expect_cleanup,
    element->token          = &hemp_element_literal_text;
    element->source         = &hemp_element_literal_text;
    element->parse_prefix   = &hemp_element_test_expect_prefix;
    element->flags          = HEMP_BE_SOURCE | HEMP_BE_FIXED;
    return element;
}


HEMP_SCAN_FUNC(hemp_element_test_expect_scanner) {
    hemp_string  src     = *srcptr;

    hemp_debug_call("hemp_element_test_expect_scanner()\n");

    /* walk to the end of line or end of tag */
    do {
        ++src;
    }
    while (
        *src && *src != HEMP_LF && *src != HEMP_CR
    );

    hemp_debug_token("EXPECT", *srcptr, src - *srcptr);

    /* update the source pointer past the text we've consumed */
    *srcptr = src;
    
    /* add a comment element to the list of scanned tokens */
    return hemp_fragments_add_fragment(
        tmpl->fragments, element,
        start, pos, src - start
    );
}


HEMP_PREFIX(hemp_element_test_expect_prefix) {
    hemp_fragment self = *fragptr;
    hemp_element  type = self->type;
    hemp_fragment block;

    hemp_debug("hemp_element_test_expect_prefix()\n");
    hemp_debug("my precedence: %d   request precedence: %d\n", type->rprec, precedence);

    HEMP_PREFIX_PRECEDENCE;

    hemp_advance(fragptr);

    block = hemp_fragment_parse_block(fragptr, scope, type->rprec, 1);

    if (! block)
        hemp_fatal("missing block for %s\n", type->start);
    
    hemp_set_expr_fragment(self, block);
    hemp_skip_whitespace(fragptr);

    return self;
}


HEMP_CLEANUP(hemp_element_test_expect_cleanup) {
    hemp_debug_call("hemp_element_test_expect_cleanup()\n");
    hemp_element_block_cleanup(
        hemp_expr_fragment(fragment)
    );
}
