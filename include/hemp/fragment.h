#ifndef HEMP_FRAGMENT_H
#define HEMP_FRAGMENT_H

#include <hemp/core.h>
#include <hemp/element.h>
#include <hemp/grammar.h>
#include <hemp/fragments.h>
#include <hemp/context.h>

//#include <hemp/type.h>
//#include <hemp/tag.h>
//#include <hemp/symbol.h>
//#include <hemp/document.h>
//#include <hemp/value.h>
//#include <hemp/macros.h>
//#include <hemp/proto.h>


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
    hemp_element    type;
    hemp_fragments  fragments;
    hemp_fragment   next;
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

hemp_list
hemp_fragment_parse_exprs(
    HEMP_PREFIX_ARGS
);

HEMP_PREFIX(hemp_fragment_parse_block);

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


/*--------------------------------------------------------------------------
 * Scanning and skipping macros
 *--------------------------------------------------------------------------*/

#define hemp_at_eof(fp) (                                       \
    (*fp)->type == HempElementEOF                                \
)

#define hemp_advance(fp) (                                      \
    ((*fp)->next && ! hemp_at_eof(fp))                          \
        ? (*fp = (*fp)->next)                                   \
        : NULL                                                  \
)

#define hemp_skip_while(fp, flag) ({                            \
    while( hemp_has_flag((*fp)->type, flag) )                   \
        if (! hemp_advance(fp))                                 \
            break;                                              \
})

#define HEMP_BE_ANY_DL_WS       HEMP_BE_DELIMITER  | HEMP_BE_WHITESPACE
#define HEMP_BE_ANY_SP_DL_WS    HEMP_BE_SEPARATOR  | HEMP_BE_ANY_DL_WS
#define HEMP_BE_ANY_TR_SP_DL_WS HEMP_BE_TERMINATOR | HEMP_BE_ANY_SP_DL_WS

#define hemp_skip_whitespace(fp)                                \
    hemp_skip_while(fp, HEMP_BE_WHITESPACE)

#define hemp_skip_delimiter(fp)                                 \
    hemp_skip_while(fp, HEMP_BE_ANY_DL_WS)

#define hemp_skip_separator(fp)                                 \
    hemp_skip_while(fp, HEMP_BE_ANY_SP_DL_WS)

#define hemp_skip_terminator(fp)                                \
    hemp_skip_while(fp, HEMP_BE_ANY_TR_SP_DL_WS)


/*--------------------------------------------------------------------------
 * Macros for setting and getting fragment arguments
 *--------------------------------------------------------------------------*/

#define hemp_expr(frag)                                         \
    (frag->args.unary.expr)

#define hemp_expr_args(frag)                                    \
    (frag->args.unary.args)

#define hemp_lhs(frag)                                          \
    (frag->args.binary.lhs)

#define hemp_rhs(frag)                                          \
    (frag->args.binary.rhs)

#define hemp_block_args(frag)                                   \
    (frag->args.block.args)

#define hemp_block_exprs(frag)                                  \
    (frag->args.block.exprs)

#define hemp_block_exprs_list(frag)                             \
    (hemp_val_list(frag->args.block.exprs))

#define hemp_expr_fragment(frag)                                 \
    ((hemp_fragment) hemp_val_obj( hemp_expr(frag) ))

#define hemp_lhs_fragment(frag)                                  \
    ((hemp_fragment) hemp_val_obj( hemp_lhs(frag) ))

#define hemp_rhs_fragment(frag)                                  \
    ((hemp_fragment) hemp_val_obj( hemp_rhs(frag) ))

#define hemp_set_expr(frag, ex)                                 \
    (frag->args.unary.expr = ex)

#define hemp_set_lhs(frag, ex)                                  \
    (frag->args.binary.lhs = ex)

#define hemp_set_rhs(frag, ex)                                  \
    (frag->args.binary.rhs = ex)

#define hemp_set_block_args(frag, ex)                           \
    (frag->args.block.args = ex)

#define hemp_set_block_exprs(frag, ex)                          \
    (frag->args.block.exprs = ex)

#define hemp_set_block_exprs_list(frag, li)                     \
    (frag->args.block.exprs = hemp_list_val(li))

#define hemp_set_expr_fragment(frag, ex)                         \
    (hemp_set_expr( frag, hemp_obj_val((hemp_object) ex) ))

#define hemp_set_lhs_fragment(frag, ex)                          \
    (hemp_set_lhs( frag, hemp_obj_val((hemp_object) ex) ))

#define hemp_set_rhs_fragment(frag, ex)                          \
    (hemp_set_rhs( frag, hemp_obj_val((hemp_object) ex) ))


/*--------------------------------------------------------------------------
 * Macros for invoking parsing methods
 *--------------------------------------------------------------------------*/

#define hemp_parse_method(fp, method, retval, ...) (            \
    (*fp)->type->parse_##method                                 \
        ? (*fp)->type->parse_##method(fp, __VA_ARGS__)          \
        : retval                                                \
)

#define hemp_parse_prefix(fp, sc, pr, fr)                       \
    hemp_parse_method(fp, prefix, NULL, sc, pr, fr)

#define hemp_parse_postfix(fp, sc, pr, fr, lhs)                 \
    hemp_parse_method(fp, postfix, lhs, sc, pr, fr, lhs)

#define hemp_parse_fixed(fp, ...)                               \
    hemp_parse_method(fp, fixed, NULL, __VA_ARGS__)

#define hemp_parse_params(fp, ...)                              \
    hemp_parse_method(fp, params, NULL, __VA_ARGS__)

#define hemp_parse_body(fp, ...)                                \
    hemp_parse_method(fp, body, NULL, __VA_ARGS__)

#define hemp_parse_lhs_expr(fragment) ({                        \
    hemp_fragment _hemp_expr = hemp_parse_prefix(               \
        fragptr, scope, fragment->type->lprec, 1                \
    );                                                          \
    if (! _hemp_expr)                                           \
        HEMP_THROW_NOEXPR(fragment);                            \
    hemp_set_lhs_fragment(fragment, _hemp_expr);                \
    _hemp_expr;                                                 \
})

#define hemp_parse_rhs_body(fragment, ...) ({                   \
    hemp_fragment _hemp_expr = hemp_parse_body(                 \
        fragptr, scope, fragment->type->rprec, 1                \
    );                                                          \
    if (! _hemp_expr)                                           \
        HEMP_THROW_NOBODY(fragment);                            \
    hemp_set_rhs_fragment(fragment, _hemp_expr);                \
    _hemp_expr;                                                 \
})





#endif /* HEMP_FRAGMENT_H */