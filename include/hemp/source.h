#ifndef HEMP_SOURCE_H
#define HEMP_SOURCE_H

#include <hemp/core.h>
#include <hemp/scheme.h>


/*--------------------------------------------------------------------------
 * A source is a data structure used to represent the source text for a 
 * document.  It has a pointer to a scheme and fields for the document name
 * and input text.
 *-------------------------------------------------------------------------*/

struct hemp_source {
    HempScheme      scheme;         /* e.g. text, file, etc                 */
    HempString      name;           /* path to file or source text          */
    HempString      md5;            /* MD5 hex digest of document uri       */
    HempString      text;           /* file contents or source text         */
};


/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

HempSource
hemp_source_new(
    HempScheme      scheme,
    HempString      name
);

void
hemp_source_md5(
    HempSource      source,
    HempString      md5
);

void
hemp_source_free(
    HempSource      source
);



/*--------------------------------------------------------------------------
 * macros
 *--------------------------------------------------------------------------*/

#define hemp_source_read(source) (              \
    source->text                                \
        ? source->text                          \
        : source->scheme->reader(source)        \
)                                           

#define hemp_source_name(source) (              \
    (HempString) source->scheme->namer(source)  \
)


#endif /* HEMP_SOURCE_H */