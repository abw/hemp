#include <hemp/scheme.h>


/*--------------------------------------------------------------------------
 * generic scheme initialisation and cleanup functions
 *--------------------------------------------------------------------------*/


hemp_scheme_p
hemp_scheme_init(
    hemp_cstr_p             name,
    hemp_source_namer_f     namer,
    hemp_source_checker_f   checker,
    hemp_source_reader_f    reader
) {
    hemp_scheme_p scheme = hemp_mem_alloc(
        sizeof(struct hemp_scheme_s)
    );

    if (! scheme)
        hemp_mem_fail("scheme");

    scheme->name = hemp_cstr_copy(name);

    if (! scheme->name)
        hemp_mem_fail("scheme name");

    scheme->namer   = namer;
    scheme->checker = checker;
    scheme->reader  = reader;

    return scheme;
}


void
hemp_scheme_free(
    hemp_scheme_p scheme
) {
    if (scheme->name)
        hemp_mem_free(scheme->name);

    hemp_mem_free(scheme);
}



/*--------------------------------------------------------------------------
 * text scheme functions 
 *-------------------------------------------------------------------------*/

hemp_cstr_p
hemp_scheme_text_reader(
    hemp_source_p source
) {
    // I think this is the cause of SWIG failing...
    return (source->text = source->name);
}


hemp_cstr_p 
hemp_scheme_text_namer(
    hemp_source_p source
) {
    return HEMP_TEXT;
}


hemp_bool_t
hemp_scheme_text_checker(
    hemp_source_p source
) {
    return HEMP_TRUE;
}



/*--------------------------------------------------------------------------
 * file scheme functions 
 *-------------------------------------------------------------------------*/

hemp_cstr_p
hemp_scheme_file_reader(
    hemp_source_p source 
) {
    hemp_cstr_p output = "TODO: read file";
    debug_red("TODO: hemp_scheme_file_reader()\n");
    return (source->text = hemp_cstr_copy(output));
}


hemp_cstr_p
hemp_scheme_file_namer(
    hemp_source_p source 
) {
    return source->name;
}


hemp_bool_t
hemp_scheme_file_checker(
    hemp_source_p source
) {
    debug_red("TODO: hemp_scheme_file_checker()\n");
    return HEMP_FALSE;
}

