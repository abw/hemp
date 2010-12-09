#ifndef HEMP_LANGUAGE_TEST_H
#define HEMP_LANGUAGE_TEST_H

#include <hemp.h>

#define HEMP_TEST          "test"
#define HEMP_TEST_VERSION   0.1

HEMP_LANGUAGE(hemp_language_test);
HEMP_DIALECT(hemp_dialect_test);
HEMP_GRAMMAR(hemp_grammar_test);

HEMP_ELEMENTS(hemp_element_test_elements);
HEMP_ELEMENT(hemp_element_test_test);
HEMP_ELEMENT(hemp_element_test_expect);

HEMP_SCANNER(hemp_element_test_test_scanner);
HEMP_PREFIX(hemp_element_test_test_prefix);
HEMP_VALUE(hemp_element_test_test_value);
HEMP_CLEANUP(hemp_element_test_test_cleanup);

HEMP_SCANNER(hemp_element_test_expect_scanner);
HEMP_PREFIX(hemp_element_test_expect_prefix);
HEMP_VALUE(hemp_element_test_expect_value);
HEMP_CLEANUP(hemp_element_test_expect_cleanup);

hemp_template hemp_dialect_test_prepare(hemp_template tmpl);
void hemp_dialect_test_cleanup(hemp_template tmpl);

#endif /* HEMP_LANGUAGE_TEST_H */
