#ifndef HEMP_DOCUMENT_H
#define HEMP_DOCUMENT_H

#include <hemp/dialect.h>
#include <hemp/source.h>
#include <hemp/scope.h>
#include <hemp/stack.h>
#include <hemp/fragments.h>


/*--------------------------------------------------------------------------
 * type definitions
 *--------------------------------------------------------------------------*/

struct hemp_document {
    Hemp            hemp;
    HempDialect     dialect;
    HempSource      source;
    HempScope       scope;
    HempFragments   fragments;
    HempFragment    tree;
//  HempAction      scanner;
//  HempTagset      tagset;

    /* these don't really belong here but it'll do for now */
    HempString      scanptr;    /* position in source string during scanning */
    HempString      scantok;    /* start of current token being scanned      */
    HempPos         scanpos;    /* byte position of scanptr (can we compute this?) */
    HempStack       scantags;   /* stack of nested tags                 */
};


/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

HempDocument
hemp_document_new(
    HempDialect     dialect,
    HempSource      source
);

void    
hemp_document_free(
    HempDocument    document
);

HempFragment
hemp_document_tokens(
    HempDocument    document
);

HempBool
hemp_document_scan(
    HempDocument    document
);

HempBool
hemp_document_compile(
    HempDocument    document
);
    
HempFragment
hemp_document_tree(
    HempDocument    document
);


HempText
hemp_document_render(
    HempDocument    document,
    HempContext     context
);

// quick hack - need to sort out names properly before going too far
HempText
hemp_document_process(
    HempDocument    document,
    HempContext     context,
    HempText        output
);


HempValue
hemp_document_data(
    HempDocument    document,
    HempContext     context
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
    HempFragment _hemp_frag = hemp_document_add_fragment(       \
        document, element                                       \
    );                                                          \
    document->scantok  = document->scanptr;                     \
    document->scanpos += _hemp_frag->length;                    \
    _hemp_frag;                                                 \
})

#define hemp_document_scanned_to(document, element, src) ({     \
    document->scanptr = src;                                    \
    HempFragment _hemp_frag = hemp_document_add_fragment(       \
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
    ((HempTag) hemp_stack_top(document->scantags))


#endif /* HEMP_DOCUMENT_H */