#ifndef HEMP_TEMPLATE_H
#define HEMP_TEMPLATE_H

#include <hemp/dialect.h>
#include <hemp/source.h>
#include <hemp/scope.h>
#include <hemp/elements.h>


/*--------------------------------------------------------------------------
 * type definitions
 *--------------------------------------------------------------------------*/

struct hemp_template {
    hemp_dialect    dialect;
    hemp_source     source;
//    hemp_tagset     tagset;
    hemp_scope      scope;
    hemp_elements   elements;
    hemp_element    tree;
    hemp_action     scanner;

    /* these don't really belong here but it'll do for now */
    hemp_pos        pos;
    hemp_string   * srcptr;
};


/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

hemp_template
hemp_template_new(
    hemp_dialect    dialect,
    hemp_source     source
);

void    
hemp_template_free(
    hemp_template   template
);

hemp_element
hemp_template_tokens(
    hemp_template   template
);

hemp_bool
hemp_template_scan(
    hemp_template   template
);

hemp_bool
hemp_template_compile(
    hemp_template   template
);
    
hemp_element
hemp_template_tree(
    hemp_template   template
);


hemp_text
hemp_template_render(
    hemp_template   template,
    hemp_context    context
);


hemp_value
hemp_template_data(
    hemp_template   template,
    hemp_context    context
);


/*--------------------------------------------------------------------------
 * macros
 *--------------------------------------------------------------------------*/

#define hemp_template_first_element(t)  \
    t->elements->head

#define hemp_template_last_element(t)  \
    t->elements->tail


#endif /* HEMP_TEMPLATE_H */