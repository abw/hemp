#include <hemp/grammar.h>
#include <hemp/namespace.h>


hemp_grammar
hemp_grammar_new(
    hemp_hemp   hemp,
    hemp_string name
) {
    hemp_grammar grammar;
    HEMP_ALLOCATE(grammar);

    grammar->hemp      = hemp;
    grammar->symbols   = hemp_hash_init();
//  grammar->keywords  = hemp_hash_init();
    grammar->operators = hemp_ptree_new(HEMP_OPERATORS_SIZE);
    grammar->name      = hemp_string_clone(name, "grammar name");

    return grammar;
}


hemp_symbol
hemp_grammar_new_symbol(
    hemp_grammar    grammar,
    hemp_string     etype,
    hemp_string     start,
    hemp_string     end
) {
//  hemp_debug_call(
//      "new [%s => %s] symbol\n", 
//      token, etype
//  );

    hemp_action constructor = hemp_factory_constructor(
        grammar->hemp->element, etype
    );
    if (! constructor)
        hemp_throw(grammar->hemp, HEMP_ERROR_INVALID, "element", etype);

    hemp_symbol symbol = hemp_symbol_new(
        etype, start, end
    );

    symbol = (hemp_symbol) hemp_action_run(
        constructor, symbol 
    );

    if (! symbol)
        hemp_throw(grammar->hemp, HEMP_ERROR_INVALID, "element", etype);

    return symbol;
}


hemp_symbol
hemp_grammar_add_symbol(
    hemp_grammar    grammar,
    hemp_string     etype,
    hemp_string     start,
    hemp_string     end,
    hemp_oprec      lprec,
    hemp_oprec      rprec
) {
//  hemp_debug_call(
//      "adding [%s => %s] symbol to %s grammar [%d|%d]\n", 
//      token, etype, grammar->name, lprec, rprec
//  );

    /* Any symbol with a start token goes into the operator prefix tree 
     * (a modified ternary search tree) which allows the scanner to easily 
     * and efficiently match longest tokens, e.g. so that '++' is 
     * recognised as one single token, not two instances of '+'
     *
     * Symbols without start tokens can't be matched directly by a parser,
     * but may be synthesised into an element tree by other elements as 
     * part of the parsing process (e.g. elements that create a hemp.block
     * to store their body content).  We use the fully-qualified element type
     * name (e.g. hemp.block) as a hash index for retrieving the symbol table
     * entry.
     *
     * Either way, the symbol or element type name must be unique for a 
     * grammar.
     */
    hemp_string name = start ? start : etype;

    if (hemp_hash_fetch_pointer(grammar->symbols, name))
        hemp_throw(grammar->hemp, HEMP_ERROR_DUPLICATE, "symbol", name);

    hemp_symbol symbol = hemp_grammar_new_symbol(
        grammar, etype, start, end
    );

    symbol->lprec   = lprec;
    symbol->rprec   = rprec;
    symbol->grammar = grammar;

    if (start) {
        hemp_ptree_store(
            grammar->operators, start, (hemp_memory) symbol
        );
    }

    hemp_hash_store_pointer(grammar->symbols, name, symbol);

    return symbol;
}


HEMP_INLINE hemp_symbol
hemp_grammar_symbol(
    hemp_grammar  grammar,
    hemp_string      name
) {
    hemp_symbol symbol = (hemp_symbol) hemp_hash_fetch_pointer(
        grammar->symbols, name
    );

    if (! symbol) {
        hemp_fatal(
            "Invalid element type specified.  %s symbol not found in %s grammar",
            name, grammar->name
        );
}

    return symbol;
}


void
hemp_grammar_free(
    hemp_grammar grammar
) {
    hemp_hash_each(grammar->symbols, &hemp_grammar_free_symbol);
    hemp_hash_free(grammar->symbols);
    hemp_ptree_free(grammar->operators);
    hemp_mem_free(grammar->name);
    hemp_mem_free(grammar);
}


hemp_bool
hemp_grammar_free_symbol(
    hemp_hash     grammars,
    hemp_pos      position,
    hemp_slot     item
) {
    hemp_symbol_free( (hemp_symbol) hemp_val_ptr(item->value) );
    return HEMP_TRUE;
}
