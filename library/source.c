#include <hemp/source.h>


/*--------------------------------------------------------------------------
 * source initialisation and cleanup functions 
 *-------------------------------------------------------------------------*/

hemp_source_p
hemp_source_init(
    hemp_scheme_p scheme,
    hemp_cstr_p   name
) {
    hemp_source_p source = (hemp_source_p) hemp_mem_alloc(
        sizeof(struct hemp_source_s)
    );

    if (! source)
        hemp_mem_fail("source");

    source->name = hemp_cstr_copy(name);

    if (! source->name)
        hemp_mem_fail("source name");

    source->scheme = scheme;
    source->text   = NULL;

    /*
    debug_mem(
        "Allocated %s source at %p\n", 
        scheme->name, source
    );]
    */

    return source;
}


void
hemp_source_free(
    hemp_source_p source
) {
    /* debug_mem("Releasing %s source at %p\n", source->scheme->name, source); */

    /* text sources have the name and text fields pointing at the same 
     * string so we only free the text field if it's different
     */
    if (source->text && source->text != source->name) {
        /* debug_mem("Releasing %s source text at %p\n", source->scheme->name, source->text); */
        hemp_mem_free(source->text);
    }

    hemp_mem_free(source->name);
    hemp_mem_free(source);
}



