#include <hemp/source.h>


/*--------------------------------------------------------------------------
 * source initialisation and cleanup functions 
 *-------------------------------------------------------------------------*/

hemp_source
hemp_source_init(
    hemp_scheme scheme,
    hemp_string name
) {
    hemp_source source;
    HEMP_ALLOCATE(source);

    source->name   = hemp_string_clone(name, "source name");
    source->scheme = scheme;
    source->md5    = 
    source->text   = NULL;

    /*
    hemp_debug_mem(
        "Allocated %s source at %p\n", 
        scheme->name, source
    );]
    */

    return source;
}


void
hemp_source_set_md5(
    hemp_source source,
    hemp_string md5
) {
    if (source->md5)
        hem_mem_free(source->md5);
    
    source->md5 = hemp_string_clone(md5, "source MD5 digest");
}


void
hemp_source_free(
    hemp_source source
) {
    /* hemp_debug_mem("Releasing %s source at %p\n", source->scheme->name, source); */

    if (source->md5)
        hemp_mem_free(source->md5);

    /* text sources have the name and text fields pointing at the same 
     * string so we only free the text field if it's different
     */

    if (source->text && source->text != source->name) {
        /* hemp_debug_mem("Releasing %s source text at %p\n", source->scheme->name, source->text); */
        hemp_mem_free(source->text);
    }

    hemp_mem_free(source->name);
    hemp_mem_free(source);
}



