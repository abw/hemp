#ifndef HEMP_ELEMENT_H
#define HEMP_ELEMENT_H

#include "hemp/defaults.h"
#include "hemp/memory.h"
#include "hemp/cstr.h"
#include "hemp/text.h"
#include "hemp/list.h"


/*--------------------------------------------------------------------------
 * data structures
 *--------------------------------------------------------------------------*/

struct hemp_etype {
    hemp_name_t     name;
    hemp_skip_fn    skip_space;
    hemp_skip_fn    skip_delimiter;
    hemp_skip_fn    skip_separator;
    hemp_parse_fn   parse_expr;
    hemp_text_fn    text;
    hemp_cstr_t     (*number)();
};

struct hemp_unary {
    hemp_element_t  expr;
    // TODO: args
};

struct hemp_binary {
    hemp_element_t  lhs;
    hemp_element_t  rhs;
};

struct hemp_block {
    hemp_list_t     exprs;
};


union hemp_evalue {
    hemp_value_t    constant;
    hemp_value_t    variable;
    struct hemp_unary   unary;
    struct hemp_binary  binary;
    struct hemp_block   block;
};

struct hemp_element {
    hemp_etype_t    type;
    hemp_cstr_t     token;
    hemp_pos_t      position;
    hemp_size_t     length;
    hemp_element_t  next;
    union hemp_evalue value;
};






/*--------------------------------------------------------------------------
 * external static element types
 *--------------------------------------------------------------------------*/

extern hemp_etype_t HempElementSpace;
extern hemp_etype_t HempElementComment;
extern hemp_etype_t HempElementTagStart;
extern hemp_etype_t HempElementTagEnd;
extern hemp_etype_t HempElementEof;

extern hemp_etype_t HempElementText;
extern hemp_etype_t HempElementWord;
extern hemp_etype_t HempElementNumber;
extern hemp_etype_t HempElementInteger;
extern hemp_etype_t HempElementSQuote;
extern hemp_etype_t HempElementDQuote;
extern hemp_etype_t HempElementBlock;


/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

#define HEMP_PARSE_PROTO        \
    hemp_element_t *elemptr,    \
    hemp_scope_t    scope,      \
    hemp_prec_t     precedence, \
    hemp_bool_t     force

#define HEMP_SKIP_PROTO         \
    hemp_element_t *elemptr

#define HEMP_PARSE_ARGS         \
    elemptr, scope, precedence, force


hemp_element_t  
    hemp_element_new(
        hemp_etype_t, 
        hemp_cstr_t, 
        hemp_pos_t, 
        hemp_size_t
    );

hemp_element_t  
    hemp_element_init();
    
void
    hemp_element_free(
        hemp_element_t element
    );

hemp_element_t
    hemp_element_dont_skip(
        HEMP_SKIP_PROTO
    );

hemp_element_t
    hemp_element_next_skip_space(
        HEMP_SKIP_PROTO
    );

hemp_element_t
    hemp_element_next_skip_delimiter(
        HEMP_SKIP_PROTO
    );

hemp_element_t
    hemp_element_next_skip_separator(
        HEMP_SKIP_PROTO
    );

hemp_element_t
    hemp_element_parse(
        hemp_element_t   // TODO: add scope
    );

hemp_element_t
    hemp_element_dont_parse(
        HEMP_PARSE_PROTO
    );

hemp_element_t
    hemp_element_parse_block(
        HEMP_PARSE_PROTO
    );

hemp_list_t
    hemp_element_parse_exprs(
        HEMP_PARSE_PROTO
    );

hemp_element_t
    hemp_element_parse_expr(
        HEMP_PARSE_PROTO
    );

hemp_bool_t
    hemp_element_dump(
        hemp_element_t element
    );

hemp_text_t
    hemp_element_block_text(
        hemp_element_t  element,
        hemp_text_t     text
    );


/*--------------------------------------------------------------------------
 * function prototypes for specific element types
 *--------------------------------------------------------------------------*/

hemp_element_t
    hemp_element_space_parse_expr(
        HEMP_PARSE_PROTO
    );

hemp_element_t
    hemp_element_literal_parse_expr(
        HEMP_PARSE_PROTO
    );

hemp_text_t                                                     /* text */
    hemp_element_literal_text(
        hemp_element_t element,
        hemp_text_t     text
    );

hemp_text_t
    hemp_element_quoted_text(
        hemp_element_t  element,
        hemp_text_t     text
    );

hemp_text_t                                                     /* EOF */
    hemp_element_eof_text(
        hemp_element_t element,
        hemp_text_t     text
    );

hemp_text_t
    hemp_element_block_text(
        hemp_element_t  element,
        hemp_text_t     text
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

#define hemp_element_skip_space     hemp_element_self
#define hemp_element_skip_delimiter hemp_element_self
#define hemp_element_skip_separator hemp_element_self



#endif /* HEMP_ELEMENT_H */