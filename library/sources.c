#include <stdio.h>
#include <string.h>
#include "hemp/sources.h"
#include "hemp/debug.h"


/*--------------------------------------------------------------------------
 * static scheme definitions 
 *-------------------------------------------------------------------------*/

static struct hemp_scheme
    hemp_scheme_text = { 
        HEMP_TEXT,
        &hemp_scheme_text_namer,
        &hemp_scheme_text_checker,
        &hemp_scheme_text_reader
    };

static struct hemp_scheme
    hemp_scheme_file = { 
        HEMP_FILE,
        &hemp_scheme_file_namer,
        &hemp_scheme_file_checker,
        &hemp_scheme_file_reader
    };

hemp_scheme_t HempSchemeText = &hemp_scheme_text;
hemp_scheme_t HempSchemeFile = &hemp_scheme_file;



/*--------------------------------------------------------------------------
 * source functions 
 *-------------------------------------------------------------------------*/

hemp_source_t
hemp_source(
    hemp_cstr_t scheme_name,
    hemp_cstr_t source_name
) {
    hemp_scheme_t scheme;

    // quick hack - should eventually be replaced by a hash array with 
    // methods for adding new schemes
    if (hemp_cstr_eq(scheme_name, HEMP_FILE)) {
        scheme = HempSchemeFile;
    }
    else if (hemp_cstr_eq(scheme_name, HEMP_TEXT)) {
        scheme = HempSchemeText;
    }
    else {
        debug_red("Invalid scheme: $s\n", scheme_name);
    }
    
    return hemp_source_init(scheme, source_name);
}
        
        


hemp_source_t
hemp_source_init(
    hemp_scheme_t scheme,
    hemp_cstr_t   name
) {
    hemp_source_t source;

    if ((source = (hemp_source_t) hemp_mem_init(sizeof(struct hemp_source)))) {
        if (source->name   = strdup(name)) {
            source->scheme = scheme;
            source->text   = NULL;
            debug_mem(
                "Allocated %s source at %p\n", 
                scheme->name, source
            );
        }
        else {
            hemp_source_null(source);
        }
    }

    // TODO handle source == NULL
    return source;
}


void
hemp_source_free(
    hemp_source_t source
) {
    debug_mem("Releasing %s source at %p\n", source->scheme->name, source);

    if (source->name) {
        // in the case of text sources the name and text fields both point
        // to the same string so we only the text if that's not the case
        if (source->text && source->text != source->name) {
            debug_mem("Releasing %s source text at %p\n", source->scheme->name, source->text);
            hemp_mem_free(source->text);
        }
        hemp_mem_free(source->name);
    }

    hemp_mem_free(source);
}



/*--------------------------------------------------------------------------
 * sources functions 
 *-------------------------------------------------------------------------*/
 
// hemp_source
// hemp_sources_source


/*--------------------------------------------------------------------------
 * text scheme functions 
 *-------------------------------------------------------------------------*/


hemp_cstr_t 
hemp_scheme_text_reader(
    hemp_source_t source
) {
    // I think this is the cause of SWIG failing...
    return (source->text = source->name);
}


hemp_cstr_t 
hemp_scheme_text_namer(
    hemp_source_t source
) {
    return HEMP_TEXT;
}


hemp_bool_t
hemp_scheme_text_checker(
    hemp_source_t source
) {
    return HEMP_TRUE;
}



/*--------------------------------------------------------------------------
 * file scheme functions 
 *-------------------------------------------------------------------------*/

hemp_cstr_t 
hemp_scheme_file_reader(
    hemp_source_t source 
) {
    hemp_cstr_t output = "TODO: read file";
    debug_red("TODO: hemp_scheme_file_reader()\n");
    return (source->text = strdup(output));
}


hemp_cstr_t 
hemp_scheme_file_namer(
    hemp_source_t source 
) {
    return source->name;
}


hemp_bool_t
hemp_scheme_file_checker(
    hemp_source_t source
) {
    debug_red("TODO: hemp_scheme_file_checker()\n");
    return HEMP_FALSE;
}

