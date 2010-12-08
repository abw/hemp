#ifndef HEMP_LANGUAGE_TEST_H
#define HEMP_LANGUAGE_TEST_H

#include <hemp.h>

#define HEMP_TEST          "test"
#define HEMP_TEST_VERSION   0.1

HEMP_LANGUAGE(hemp_language_test);

HEMP_DIALECT(hemp_dialect_test);

HEMP_GRAMMAR(hemp_grammar_test);

HEMP_ELEMENTS(hemp_element_test_symbols);
HEMP_ELEMENT(hemp_element_test_test_symbol);
HEMP_ELEMENT(hemp_element_test_expect_symbol);
HEMP_SCAN_FUNC(hemp_element_test_test_scanner);
HEMP_PREFIX(hemp_element_test_test_prefix);
HEMP_VALUE(hemp_element_test_test_value);
void hemp_element_test_test_clean(hemp_element);

HEMP_SCAN_FUNC(hemp_element_test_expect_scanner);
HEMP_PREFIX(hemp_element_test_expect_prefix);
HEMP_VALUE(hemp_element_test_expect_value);
void hemp_element_test_expect_clean(hemp_element);


hemp_template hemp_dialect_test_prepare(hemp_template tmpl);
void hemp_dialect_test_cleanup(hemp_template tmpl);

#endif /* HEMP_LANGUAGE_TEST_H */
