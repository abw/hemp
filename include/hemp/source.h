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
    hemp_scheme     scheme;         /* e.g. text, file, etc                 */
    hemp_string     name;           /* path to file or source text          */
    hemp_string     md5;            /* MD5 hex digest of document uri       */
    hemp_string     text;           /* file contents or source text         */
};


/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

hemp_source
hemp_source_new(
    hemp_scheme     scheme,
    hemp_string     name
);

void
hemp_source_md5(
    hemp_source     source,
    hemp_string     md5
);

void
hemp_source_free(
    hemp_source     source
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
    (hemp_string) source->scheme->namer(source) \
)


#endif /* HEMP_SOURCE_H */