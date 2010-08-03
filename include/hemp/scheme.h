#ifndef HEMP_SCHEME_H
#define HEMP_SCHEME_H

#include <hemp/core.h>
#include <hemp/hash.h>
#include <hemp/source.h>


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
 *-------------------------------------------------------------------------*/

struct hemp_scheme_s {
    hemp_cstr_p             name;
    hemp_source_namer_f     namer;
    hemp_source_checker_f   checker;
    hemp_source_reader_f    reader;
};



/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

/* generic scheme function prototypes */
hemp_scheme_p   hemp_scheme_init(
    hemp_cstr_p,
    hemp_source_namer_f,
    hemp_source_checker_f,
    hemp_source_reader_f
);
void            hemp_scheme_free(hemp_scheme_p);

/* text: scheme prototypes */
hemp_cstr_p     hemp_scheme_text_namer  ( hemp_source_p );
hemp_cstr_p     hemp_scheme_text_reader ( hemp_source_p );
hemp_bool_t     hemp_scheme_text_checker( hemp_source_p );

/* file: scheme prototypes */
hemp_cstr_p     hemp_scheme_file_namer  ( hemp_source_p );
hemp_cstr_p     hemp_scheme_file_reader ( hemp_source_p );
hemp_bool_t     hemp_scheme_file_checker( hemp_source_p );


#endif /* HEMP_SCHEME_H */