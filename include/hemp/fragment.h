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
    hemp_value      expr;
    hemp_value      args;
}   hemp_unop;

typedef struct {
    hemp_value      lhs;
    hemp_value      rhs;
}   hemp_binop;

typedef struct {
    hemp_value      exprs;
    hemp_value      args;
}   hemp_blkop;

typedef union {
    hemp_value      value;
    hemp_unop       unary;
    hemp_binop      binary;
    hemp_blkop      block;
}   hemp_opargs;

struct hemp_fragment {
    // TODO: clean this up and remove some superfluous items
    hemp_element    type;
    hemp_fragments  fragments;
    hemp_fragment   next;
    hemp_fragment   branch;
    hemp_string     token;
    hemp_pos        position;
    hemp_size       length;
    hemp_flags      flags;
    hemp_opargs     args;   // heap allocate to reduce size to single ptr?
};


/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

hemp_fragment
hemp_fragment_init(
    hemp_fragment   fragment,
    hemp_element    element,
    hemp_string     token,
    hemp_pos        position,
    hemp_size       length
);

void
hemp_fragment_free(
    hemp_fragment   fragment
);

HEMP_INLINE hemp_grammar
hemp_fragment_grammar(
    hemp_fragment fragment
);

HEMP_INLINE hemp_fragments
hemp_fragment_fragments(
    hemp_fragment fragment
);

HEMP_INLINE hemp_document
hemp_fragment_document(
    hemp_fragment fragment
);

HEMP_INLINE hemp_element
hemp_fragment_grammar_element(
    hemp_fragment fragment,
    hemp_string   name
);

HEMP_INLINE hemp_fragment
hemp_fragment_new_fragment(
    hemp_fragment   fragment,
    hemp_string     typename
);

hemp_element
hemp_fragment_retype(
    hemp_fragment   fragment,
    hemp_string     typename
);

hemp_fragment
hemp_fragment_parse(
    hemp_fragment   fragment, 
    hemp_scope      scope
);

hemp_bool
hemp_fragment_dump(
    hemp_fragment f
);

void hemp_fragment_dump_exprs(
    hemp_list exprs
);

void
hemp_fragment_debug(
    hemp_fragment f
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