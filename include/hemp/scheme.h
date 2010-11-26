#ifndef HEMP_SCHEME_H
#define HEMP_SCHEME_H

#include <hemp/core.h>
#include <hemp/source.h>
#include <hemp/type/hash.h>


/*--------------------------------------------------------------------------
 * data structures
 *--------------------------------------------------------------------------*/

struct hemp_scheme {
    hemp_string             name;
    hemp_source_namer_f     namer;
    hemp_source_checker_f   checker;
    hemp_source_reader_f    reader;
};


/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

hemp_scheme   
    hemp_scheme_new(
        hemp_string             name,
        hemp_source_namer_f     namer,
        hemp_source_checker_f   checker,
        hemp_source_reader_f    reader
    );

void
    hemp_scheme_free(
        hemp_scheme             scheme
    );

/* text: scheme prototypes */
hemp_string hemp_scheme_text_namer  ( hemp_source );
hemp_string hemp_scheme_text_reader ( hemp_source );
hemp_bool   hemp_scheme_text_checker( hemp_source );

/* file: scheme prototypes */
hemp_string hemp_scheme_file_namer  ( hemp_source );
hemp_string hemp_scheme_file_reader ( hemp_source );
hemp_bool   hemp_scheme_file_checker( hemp_source );


#endif /* HEMP_SCHEME_H */