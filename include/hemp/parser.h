#ifndef HEMP_PARSER_H
#define HEMP_PARSER_H

#include <hemp/fragment.h>

/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

HempList
hemp_parse_exprs(
    HEMP_PREFIX_ARGS
);

HempList
hemp_parse_pairs(
    HEMP_PREFIX_ARGS
);

HEMP_PREFIX(hemp_parse_block);


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

#define hemp_expr_fragment(frag)                                \
    (hemp_val_frag( hemp_expr(frag)) )

#define hemp_expr_args_fragment(frag)                           \
    (hemp_val_frag( hemp_expr_args(frag) ))

#define hemp_lhs_fragment(frag)                                 \
    (hemp_val_frag( hemp_lhs(frag) ))

#define hemp_rhs_fragment(frag)                                 \
    (hemp_val_frag( hemp_rhs(frag) ))

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

#define hemp_set_expr_fragment(frag, ex)                        \
    (hemp_set_expr( frag, hemp_frag_val(ex) ))

#define hemp_set_expr_args_fragment(frag, a)                    \
    (hemp_set_expr_args( frag, hemp_frag_val(a) ))

#define hemp_set_lhs_fragment(frag, ex)                         \
    (hemp_set_lhs( frag, hemp_frag_val(ex) ))

#define hemp_set_rhs_fragment(frag, ex)                         \
    (hemp_set_rhs( frag, hemp_frag_val(ex) ))


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

#define hemp_parse_branch(fp, ...)                              \
    hemp_parse_method(fp, branch, NULL, __VA_ARGS__)

#define hemp_parse_fixed(fp, ...)                               \
    hemp_parse_method(fp, fixed, NULL, __VA_ARGS__)

#define hemp_parse_params(fp, ...)                              \
    hemp_parse_method(fp, params, NULL, __VA_ARGS__)

#define hemp_parse_pair(fp, ...)                                \
    hemp_parse_method(fp, pair, NULL, __VA_ARGS__)

#define hemp_parse_body(fp, ...)                                \
    hemp_parse_method(fp, body, NULL, __VA_ARGS__)


/*--------------------------------------------------------------------------
 * More syntactic sugar for parsing something and stashing it in the fragment
 *--------------------------------------------------------------------------*/

#define hemp_parse_lhs_expr(fragment, precedence) ({            \
    HempFragment _hemp_expr = hemp_parse_prefix(               \
        fragptr, scope, precedence, 1                           \
    );                                                          \
    if (! _hemp_expr)                                           \
        HEMP_THROW_NOEXPR(fragment);                            \
    hemp_set_lhs_fragment(fragment, _hemp_expr);                \
    _hemp_expr;                                                 \
})

#define hemp_parse_lhs_expr_lprec(fragment)                     \
    hemp_parse_lhs_expr(fragment, fragment->type->lprec)

#define hemp_parse_lhs_expr_rprec(fragment)                     \
    hemp_parse_lhs_expr(fragment, fragment->type->rprec)


#define hemp_parse_rhs_expr(fragment, precedence) ({            \
    HempFragment _hemp_expr = hemp_parse_prefix(               \
        fragptr, scope, precedence, 1                           \
    );                                                          \
    if (! _hemp_expr)                                           \
        HEMP_THROW_NOEXPR(fragment);                            \
    hemp_set_rhs_fragment(fragment, _hemp_expr);                \
    _hemp_expr;                                                 \
})

#define hemp_parse_rhs_expr_lprec(fragment)                     \
    hemp_parse_rhs_expr(fragment, fragment->type->lprec)

#define hemp_parse_rhs_expr_rprec(fragment)                     \
    hemp_parse_rhs_expr(fragment, fragment->type->rprec)


#define hemp_parse_lhs_pairs(fragment, precedence) ({           \
    HempList _hemp_pairs = hemp_parse_pairs(                   \
        fragptr, scope, precedence, 1                           \
    );                                                          \
    hemp_set_lhs(fragment, hemp_list_val(_hemp_pairs));         \
    _hemp_pairs;                                                \
})

#define hemp_parse_lhs_pairs_lprec(fragment)                    \
    hemp_parse_lhs_pairs(fragment, fragment->type->lprec)

#define hemp_parse_lhs_pairs_rprec(fragment)                    \
    hemp_parse_lhs_pairs(fragment, fragment->type->rprec)


/* TODO: check that rhs body always uses rprec */

#define hemp_parse_rhs_body(fragment) ({                        \
    HempFragment _hemp_expr = hemp_parse_body(                 \
        fragptr, scope, fragment->type->rprec, 0                \
    );                                                          \
    if (! _hemp_expr)                                           \
        HEMP_THROW_NOBODY(fragment);                            \
    hemp_set_rhs_fragment(fragment, _hemp_expr);                \
    _hemp_expr;                                                 \
})

#define hemp_parse_body_terminator(fragment, block) ({          \
    if (hemp_not_flag(block, HEMP_BE_TERMINATED)) {             \
        if (hemp_element_terminator_matches(                    \
            *fragptr, fragment->type->end)                      \
        ) {                                                     \
            hemp_advance(fragptr);                              \
        }                                                       \
        else {                                                  \
            HEMP_THROW_NOEND(fragment);                         \
        }                                                       \
    }                                                           \
})

#define hemp_parse_rhs_body_terminated(fragment) ({             \
    HempFragment _hemp_body = hemp_parse_rhs_body(fragment);   \
    hemp_parse_body_terminator(fragment, _hemp_body);           \
    _hemp_body;                                                 \
})


#endif /* HEMP_PARSER_H */