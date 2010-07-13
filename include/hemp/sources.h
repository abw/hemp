#ifndef HEMP_SOURCES_H
#define HEMP_SOURCES_H

#include "hemp/memory.h"
#include "hemp/cstr.h"

#define HEMP_TEXT "text"
#define HEMP_FILE "file"


/*--------------------------------------------------------------------------
 * data structures
 *
 * A scheme is the part before the first colon in a URI/URL
 * e.g.
 *    "file:/path/to/file"              # file scheme
 *    "text:source text"                # text scheme
 *
 * Each scheme has a name and a reader function whose job it is to return the 
 * source text for the template.  e.g. the "file" scheme reads the file 
 * content, the "text" scheme simply returns the source text (i.e. the part
 * after the leading "text:" in the example above.  At some point in the 
 * future this may be expanded to include other functions.  Oh look, we've 
 * already added one: namer() which Does The Right Thing to return a human
 * readable name for the source.
 *
 * A source is a data structure used to represent the source text for a 
 * template.  It has a pointer to a scheme and fields for the template name
 * and input text.
 *-------------------------------------------------------------------------*/

struct hemp_scheme {
    hemp_name_t         name;
    hemp_source_text_fn namer;
    hemp_source_bool_fn checker;
    hemp_source_text_fn reader;
};


struct hemp_source {
    hemp_scheme_t scheme;           /* e.g. text, file, etc          */
    hemp_cstr_t   name;             /* path to file or source text   */
    hemp_cstr_t   text;             /* file contents or source text  */
};



/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

/* text: scheme prototypes */
hemp_cstr_t     hemp_scheme_text_namer  ( hemp_source_t );
hemp_cstr_t     hemp_scheme_text_reader ( hemp_source_t );
hemp_bool_t     hemp_scheme_text_checker( hemp_source_t );

/* file: scheme prototypes */
hemp_cstr_t     hemp_scheme_file_namer  ( hemp_source_t );
hemp_cstr_t     hemp_scheme_file_reader ( hemp_source_t );
hemp_bool_t     hemp_scheme_file_checker( hemp_source_t );

/* source prototypes */
hemp_source_t   hemp_source( hemp_cstr_t, hemp_cstr_t );
hemp_source_t   hemp_source_init( hemp_scheme_t, hemp_cstr_t );
void            hemp_source_free( hemp_source_t );


/*--------------------------------------------------------------------------
 * macros
 *--------------------------------------------------------------------------*/

#define hemp_source_null(source)            \
    hemp_source_free(source);               \
    source = NULL

#define hemp_source_read(source) (          \
    source->text                            \
        ? source->text                      \
        : source->scheme->reader(source)    \
)                                           


/*--------------------------------------------------------------------------
 * externals
 *--------------------------------------------------------------------------*/

extern hemp_scheme_t HempSchemeText;
extern hemp_scheme_t HempSchemeFile;


#endif /* HEMP_SOURCES_H */