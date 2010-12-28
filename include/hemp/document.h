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
    hemp_scope      scope;
    hemp_fragments  fragments;
    hemp_fragment   tree;
//  hemp_action     scanner;
//  hemp_tagset     tagset;

    /* these don't really belong here but it'll do for now */
    hemp_string     scanptr;    /* position in source string during scanning */
    hemp_string     scantok;    /* start of current token being scanned      */
    hemp_pos        scanpos;    /* byte position of scanptr (can we compute this?) */
    hemp_stack      scantags;   /* stack of nested tags                 */
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

// quick hack - need to sort out names properly before going too far
hemp_text
hemp_document_process(
    hemp_document document,
    hemp_context  context,
    hemp_text     output
);


hemp_value
hemp_document_data(
    hemp_document   document,
    hemp_context    context
);

HEMP_OUTPUT(hemp_document_pairs);


/*--------------------------------------------------------------------------
 * macros
 *--------------------------------------------------------------------------*/

#define hemp_document_hemp(d)                                   \
    d->dialect->hemp

#define hemp_document_first_fragment(d)                         \
    d->fragments->head

#define hemp_document_last_fragment(d)                          \
    d->fragments->tail

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