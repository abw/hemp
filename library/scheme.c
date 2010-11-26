#include <hemp/scheme.h>


/*--------------------------------------------------------------------------
 * generic scheme initialisation and cleanup functions
 *--------------------------------------------------------------------------*/


hemp_scheme
hemp_scheme_new(
    hemp_string             name,
    hemp_source_namer_f     namer,
    hemp_source_checker_f   checker,
    hemp_source_reader_f    reader
) {
    hemp_scheme scheme;
    HEMP_ALLOCATE(scheme);

    scheme->name    = hemp_string_clone(name, "scheme name");
    scheme->namer   = namer;
    scheme->checker = checker;
    scheme->reader  = reader;

    return scheme;
}


void
hemp_scheme_free(
    hemp_scheme scheme
) {
    if (scheme->name)
        hemp_mem_free(scheme->name);

    hemp_mem_free(scheme);
}



/*--------------------------------------------------------------------------
 * text scheme functions 
 *-------------------------------------------------------------------------*/

hemp_string
hemp_scheme_text_reader(
    hemp_source source
) {
    return (source->text = source->name);
}


hemp_string 
hemp_scheme_text_namer(
    hemp_source source
) {
    return HEMP_TEXT;
}


hemp_bool
hemp_scheme_text_checker(
    hemp_source source
) {
    return HEMP_TRUE;
}



/*--------------------------------------------------------------------------
 * file scheme functions 
 *-------------------------------------------------------------------------*/

hemp_string
hemp_scheme_file_reader(
    hemp_source source 
) {
    hemp_string output = "TODO: read file";
    hemp_debug_red("TODO: hemp_scheme_file_reader()\n");
    return (source->text = hemp_string_copy(output));
}


hemp_string
hemp_scheme_file_namer(
    hemp_source source 
) {
    return source->name;
}


hemp_bool
hemp_scheme_file_checker(
    hemp_source source
) {
    hemp_debug_red("TODO: hemp_scheme_file_checker()\n");
    return HEMP_FALSE;
}

