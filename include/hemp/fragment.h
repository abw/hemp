#ifndef HEMP_FRAGMENT_H
#define HEMP_FRAGMENT_H

#include <hemp/core.h>
#include <hemp/element.h>
#include <hemp/grammar.h>
#include <hemp/fragments.h>
#include <hemp/context.h>


/*--------------------------------------------------------------------------
 * type definitions
 *--------------------------------------------------------------------------*/

typedef struct {
    HempValue      expr;
    HempValue      args;
}   hemp_unop;

typedef struct {
    HempValue      lhs;
    HempValue      rhs;
}   hemp_binop;

typedef struct {
    HempValue      exprs;
    HempValue      args;
}   hemp_blkop;

typedef union {
    HempValue      value;
    hemp_unop       unary;
    hemp_binop      binary;
    hemp_blkop      block;
}   hemp_opargs;

struct hemp_fragment {
    // TODO: clean this up and remove some superfluous items
    HempElement    type;
    HempFragments  fragments;
    HempFragment   next;
    HempFragment   branch;
    HempString     token;
    HempPos        position;
    HempSize       length;
    HempFlags      flags;
    hemp_opargs     args;   // heap allocate to reduce size to single ptr?
};


/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

HempFragment
hemp_fragment_init(
    HempFragment   fragment,
    HempElement    element,
    HempString     token,
    HempPos        position,
    HempSize       length
);

void
hemp_fragment_free(
    HempFragment   fragment
);

HEMP_INLINE HempGrammar
hemp_fragment_grammar(
    HempFragment fragment
);

HEMP_INLINE HempFragments
hemp_fragment_fragments(
    HempFragment fragment
);

HEMP_INLINE HempDocument
hemp_fragment_document(
    HempFragment fragment
);

HEMP_INLINE HempElement
hemp_fragment_grammar_element(
    HempFragment fragment,
    HempString   name
);

HEMP_INLINE HempFragment
hemp_fragment_new_fragment(
    HempFragment   fragment,
    HempString     typename
);

HempElement
hemp_fragment_retype(
    HempFragment   fragment,
    HempString     typename
);

HempFragment
hemp_fragment_parse(
    HempFragment   fragment, 
    HempScope      scope
);

HempBool
hemp_fragment_dump(
    HempFragment f
);

void hemp_fragment_dump_exprs(
    HempList exprs
);

void
hemp_fragment_debug(
    HempFragment f
);


/*--------------------------------------------------------------------------
 * general purpose macros
 *--------------------------------------------------------------------------*/

#define hemp_fragment_new(element, token, position, length)     \
    hemp_fragment_init(NULL, element, token, position, length)

#define hemp_fragment_cleanup(f)                                \
    f->type->cleanup                                            \
        ? f->type->cleanup(f)                                   \
        : NULL;


#endif /* HEMP_FRAGMENT_H */