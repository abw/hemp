#ifndef HEMP_SOURCE_H
#define HEMP_SOURCE_H

#include <hemp/core.h>
#include <hemp/scheme.h>


/*--------------------------------------------------------------------------
 * A source is a data structure used to represent the source text for a 
 * template.  It has a pointer to a scheme and fields for the template name
 * and input text.
 *-------------------------------------------------------------------------*/

struct hemp_source {
    hemp_scheme scheme;           /* e.g. text, file, etc                 */
    hemp_string    md5;              /* MD5 hex digest of template uri       */
    hemp_string    name;             /* path to file or source text          */
    hemp_string    text;             /* file contents or source text         */
};



/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

hemp_source   hemp_source_init( hemp_scheme, hemp_string );
void            hemp_source_free( hemp_source );



/*--------------------------------------------------------------------------
 * macros
 *--------------------------------------------------------------------------*/

#define hemp_source_read(source) (          \
    source->text                            \
        ? source->text                      \
        : source->scheme->reader(source)    \
)                                           

#define hemp_source_name(source)            \
    ((hemp_string) source->scheme->namer(source))


#endif /* HEMP_SOURCE_H */