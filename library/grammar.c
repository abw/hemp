#include <hemp/grammar.h>


hemp_grammar_p
hemp_grammar_init(
    hemp_p      hemp,
    hemp_cstr_p name
) {
    hemp_grammar_p grammar = (hemp_grammar_p) hemp_mem_alloc(
        sizeof(struct hemp_grammar_s)
    );

    if (! grammar)
        hemp_mem_fail("grammar");

    grammar->hemp      = hemp;
    grammar->keywords  = hemp_hash_init();
    grammar->operators = hemp_ptree_init(HEMP_OPERATORS_SIZE);
    grammar->name      = hemp_cstr_copy(name);

    if (! grammar->name)
        hemp_mem_fail("grammar name");

    return grammar;
}


void
hemp_grammar_free(
    hemp_grammar_p grammar
) {
    hemp_mem_free(grammar->name);
    hemp_hash_free(grammar->keywords);
    hemp_ptree_free(grammar->operators);
    hemp_mem_free(grammar);
}
