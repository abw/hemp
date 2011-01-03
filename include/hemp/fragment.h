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

struct hemp_unary_op {
    HempValue       expr;
    HempValue       args;
};

struct hemp_binary_op {
    HempValue       lhs;
    HempValue       rhs;
};

struct hemp_block_op {
    HempValue       exprs;
    HempValue       args;
};

union hemp_op {
    HempValue       value;
    HempUnaryOp     unary;
    HempBinaryOp    binary;
    HempBlockOp     block;
};

struct hemp_fragment {
    // TODO: clean this up and remove some superfluous items
    HempElement     type;
    HempFragments   fragments;
    HempFragment    next;
    HempFragment    branch;
    HempString      token;
    HempPos         position;
    HempSize        length;
    HempFlags       flags;
    HempOp          op;   // heap allocate to reduce size to single ptr?
};


/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

HempFragment
hemp_fragment_init(
    HempFragment    fragment,
    HempElement     element,
    HempString      token,
    HempPos         position,
    HempSize        length
);

void
hemp_fragment_free(
    HempFragment    fragment
);

HEMP_INLINE HempGrammar
hemp_fragment_grammar(
    HempFragment    fragment
);

HEMP_INLINE HempFragments
hemp_fragment_fragments(
    HempFragment    fragment
);

HEMP_INLINE HempDocument
hemp_fragment_document(
    HempFragment    fragment
);

HEMP_INLINE HempElement
hemp_fragment_grammar_element(
    HempFragment    fragment,
    HempString      name
);

HEMP_INLINE HempFragment
hemp_fragment_new_fragment(
    HempFragment    fragment,
    HempString      typename
);

HempElement
hemp_fragment_retype(
    HempFragment    fragment,
    HempString      typename
);

HempFragment
hemp_fragment_parse(
    HempFragment    fragment, 
    HempScope       scope
);

HempBool
hemp_fragment_dump(
    HempFragment    fragment
);

void hemp_fragment_dump_exprs(
    HempList        exprs
);

void
hemp_fragment_debug(
    HempFragment    fragment
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