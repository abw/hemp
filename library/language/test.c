#include <hemp/language/test.h>


static struct hemp_symbols_s hemp_symbols_test[] = {
    { "test.test",   &hemp_element_test_test_symbol     },
    { "test.expect", &hemp_element_test_expect_symbol   },
    { NULL, NULL },
};


HEMP_LANGUAGE_FUNC(hemp_language_test) {
    hemp_debug("hemp_language_test(%p, %s)\n", hemp, name);

    hemp_language_p language = hemp_language_init(
        hemp, name, HEMP_TEST_VERSION
    );
    
    /* register handlers for symbols */
    HEMP_ELEMENTS(hemp_symbols_test);

    /* register grammars */
    HEMP_GRAMMAR(HEMP_TEST, &hemp_grammar_test);

    /* register dialects */
    HEMP_DIALECT(HEMP_TEST, &hemp_dialect_test);

    return language;
}


/*--------------------------------------------------------------------------
 * dialect
 *--------------------------------------------------------------------------*/

HEMP_DIALECT_FUNC(hemp_dialect_test) {
    hemp_dialect_p dialect = hemp_dialect_init(hemp, name);
    
    dialect->prepare = &hemp_dialect_test_prepare;
    dialect->scanner = &hemp_scan_text;
    dialect->cleanup = &hemp_dialect_test_cleanup;
    
    return dialect;
}


hemp_template_p
hemp_dialect_test_prepare(
    hemp_template_p tmpl
) {
    hemp_debug("hemp_dialect_test_prepare(%p)\n", tmpl);

    hemp_p         hemp    = tmpl->dialect->hemp;
    hemp_tagset_p  tagset  = tmpl->tagset;
    hemp_grammar_p grammar = hemp_grammar(hemp, HEMP_TEST);

    hemp_tagset_add_tag(
        tagset, 
        hemp_tag_init(
            HEMP_TEST,
            HEMP_OUTLINE_TAG,
            "--", "NULL",
            &hemp_outline_tag_scanner,
            grammar
        )
    );

    return tmpl;
}


void
hemp_dialect_test_cleanup(
    hemp_template_p tmpl
) {
    hemp_debug_call("hemp_dialect_test_cleanup(%p)\n", tmpl);
}



/*--------------------------------------------------------------------------
 * grammar
 *--------------------------------------------------------------------------*/

HEMP_GRAMMAR_FUNC(hemp_grammar_test) {
    hemp_debug("hemp_grammar_test(%p, %s)\n", hemp, name);
    hemp_grammar_p grammar = hemp_grammar_hemp_alpha(hemp, name);
    HEMP_BLOCKOP("test.test",   "test",   11);
    HEMP_BLOCKOP("test.expect", "expect", 11);
    return grammar;
}


/*--------------------------------------------------------------------------
 * elements
 *--------------------------------------------------------------------------*/

HEMP_SYMBOL_FUNC(hemp_element_test_test_symbol) {
    hemp_debug("test.test symbol: %s\n", symbol->name);
    symbol->scanner    = &hemp_element_test_test_scanner;
    symbol->cleanup    = &hemp_element_test_test_clean,
    symbol->value      = &hemp_element_test_test_value,
    symbol->text       = &hemp_element_value_text,
    symbol->token      = &hemp_element_literal_token;
    symbol->source     = &hemp_element_literal_source;
    symbol->prefix     = &hemp_element_test_test_prefix;
    symbol->flags      = HEMP_BE_SOURCE | HEMP_BE_FIXED;
    return symbol;
}


HEMP_SCAN_FUNC(hemp_element_test_test_scanner) {
    hemp_str_p  src     = *srcptr;
    hemp_str_p  tag_end = tag->end;
    hemp_size_t tag_len = strlen(tag->end);

    hemp_debug_call("hemp_element_test_test_scanner()\n");

    /* TODO: add to_tag_end() method to scan all text, accounting for
     * inline/outline tags
     */

    /* walk to the end of line or end of tag */
    src = tag->to_end_of_line(tag, src);

    hemp_debug_token("TEST", *srcptr, src - *srcptr);

    /* update the source pointer past the text we've consumed */
    *srcptr = src;
    
    /* add a comment element to the list of scanned tokens */
    return hemp_elements_append(
        tmpl->elements, symbol,
        start, pos, src - start
    );
}


HEMP_PREFIX_FUNC(hemp_element_test_test_prefix) {
    hemp_element_p self = *elemptr;
    hemp_symbol_p  type = self->type;
    hemp_element_p block;

    hemp_debug("hemp_element_test_test_prefix()\n");
    hemp_debug("my precedence: %d   request precedence: %d\n", type->rprec, precedence);

    HEMP_PREFIX_PRECEDENCE;
    hemp_go_next(elemptr);

    block = hemp_element_parse_block(elemptr, scope, type->rprec, 1);

    if (! block)
        hemp_fatal("missing block for %s\n", type->start);
    
    hemp_set_expr_element(self, block);
    hemp_skip_whitespace(elemptr);

    hemp_debug("parsed test, next element is %s:\n", (*elemptr)->type->name);

    return self;
}


HEMP_VALUE_FUNC(hemp_element_test_test_value) {
    hemp_text_p text = hemp_context_tmp_text(context);
    hemp_todo("hemp_element_test_test_value()\n");
}


void
hemp_element_test_test_clean(
    hemp_element_p element
) {
    hemp_debug_call("hemp_element_test_test_clean()\n");
    hemp_element_block_clean(
        hemp_expr_element(element)
    );
}


/*--------------------------------------------------------------------------
 * expect
 *--------------------------------------------------------------------------*/

hemp_symbol_p
hemp_element_test_expect_symbol(
    hemp_p        hemp,
    hemp_symbol_p symbol
) {
    hemp_debug("test.test symbol: %s\n", symbol->name);
    symbol->scanner    = &hemp_element_test_expect_scanner;
    symbol->cleanup    = &hemp_element_test_expect_clean,
    symbol->token      = &hemp_element_literal_token;
    symbol->source     = &hemp_element_literal_source;
    symbol->prefix     = &hemp_element_test_expect_prefix;
    symbol->flags      = HEMP_BE_SOURCE | HEMP_BE_FIXED;
    return symbol;
}


HEMP_SCAN_FUNC(hemp_element_test_expect_scanner) {
    hemp_str_p  src     = *srcptr;
    hemp_str_p  tag_end = tag->end;
    hemp_size_t tag_len = strlen(tag->end);

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
    return hemp_elements_append(
        tmpl->elements, symbol,
        start, pos, src - start
    );
}


HEMP_PREFIX_FUNC(hemp_element_test_expect_prefix) {
    hemp_element_p self = *elemptr;
    hemp_symbol_p  type = self->type;
    hemp_element_p block;

    hemp_debug("hemp_element_test_expect_prefix()\n");
    hemp_debug("my precedence: %d   request precedence: %d\n", type->rprec, precedence);

    HEMP_PREFIX_PRECEDENCE;

    hemp_go_next(elemptr);

    block = hemp_element_parse_block(elemptr, scope, type->rprec, 1);

    if (! block)
        hemp_fatal("missing block for %s\n", type->start);
    
    hemp_set_expr_element(self, block);
    hemp_skip_whitespace(elemptr);

    return self;
}


void
hemp_element_test_expect_clean(
    hemp_element_p element
) {
    hemp_debug_call("hemp_element_test_expect_clean()\n");
    hemp_element_block_clean(
        hemp_expr_element(element)
    );
}
