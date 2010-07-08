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
    hemp_text_t scheme_name,
    hemp_text_t source_name
) {
    hemp_scheme_t scheme;

    // quick hack - should eventually be replaced by a hash array with 
    // methods for adding new schemes
    if (hemp_str_eq(scheme_name, HEMP_FILE)) {
        scheme = HempSchemeFile;
    }
    else if (hemp_str_eq(scheme_name, HEMP_TEXT)) {
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
    hemp_text_t   name
) {
    hemp_source_t source;

    if ((source = (hemp_source_t) hemp_mem_init(sizeof(struct hemp_source)))) {
        if (source->name   = strdup(name)) {
            source->scheme = scheme;
            source->text   = NULL;
            debug_cyan(
                "Allocated %s source at %p\n", 
                scheme->name, source
            );
        }
        else {
            hemp_source_free(source);
            source = NULL;
        }
    }

    // TODO handle source == NULL
    return source;
}


void
hemp_source_free(
    hemp_source_t source
) {
    if (source->name) {
        // in the case of text sources the name and text fields both point
        // to the same string so we only the text if that's not the case
        if (source->text && source->text != source->name) {
            debug_cyan("Releasing %s source text at %p\n", source->scheme->name, source->text);
            hemp_mem_free(source->text);
        }
        hemp_mem_free(source->name);
    }
    debug_cyan("Releasing %s source at %p\n", source->scheme->name, source);
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


hemp_text_t 
hemp_scheme_text_reader(
    hemp_source_t source
) {
    // This is a do-nothing stub that simple returns the input text.
    // It exists so that all schemes have a uniform access interface.
    debug_blue("hemp_scheme_text_reader()\n");
    return (source->text = source->name);
}


hemp_text_t 
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

hemp_text_t 
hemp_scheme_file_reader(
    hemp_source_t source 
) {
    hemp_text_t output = "TODO: read file";
    debug_blue("hemp_scheme_file_reader()\n");
    debug_red("TODO: file scheme reader for %s:%s\n", source->scheme->name, source->name);
    return (source->text = strdup(output));
}


hemp_text_t 
hemp_scheme_file_namer(
    hemp_source_t source 
) {
    return source->name;
}


hemp_bool_t
hemp_scheme_file_checker(
    hemp_source_t source
) {
    debug_red("TODO: file scheme checker for %s:%s\n", source->scheme->name, source->name);
    return HEMP_FALSE;
}

