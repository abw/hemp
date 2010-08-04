#ifndef HEMP_ELEMENT_H
#define HEMP_ELEMENT_H

#include <hemp/type.h>
#include <hemp/text.h>


/*--------------------------------------------------------------------------
 * data structures
 *--------------------------------------------------------------------------*/

struct hemp_etype_s {
    HEMP_TYPE_BASE
    hemp_flags_t    flags;
    hemp_clean_f    cleanup;
    hemp_skip_f     skip_space;
    hemp_skip_f     skip_delimiter;
    hemp_skip_f     skip_separator;
    hemp_parse_f    parse_expr;
    hemp_text_f     text;

    hemp_cstr_p     (*number)();
};

struct hemp_unary_s {
    hemp_element_p  expr;
    // TODO: args
};

struct hemp_binary_s {
    hemp_element_p  lhs;
    hemp_element_p  rhs;
};

struct hemp_block_s {
    hemp_list_p     exprs;
};

union hemp_evalue_u {
    hemp_cstr_p         text;
    hemp_int_t          integer;
    hemp_num_t          number;
    hemp_unary_t        unary;
    hemp_binary_t       binary;
    hemp_block_t        block;
};

struct hemp_element_s {
    hemp_etype_p    type;
    hemp_element_p  next;
    hemp_cstr_p     token;
    hemp_pos_t      position;
    hemp_size_t     length;
    hemp_flags_t    flags;
    hemp_evalue_t   value;
};


/*--------------------------------------------------------------------------
 * external static element types
 *--------------------------------------------------------------------------*/

extern hemp_etype_p HempElementSpace;
extern hemp_etype_p HempElementComment;
extern hemp_etype_p HempElementTagStart;
extern hemp_etype_p HempElementTagEnd;
extern hemp_etype_p HempElementEof;

extern hemp_etype_p HempElementText;
extern hemp_etype_p HempElementWord;
extern hemp_etype_p HempElementNumber;
extern hemp_etype_p HempElementInteger;
extern hemp_etype_p HempElementSQuote;
extern hemp_etype_p HempElementDQuote;
extern hemp_etype_p HempElementBlock;


/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

#define HEMP_PARSE_PROTO        \
    hemp_element_p *elemptr,    \
    hemp_scope_p    scope,      \
    hemp_prec_t     precedence, \
    hemp_bool_t     force

#define HEMP_SKIP_PROTO         \
    hemp_element_p *elemptr

#define HEMP_TEXT_PROTO         \
    hemp_element_p element,     \
    hemp_text_p    text

#define HEMP_PARSE_ARGS         \
    elemptr, scope, precedence, force

hemp_element_p
    hemp_element_new();

hemp_element_p
    hemp_element_init(
        hemp_element_p,
        hemp_etype_p, 
        hemp_cstr_p, 
        hemp_pos_t, 
        hemp_size_t
    );

void
    hemp_element_free(
        hemp_element_p element
    );

hemp_element_p
    hemp_element_dont_skip(
        HEMP_SKIP_PROTO
    );

hemp_element_p
    hemp_element_next_skip_space(
        HEMP_SKIP_PROTO
    );

hemp_element_p
    hemp_element_next_skip_delimiter(
        HEMP_SKIP_PROTO
    );

hemp_element_p
    hemp_element_next_skip_separator(
        HEMP_SKIP_PROTO
    );

hemp_element_p
    hemp_element_parse(
        hemp_element_p   // TODO: add scope
    );

hemp_element_p
    hemp_element_dont_parse(
        HEMP_PARSE_PROTO
    );

hemp_element_p
    hemp_element_parse_block(
        HEMP_PARSE_PROTO
    );

hemp_list_p
    hemp_element_parse_exprs(
        HEMP_PARSE_PROTO
    );

hemp_element_p
    hemp_element_parse_expr(
        HEMP_PARSE_PROTO
    );

hemp_bool_t
    hemp_element_dump(
        hemp_element_p element
    );


/*--------------------------------------------------------------------------
 * function prototypes for specific element types
 *--------------------------------------------------------------------------*/

hemp_element_p
    hemp_element_space_parse_expr(
        HEMP_PARSE_PROTO
    );

hemp_element_p
    hemp_element_literal_parse_expr(
        HEMP_PARSE_PROTO
    );

hemp_text_p
    hemp_element_literal_text(
        HEMP_TEXT_PROTO
    );

hemp_text_p
    hemp_element_quoted_text(
        HEMP_TEXT_PROTO
    );

hemp_text_p
    hemp_element_eof_text(
        HEMP_TEXT_PROTO
    );

hemp_text_p
    hemp_element_block_text(
        HEMP_TEXT_PROTO
    );

hemp_mem_p
    hemp_element_number_constructor(
        hemp_p      hemp,
        hemp_cstr_p name
    );


void
    hemp_element_text_clean(
        hemp_element_p element
    );


/*--------------------------------------------------------------------------
 * macros
 *--------------------------------------------------------------------------*/

#define hemp_element_null(e) \
    hemp_element_free(e);    \
    e = NULL;

#define hemp_parse_expr(ep, sc, pr, fr) \
    (*ep)->type->parse_expr(ep, sc, pr, fr)

#define hemp_skip_space(ep) \
    (*ep)->type->skip_space(ep)

#define hemp_skip_delimiter(ep) \
    (*ep)->type->skip_delimiter(ep)

#define hemp_skip_separator(ep) \
    (*ep)->type->skip_separator(ep)

#define hemp_has_next(ep) \
    (*elemptr)->next

#define hemp_go_next(ep) \
    *elemptr = (*elemptr)->next

#define hemp_at_eof(ep) \
    (*ep)->type == HempElementEof

#define hemp_set_flag(item, flag) \
    item->flags |= flag

#define hemp_clear_flag(flags, flag) \
    item->flags &= ~flag

#define hemp_has_flag(item, flag) \
    item->flags & flag

#define hemp_not_flag(item, flag) \
    ! (hemp_has_flag(item, flag))

#define hemp_element_skip_space     hemp_element_self
#define hemp_element_skip_delimiter hemp_element_self
#define hemp_element_skip_separator hemp_element_self



#endif /* HEMP_ELEMENT_H */