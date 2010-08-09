#ifndef HEMP_GRAMMAR_H
#define HEMP_GRAMMAR_H

#include <hemp/core.h>
#include <hemp/hash.h>
#include <hemp/ptree.h>
#include <hemp/element.h>
#include <hemp/action.h>
#include <hemp/factory.h>
#include <hemp/symbol.h>


/*
    commonly used elements:
        - text, space, comment, padding
        - tag_start, tag_end
        - number, integer
        - word, squote, dquote
*/
/*--------------------------------------------------------------------------
 * data structures
 *--------------------------------------------------------------------------*/

struct hemp_grammar_s {
    hemp_p          hemp;
    hemp_cstr_p     name;
    hemp_hash_p     symbols;
    hemp_hash_p     keywords;
    hemp_ptree_p    operators;
};


hemp_grammar_p
    hemp_grammar_init(
        hemp_p      hemp,
        hemp_cstr_p name
    );

hemp_symbol_p
    hemp_grammar_new_symbol(
        hemp_grammar_p grammar,
        hemp_cstr_p    element,
        hemp_cstr_p    start,
        hemp_cstr_p    end
    );

hemp_symbol_p
    hemp_grammar_add_symbol(
        hemp_grammar_p grammar,
        hemp_cstr_p    element,
        hemp_cstr_p    start,
        hemp_cstr_p    end,
        hemp_prec_t    lprec,
        hemp_prec_t    rprec
    );

void
    hemp_grammar_free(
        hemp_grammar_p grammar
    );

hemp_bool_t
hemp_grammar_free_symbol(
    hemp_hash_p         grammars,
    hemp_pos_t          position,
    hemp_hash_item_p    item
);


#endif /* HEMP_GRAMMAR_H */