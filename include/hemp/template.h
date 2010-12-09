#ifndef HEMP_TEMPLATE_H
#define HEMP_TEMPLATE_H

#include <hemp/dialect.h>
#include <hemp/source.h>
#include <hemp/scope.h>
#include <hemp/stack.h>
#include <hemp/fragments.h>


/*--------------------------------------------------------------------------
 * type definitions
 *--------------------------------------------------------------------------*/

struct hemp_template {
    hemp_dialect    dialect;
    hemp_source     source;
//    hemp_tagset     tagset;
    hemp_scope      scope;
    hemp_fragments  fragments;
    hemp_fragment   tree;
    hemp_action     scanner;

    /* these don't really belong here but it'll do for now */
    hemp_pos        scanpos;
    hemp_string     scanptr;
    hemp_string     scantok;
    hemp_stack      scanners;
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

hemp_fragment
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
    
hemp_fragment
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

#define hemp_template_first_element(t)      \
    t->fragments->head

#define hemp_template_last_element(t)       \
    t->fragments->tail

#define hemp_template_add_fragment(template, element)           \
    hemp_fragments_add_fragment(                                \
        template->fragments, element,                           \
        template->scantok, template->scanpos,                   \
        template->scanptr - template->scantok                   \
    )

#define hemp_template_scanned(template, element) ({             \
    hemp_fragment _hemp_frag = hemp_template_add_fragment(      \
        template, element                                       \
    );                                                          \
    template->scantok  = template->scanptr;                     \
    template->scanpos += _hemp_frag->length;                    \
    _hemp_frag;                                                 \
})


#define hemp_template_scanned_to(template, element, src) ({     \
    template->scanptr = src;                                    \
    hemp_fragment _hemp_frag = hemp_template_add_fragment(      \
        template, element                                       \
    );                                                          \
    template->scantok  = src;                                   \
    template->scanpos += _hemp_frag->length;                    \
    _hemp_frag;                                                 \
})



#endif /* HEMP_TEMPLATE_H */