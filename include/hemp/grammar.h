#ifndef HEMP_GRAMMAR_H
#define HEMP_GRAMMAR_H

#include <hemp/core.h>
#include <hemp/hash.h>
#include <hemp/ptree.h>


/*--------------------------------------------------------------------------
 * data structures
 *--------------------------------------------------------------------------*/

struct hemp_grammar_s {
    hemp_p          hemp;
    hemp_cstr_p     name;
    hemp_hash_p     keywords;
    hemp_ptree_p    operators;
};


hemp_grammar_p
    hemp_grammar_init(
        hemp_p      hemp,
        hemp_cstr_p name
    );

void
    hemp_grammar_free(
        hemp_grammar_p grammar
    );


#endif /* HEMP_GRAMMAR_H */