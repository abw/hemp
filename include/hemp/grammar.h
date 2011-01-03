#ifndef HEMP_GRAMMAR_H
#define HEMP_GRAMMAR_H

#include <hemp/core.h>
#include <hemp/action.h>
#include <hemp/element.h>
#include <hemp/factory.h>
#include <hemp/ptree.h>
#include <hemp/namespace.h>
#include <hemp/document.h>
#include <hemp/type/hash.h>


/*--------------------------------------------------------------------------
 * type definitions
 *--------------------------------------------------------------------------*/

typedef HempGrammar 
(*  hemp_grammar_f) (
    Hemp            hemp,
    HempString      name
);

struct hemp_grammar {
    Hemp            hemp;
    HempString      name;
    HempHash        elements;
    HempPtree       operators;
//  HempHash        keywords;
};

struct hemp_grammars {
    HempString      name;
    hemp_grammar_f  ctor;
};



/*--------------------------------------------------------------------------
 * Factory prototypes
 *--------------------------------------------------------------------------*/

HEMP_FACTORY(hemp_grammar_factory);
HEMP_HASH_ITERATOR(hemp_grammar_cleaner);


/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

HempGrammar
hemp_grammar_new(
    Hemp            hemp,
    HempString      name
);

HempElement
hemp_grammar_new_element(
    HempGrammar     grammar,
    HempString      element,
    HempString      start,
    HempString      end
);

HempElement
hemp_grammar_add_element(
    HempGrammar     grammar,
    HempString      element,
    HempString      start,
    HempString      end,
    HempPrec        lprec,
    HempPrec        rprec
);

HEMP_INLINE HempElement
hemp_grammar_element(
    HempGrammar     grammar,
    HempString      name
);

void
hemp_grammar_free(
    HempGrammar     grammar
);

HempBool
hemp_grammar_free_element(
    HempHash        grammars,
    HempPos         position,
    HempSlot        item
);

HempBool
hemp_grammar_scanner(
    HempGrammar     grammar,
    HempDocument    document
);


#endif /* HEMP_GRAMMAR_H */