#ifndef HEMP_ELEMENT_H
#define HEMP_ELEMENT_H

#include "hemp/memory.h"
#include "hemp/cstr.h"


/*--------------------------------------------------------------------------
 * data structures
 *--------------------------------------------------------------------------*/

struct hemp_etype {
    hemp_name_t     name;
    hemp_skip_fn    skip_space;
    hemp_parse_fn   parse_expr;
    hemp_cstr_t     (*text)();
    hemp_cstr_t     (*number)();
};

struct hemp_element {
    hemp_etype_t    type;
    hemp_cstr_t     token;
    hemp_pos_t      position;
    hemp_size_t     length;
    hemp_element_t  next;
    hemp_evalue_t   value;
};

union hemp_evalue {
    hemp_value_t    constant;
    hemp_value_t    variable;
    hemp_unary_t    unary;
    hemp_binary_t   binary;
};

struct hemp_unary {
    hemp_element_t  expr;
    // TODO: args
};

struct hemp_binary {
    hemp_element_t  lhs;
    hemp_element_t  rhs;
};



/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

#define HEMP_PARSE_PROTO        \
    hemp_element_t  element,    \
    hemp_element_t *elemptr,    \
    hemp_scope_t    scope,      \
    hemp_prec_t     precedence, \
    hemp_bool_t     force

#define HEMP_SKIP_PROTO         \
    hemp_element_t  element,    \
    hemp_element_t *elemptr

#define HEMP_PARSE_ARGS         \
    element, elemptr, scope, precedence, force


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
    hemp_element_skip_space(
        HEMP_SKIP_PROTO
    );

hemp_element_t
    hemp_element_next_skip_space(
        HEMP_SKIP_PROTO
    );

hemp_element_t
    hemp_element_parse(
        hemp_element_t   // TODO: add scope
    );

hemp_element_t
    hemp_element_parse_block(
        HEMP_PARSE_PROTO
    );

hemp_element_t
    hemp_element_parse_exprs(
        HEMP_PARSE_PROTO
    );

hemp_element_t
    hemp_element_parse_expr(
        HEMP_PARSE_PROTO
    );

hemp_element_t
    hemp_literal_parse_expr(
        HEMP_PARSE_PROTO
        );

hemp_bool_t
    hemp_element_dump(
        hemp_element_t element
    );



/*--------------------------------------------------------------------------
 * macros
 *--------------------------------------------------------------------------*/

#define hemp_element_null(e) \
    hemp_element_free(e);    \
    e = NULL;

#define hemp_parse_expr(ep, sc, pr, fr) \
    (*ep)->type->parse_expr(*ep, ep, sc, pr, fr)

#define hemp_skip_space(el, ep) \
    el->type->skip_space(el, ep)


#endif /* HEMP_ELEMENT_H */