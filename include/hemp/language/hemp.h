#ifndef HEMP_LANGUAGE_HEMP_H
#define HEMP_LANGUAGE_HEMP_H

#include <hemp.h>

#define HEMP_LANGUAGE_HEMP_VERSION 0.01

HEMP_LANGUAGE(hemp_language_hemp_new);

HEMP_SYMBOLS(hemp_element_assign_symbols);
HEMP_SYMBOLS(hemp_element_bracket_symbols);
HEMP_SYMBOLS(hemp_element_boolean_symbols);
HEMP_SYMBOLS(hemp_element_number_symbols);
HEMP_SYMBOLS(hemp_element_text_symbols);

HEMP_GRAMMAR(hemp_grammar_hemp_alpha);
HEMP_GRAMMAR(hemp_grammar_hemp_bravo);
HEMP_GRAMMAR(hemp_grammar_hemp_charlie);

void hemp_grammar_add_hemp_alpha(hemp_grammar);
void hemp_grammar_add_hemp_bravo(hemp_grammar);
void hemp_grammar_add_hemp_charlie(hemp_grammar);


#endif /* HEMP_LANGUAGE_HEMP_H */
