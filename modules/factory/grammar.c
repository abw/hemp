#include <hemp.h>
#include <hemp/debug.h>


HEMP_FACTORY(hemp_grammar_factory);
HEMP_HASH_ITERATOR(hemp_grammar_cleaner);


/*--------------------------------------------------------------------------
 * Binder function registers the grammar factory with the hemp instance
 *--------------------------------------------------------------------------*/

HempBool
hemp_module_binder(
    HempModule  module,
    Hemp        hemp
) {
    hemp_register_factory( hemp, HEMP_FACTORY_GRAMMAR, &hemp_grammar_factory );
    hemp_debug_msg("loaded grammar factory module\n");
    return HEMP_TRUE;
}


/*--------------------------------------------------------------------------
 * Factory functions for loading grammars
 *--------------------------------------------------------------------------*/

HEMP_FACTORY(hemp_grammar_factory) {
    hemp_debug_init("instantiating grammar factory\n");
    HempFactory factory = hemp_factory_new(hemp, name);
    factory->cleaner     = &hemp_grammar_cleaner;
    factory->autoload    = NULL;        // TODO: load language
    return factory;
}


HEMP_HASH_ITERATOR(hemp_grammar_cleaner) {
    HempGrammar grammar = (HempGrammar) hemp_val_ptr(item->value);
    hemp_debug_init("cleaning grammar: %s\n", grammar->name);
    hemp_grammar_free(grammar);
    return HEMP_TRUE;
}


