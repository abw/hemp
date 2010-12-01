#ifndef HEMP_LANGUAGE_TT3_H
#define HEMP_LANGUAGE_TT3_H

#include <hemp.h>

#define HEMP_LANGUAGE_TT3_VERSION 0.2

HEMP_LANGUAGE(hemp_language_tt3);

HEMP_DIALECT(hemp_dialect_tt3);

HEMP_GRAMMAR(hemp_grammar_tt3_core);
HEMP_GRAMMAR(hemp_grammar_tt3_command);
HEMP_GRAMMAR(hemp_grammar_tt3_control);

HEMP_SYMBOLS(hemp_element_tt3_command_symbols);

HEMP_SYMBOL(hemp_element_tt3_if_symbol);
HEMP_SYMBOL(hemp_element_tt3_TODO_symbol);


#endif /* HEMP_LANGUAGE_TT3_H */
