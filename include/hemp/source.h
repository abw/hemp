#ifndef HEMP_SOURCE_H
#define HEMP_SOURCE_H

#include <hemp/core.h>
#include <hemp/scheme.h>


/*--------------------------------------------------------------------------
 * A source is a data structure used to represent the source text for a 
 * template.  It has a pointer to a scheme and fields for the template name
 * and input text.
 *-------------------------------------------------------------------------*/

struct hemp_source_s {
    hemp_scheme_p scheme;           /* e.g. text, file, etc                 */
    hemp_cstr_p   md5;              /* MD5 hex digest of template uri       */
    hemp_cstr_p   name;             /* path to file or source text          */
    hemp_cstr_p   text;             /* file contents or source text         */
};



/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

hemp_source_p   hemp_source_init( hemp_scheme_p, hemp_cstr_p );
void            hemp_source_free( hemp_source_p );



/*--------------------------------------------------------------------------
 * macros
 *--------------------------------------------------------------------------*/

#define hemp_source_read(source) (          \
    source->text                            \
        ? source->text                      \
        : source->scheme->reader(source)    \
)                                           


#endif /* HEMP_SOURCE_H */