#ifndef HEMP_LANGUAGE_TEST_H
#define HEMP_LANGUAGE_TEST_H

#include <hemp.h>

#define HEMP_TEST          "test"
#define HEMP_TEST_VERSION   0.1

HEMP_LANGUAGE_FUNC(hemp_language_test);
HEMP_DIALECT_FUNC(hemp_dialect_test);
HEMP_GRAMMAR_FUNC(hemp_grammar_test);
HEMP_SYMBOLS_FUNC(hemp_element_test_symbols);
HEMP_SYMBOL_FUNC(hemp_element_test_test_symbol);
HEMP_SYMBOL_FUNC(hemp_element_test_expect_symbol);
HEMP_SCAN_FUNC(hemp_element_test_test_scanner);
HEMP_PREFIX_FUNC(hemp_element_test_test_prefix);

HEMP_SCAN_FUNC(hemp_element_test_expect_scanner);
HEMP_PREFIX_FUNC(hemp_element_test_expect_prefix);


hemp_template_p hemp_dialect_test_prepare(hemp_template_p tmpl);
void hemp_dialect_test_cleanup(hemp_template_p tmpl);

#endif /* HEMP_LANGUAGE_TEST_H */
