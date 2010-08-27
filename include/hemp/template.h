#ifndef HEMP_TEMPLATE_H
#define HEMP_TEMPLATE_H

#include <hemp/dialect.h>
#include <hemp/source.h>
#include <hemp/elements.h>


/*--------------------------------------------------------------------------
 * data structures
 *--------------------------------------------------------------------------*/

struct hemp_template_s {
    hemp_dialect_p  dialect;
    hemp_source_p   source;
    hemp_tagset_p   tagset;
    hemp_elements_p elements;
    hemp_element_p  tree;
};


/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

hemp_template_p
    hemp_template_init(
        hemp_dialect_p  dialect,
        hemp_source_p   source
    );

void    
    hemp_template_free(
        hemp_template_p
    );

hemp_element_p
    hemp_template_tokens(
        hemp_template_p tmpl
    );

hemp_bool_t
    hemp_template_scan(
        hemp_template_p tmpl
    );

hemp_bool_t
    hemp_template_compile(
        hemp_template_p
    );

hemp_text_p
    hemp_template_render(
        hemp_template_p tmpl,
        hemp_context_p  context
    );


/*--------------------------------------------------------------------------
 * macros
 *--------------------------------------------------------------------------*/

#define hemp_template_first_element(t)  \
    t->elements->head

#define hemp_template_last_element(t)  \
    t->elements->tail


#endif /* HEMP_TEMPLATE_H */