#include <hemp/source.h>


/*--------------------------------------------------------------------------
 * source initialisation and cleanup functions 
 *-------------------------------------------------------------------------*/

HempSource
hemp_source_new(
    HempScheme scheme,
    HempString name
) {
    HempSource source;
    HEMP_ALLOCATE(source);
    source->name   = hemp_string_clone(name, "source name");
    source->scheme = scheme;
    source->md5    = 
    source->text   = NULL;

    return source;
}


void
hemp_source_md5(
    HempSource source,
    HempString md5
) {
    if (source->md5)
        hemp_mem_free(source->md5);
    
    source->md5 = hemp_string_clone(md5, "source MD5 digest");
}


void
hemp_source_free(
    HempSource source
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



