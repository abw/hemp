#include <hemp/source.h>


/*--------------------------------------------------------------------------
 * source initialisation and cleanup functions 
 *-------------------------------------------------------------------------*/

hemp_source
hemp_source_new(
    hemp_scheme scheme,
    hemp_string name
) {
    hemp_source source;
    HEMP_ALLOCATE(source);
    source->name   = hemp_string_clone(name, "source name");
    source->scheme = scheme;
    source->md5    = 
    source->text   = NULL;

    return source;
}


void
hemp_source_md5(
    hemp_source source,
    hemp_string md5
) {
    if (source->md5)
        hemp_mem_free(source->md5);
    
    source->md5 = hemp_string_clone(md5, "source MD5 digest");
}


void
hemp_source_free(
    hemp_source source
) {
    /* hemp_debug_mem("Releasing %s source at %p\n", source->scheme->name, source); */

    if (source->md5)
        hemp_mem_free(source->md5);

    if (source->scheme->cleaner) {
//      hemp_debug_mem("calling %s source cleaner\n", source->scheme->name);
        source->scheme->cleaner(source);
    }

    hemp_mem_free(source->name);
    hemp_mem_free(source);
}



