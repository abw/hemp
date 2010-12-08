#ifndef HEMP_LANGUAGE_TT3_H
#define HEMP_LANGUAGE_TT3_H

#include <hemp.h>

#define HEMP_LANGUAGE_TT3_VERSION 0.2

HEMP_LANGUAGE(hemp_language_tt3);

HEMP_DIALECT(hemp_dialect_tt3);
HEMP_DIALECT(hemp_dialect_tt3_unplugged);

HEMP_GRAMMAR(hemp_grammar_tt3_core);
HEMP_GRAMMAR(hemp_grammar_tt3_command);
HEMP_GRAMMAR(hemp_grammar_tt3_control);

HEMP_ELEMENTS(hemp_element_tt3_command_elements);
HEMP_ELEMENT(hemp_element_tt3_if);
HEMP_ELEMENT(hemp_element_tt3_TODO);


hemp_template   hemp_dialect_tt3_prepare(hemp_template template);
void            hemp_dialect_tt3_cleanup(hemp_template template);

hemp_template   hemp_dialect_tt3_unplugged_prepare(hemp_template template);
void            hemp_dialect_tt3_unplugged_cleanup(hemp_template template);


#endif /* HEMP_LANGUAGE_TT3_H */
