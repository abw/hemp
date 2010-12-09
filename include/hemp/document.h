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

struct hemp_document {
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
    hemp_stack      scantags;
};


/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

hemp_document
hemp_document_new(
    hemp_dialect    dialect,
    hemp_source     source
);

void    
hemp_document_free(
    hemp_document   document
);

hemp_fragment
hemp_document_tokens(
    hemp_document   document
);

hemp_bool
hemp_document_scan(
    hemp_document   document
);

hemp_bool
hemp_document_compile(
    hemp_document   document
);
    
hemp_fragment
hemp_document_tree(
    hemp_document   document
);


hemp_text
hemp_document_render(
    hemp_document   document,
    hemp_context    context
);


hemp_value
hemp_document_data(
    hemp_document   document,
    hemp_context    context
);


/*--------------------------------------------------------------------------
 * macros
 *--------------------------------------------------------------------------*/

#define hemp_document_first_fragment(t)                         \
    t->fragments->head

#define hemp_document_last_fragment(t)                          \
    t->fragments->tail

#define hemp_document_add_fragment(document, element)           \
    hemp_fragments_add_fragment(                                \
        document->fragments, element,                           \
        document->scantok, document->scanpos,                   \
        document->scanptr - document->scantok                   \
    )

#define hemp_document_scanned(document, element) ({             \
    hemp_fragment _hemp_frag = hemp_document_add_fragment(      \
        document, element                                       \
    );                                                          \
    document->scantok  = document->scanptr;                     \
    document->scanpos += _hemp_frag->length;                    \
    _hemp_frag;                                                 \
})

#define hemp_document_scanned_to(document, element, src) ({     \
    document->scanptr = src;                                    \
    hemp_fragment _hemp_frag = hemp_document_add_fragment(      \
        document, element                                       \
    );                                                          \
    document->scantok  = src;                                   \
    document->scanpos += _hemp_frag->length;                    \
    _hemp_frag;                                                 \
})

#define hemp_document_enter_tag(document, tag)                  \
    hemp_stack_push(document->scantags, tag)

#define hemp_document_leave_tag(document)                       \
    hemp_stack_pop(document->scantags)

#define hemp_document_current_tag(document)                     \
    ((hemp_tag) hemp_stack_top(document->scantags))


#endif /* HEMP_DOCUMENT_H */