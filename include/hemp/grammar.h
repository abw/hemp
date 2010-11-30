#ifndef HEMP_GRAMMAR_H
#define HEMP_GRAMMAR_H

#include <hemp/core.h>
#include <hemp/ptree.h>
#include <hemp/element.h>
#include <hemp/action.h>
#include <hemp/factory.h>
#include <hemp/symbol.h>
#include <hemp/type/hash.h>


/*--------------------------------------------------------------------------
 * data structures
 *--------------------------------------------------------------------------*/

typedef hemp_grammar 
(*  hemp_grammar_f) (
    hemp_hemp       hemp,
    hemp_string     name
);

struct hemp_grammar {
    hemp_hemp       hemp;
    hemp_string     name;
    hemp_hash       symbols;
    hemp_hash       keywords;
    hemp_ptree      operators;
};

struct hemp_grammars {
    hemp_string     name;
    hemp_grammar_f  ctor;
};


/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

hemp_grammar
hemp_grammar_new(
    hemp_hemp       hemp,
    hemp_string     name
);

hemp_symbol
hemp_grammar_new_symbol(
    hemp_grammar    grammar,
    hemp_string     element,
    hemp_string     start,
    hemp_string     end
);

hemp_symbol
hemp_grammar_add_symbol(
    hemp_grammar    grammar,
    hemp_string     element,
    hemp_string     start,
    hemp_string     end,
    hemp_oprec      lprec,
    hemp_oprec      rprec
);

HEMP_INLINE hemp_symbol
hemp_grammar_symbol(
    hemp_grammar    grammar,
    hemp_string     name
);

void
hemp_grammar_free(
    hemp_grammar    grammar
);

hemp_bool
hemp_grammar_free_symbol(
    hemp_hash       grammars,
    hemp_pos        position,
    hemp_slot       item
);


#endif /* HEMP_GRAMMAR_H */