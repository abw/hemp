#include <hemp.h>


/*--------------------------------------------------------------------------
 * hemp object initialisation functions
 *--------------------------------------------------------------------------*/

Hemp
hemp_new() {
    Hemp hemp = (Hemp) hemp_mem_alloc(
        sizeof(struct hemp)
    );
    if (! hemp)
        hemp_mem_fail("Hemp");

    hemp->global     = hemp_global_init();
    hemp->context    = hemp_context_new(hemp);
    hemp->filesystem = NULL;    /* prepared on demand, after configuration */

    hemp_init_errors(hemp);
    hemp_init_config(hemp);
    hemp_init_factories(hemp);
    hemp_init_schemes(hemp);
    hemp_init_languages(hemp);
    hemp_init_documents(hemp);
    hemp_init_viewers(hemp);

    // YUK.  We have to do this to force all the hemp stuff to be loaded.
    // Needs work to make this more auto-load on demand.
    hemp_language_instance(hemp, "hemp");
//  hemp_debug_msg("LANGUAGE: %s v%0.2f\n", language->name, language->version);

    return hemp;
}


void 
hemp_ready(
    Hemp hemp
) {
    hemp_debug_msg("hemp_ready()\n");
    hemp_init_filesystem(hemp);
}


void
hemp_init_errors(
    Hemp hemp
) {
    /* install error messages - may want to localise these one day */
    hemp->errmsg = hemp_errmsg;
    hemp->error  = NULL;

    /* define parent longjmp buffer for error handling */
    hemp->jump = (HempJump) hemp_mem_alloc(
        sizeof(struct hemp_jump)
    );
    if (! hemp->jump)
        hemp_mem_fail("jump buffer");
    
    hemp->jump->parent = NULL;
    hemp->jump->depth  = 0;

    HempErrno e = setjmp(hemp->jump->buffer);

    if (e) {
        // TODO: proper handling... but if memory serves this is iffy 
        // because we've lost the hemp reference.
        hemp_fatal("uncaught error code: %d", e);
    }
}


void
hemp_init_config(
    Hemp   hemp
) {
    hemp->config            = hemp_hash_new();
    hemp->config->parent    = hemp->global->config;
}


void
hemp_init_factories(
    Hemp   hemp
) {
    /* create new meta-factory to manage factory objects that in turn manage
     * different hemp resources: codecs, dialects, elements, grammars, etc.
     * This is NEW code.
     */
    hemp->factory   = hemp_meta_factory(hemp, "factory");

    /* create new factory objects to manage resources */
    /* This is OLD code */
    hemp->codec     = hemp_codec_factory    (hemp, "codec");
    hemp->dialect   = hemp_dialect_factory  (hemp, "dialect");
    hemp->element   = hemp_element_factory  (hemp, "element");
    hemp->feature   = hemp_feature_factory  (hemp, "feature");
    hemp->grammar   = hemp_grammar_factory  (hemp, "grammar");
    hemp->language  = hemp_language_factory (hemp, "language");
    hemp->scheme    = hemp_scheme_factory   (hemp, "scheme");
    hemp->tag       = hemp_tag_factory      (hemp, "tag");
    hemp->viewer    = hemp_viewer_factory   (hemp, "viewer");
}


void
hemp_init_schemes(
    Hemp hemp
) {
    hemp_register_scheme(
        hemp, HEMP_TEXT, &hemp_scheme_text_new
    );
    hemp_register_scheme(
        hemp, HEMP_FILE, &hemp_scheme_file_new
    );
}


void
hemp_init_languages(
    Hemp hemp
) {
    // TODO: languages should be loaded on demand from modules
    // UPDATE: json is now.
    hemp_register_language(
        hemp, HEMP_HEMP, &hemp_language_hemp_new
    );
    hemp_register_language(
        hemp, HEMP_TT3, &hemp_language_tt3
    );
}


void
hemp_init_documents(
    Hemp hemp
) {
    hemp->documents = hemp_hash_new();
}


void
hemp_init_viewers(
    Hemp hemp
) {
    hemp_register_viewer(
        hemp, HEMP_TEXT, &hemp_viewer_text
    );
}


void
hemp_init_filesystem(
    Hemp hemp
) {
    hemp_debug_msg("hemp_init_filesystem()\n");
    HempValue  dir   = hemp_config_get(hemp, HEMP_CONFIG_DIR);
    HempValue  path  = hemp_config_get(hemp, HEMP_CONFIG_PATH);
    HempList   paths = hemp_list_new();
    HempString base  = hemp_to_string(dir, hemp->context);

    if (hemp->filesystem)
        hemp_filesystem_free(hemp->filesystem);

    hemp->filesystem = hemp_filesystem_new(hemp);

    hemp_debug_msg("base dir: %s\n", base);

    if (hemp_is_defined(path)) {
        hemp_debug_msg("looking for paths\n");
        hemp_values(path, hemp->context, hemp_list_val(paths));
        hemp_debug_msg("got %d paths\n", paths->length);
        
        // I think these should be relative to cwd rather than base
        
        int n;
        for (n = 0; n < paths->length; n++) {
            HempValue  v = hemp_list_item(paths, n);
            HempString s = hemp_to_string(v, hemp->context);
            HempString p = hemp_uri_path_join(base, s, 1);
            printf("%d: %s    [%s] + [%s] = [%s]\n", n, s, base, s, p);
            hemp_mem_free(p);
        }
    }

    hemp_list_free(paths);
}


/*--------------------------------------------------------------------------
 * hemp object cleanup functions
 *--------------------------------------------------------------------------*/

void
hemp_free(
    Hemp hemp
) {
    hemp_debug_call("hemp_free()\n");

    hemp_context_free(hemp->context);

    /* free all the components */
    hemp_free_filesystem(hemp);
    hemp_free_documents(hemp);
    hemp_free_factories(hemp);
    hemp_free_config(hemp);
    hemp_free_errors(hemp);

    /* tags */
    //  hemp_hash_each(hemp->tags, &hemp_tagset_free_tag);
    //  hemp_hash_free(hemp->tags);

    /* hemp */
    hemp_mem_free(hemp);

    /* release global data if we're the last hemp object */
    hemp_global_free();
}


void
hemp_free_documents(
    Hemp hemp
) {
    hemp_hash_each(hemp->documents, &hemp_free_document);
    hemp_hash_free(hemp->documents);
}


void
hemp_free_factories(
    Hemp hemp
) {
    hemp_factory_free(hemp->factory);
    hemp_factory_free(hemp->viewer);
    hemp_factory_free(hemp->element);
    hemp_factory_free(hemp->grammar);
    hemp_factory_free(hemp->feature);
    hemp_factory_free(hemp->tag);
    hemp_factory_free(hemp->dialect);
    hemp_factory_free(hemp->language);
    hemp_factory_free(hemp->scheme);
    hemp_factory_free(hemp->codec);
}


void
hemp_free_config(
    Hemp hemp
) {
    hemp_hash_free(hemp->config);
}


void
hemp_free_errors(
    Hemp hemp
) {
    /* free parent jump buffer, discard all others (statically allocated) */
    HempJump j = hemp->jump;
    while (j->parent && j->parent != j) {
        j = j->parent;
    }
    hemp_mem_free(j);

    /* free errors */
    HempError error = hemp->error;
    HempError next;

    while (error) {
        next = error->parent;
        hemp_error_free(error);
        error = next;
    }
}


void
hemp_free_filesystem(
    Hemp hemp
) {
    if (hemp->filesystem)
        hemp_filesystem_free(hemp->filesystem);
}


/*--------------------------------------------------------------------------
 * Cleanup functions for individual component instances
 *--------------------------------------------------------------------------*/


HEMP_HASH_ITERATOR(hemp_free_document) {
    hemp_debug_init("cleaning document\n");
    hemp_document_free( (HempDocument) hemp_val_ptr(item->value) );
    return HEMP_TRUE;
}



/*--------------------------------------------------------------------------
 * dialect, tag and element management
 *--------------------------------------------------------------------------*/


void 
hemp_register_dialects(
    Hemp       hemp,
    HempDialects   dialect
) {
    hemp_register_all(hemp, dialect, dialect);
}


void 
hemp_register_tags(
    Hemp   hemp,
    HempTags   tag
) {
    hemp_register_all(hemp, tag, tag);
}


void 
hemp_register_grammars(
    Hemp       hemp,
    HempGrammars   grammar
) {
    hemp_register_all(hemp, grammar, grammar);
}


/* TODO: sort out the names: element/symbols */
void 
hemp_register_elements(
    Hemp       hemp,
    HempElements   elements
) {
    while (elements && elements->name) {
        hemp_factory_register(
            hemp->element, elements->name, (HempActor) elements->ctor, hemp
        );
        elements++;
    }
}



/*--------------------------------------------------------------------------
 * document functions
 *--------------------------------------------------------------------------*/

HempDocument
hemp_document_instance(
    Hemp   hemp,
    HempString dialect,
    HempString scheme,
    HempString source
) {
    HempDocument document;
    static struct hemp_md5 md5;

    hemp_md5_init(&md5);
    hemp_md5_update_string(&md5, scheme);
    hemp_md5_update_string(&md5, source);
    hemp_md5_final(&md5);
    
//  hemp_debug("MD5 for %s document [%s] is %s\n", scheme, source, md5.output);

    document = hemp_hash_fetch_pointer(hemp->documents, (HempString) md5.output);

    if (document) {
        if (hemp_string_eq(document->source->scheme->name, scheme)
        &&  hemp_string_eq(document->source->name, source)) {
            // TODO: bump up LRU cache
//          hemp_debug("returning cached document\n");
            return document;
        }
        else {
            /* different document with MD5 hash collision - free old one */
            hemp_document_free(document);
        }
    }

    /* NOTE: do this first - otherwise there's a potential memory leak 
     * in the instantiated source instance.
     */
    HempDialect dialect_inst = hemp_dialect_instance(hemp, dialect);

    document = hemp_dialect_document(
        dialect_inst,
        hemp_source_instance(hemp, scheme, source)
    );

//  hemp_debug("caching new document\n");

    /* let the source allocate memory for storing md5 permanently */
    hemp_source_md5(document->source, (HempString) md5.output);
    hemp_hash_store_pointer(hemp->documents, document->source->md5, document);
    
    return document;
}


/*--------------------------------------------------------------------------
 * runtime context
 *--------------------------------------------------------------------------*/

HempContext
hemp_context_instance(
    Hemp   hemp
) {
    return hemp_context_new(hemp);
}



/*--------------------------------------------------------------------------
 * configuration values
 *--------------------------------------------------------------------------*/

void
hemp_configure(
    Hemp   hemp,
    HempValue  config
) {
    /* ask the config value to yield pairs (x => y) into the config hash */
    hemp_call(config, pairs, hemp->context, hemp_hash_val(hemp->config));
}


void
hemp_configure_from(
    Hemp   hemp,
    HempString dialect,
    HempString scheme,
    HempString source
) {
    HempDocument document = hemp_document_instance(
        hemp, dialect, scheme, source
    );

    /* ask the document to yield pairs (x => y) into the config hash */
    hemp_document_pairs(
        hemp_ptr_val(document),
        hemp->context,
        hemp_hash_val(hemp->config)
    );
}


HempString
hemp_config_get_string(
    Hemp   hemp,
    HempString name
) {
    HempValue  value  = hemp_config_get(hemp, name);
    HempString string = NULL;

    if (hemp_is_defined(value))
        string = hemp_to_string(value, hemp->context);

    return string;
}


HempFilesystem
hemp_filesystem_instance(
    Hemp   hemp
) {
    if (! hemp->filesystem)
        hemp_init_filesystem(hemp);

    return hemp->filesystem;
}


/*--------------------------------------------------------------------------
 * error handling
 *--------------------------------------------------------------------------*/

HempString
hemp_error_format(
    Hemp   hemp,
    HempErrno  number
) {
    if (number < 0 || number >= HEMP_ERROR_MAX) 
        hemp_fatal("Invalid error number: %d", number);

    HempString format = hemp->errmsg[number];

    /* Should we ever have NULL message entries? */
    if (! format)
        format = hemp->errmsg[HEMP_ERROR_UNKNOWN];

    if (! format)
        hemp_fatal("No error message format for error number %d", number);
        
    return format;
}


HempError
hemp_error_message(
    Hemp   hemp,
    HempErrno  number,
    ...
) {
    hemp_debug_call("hemp_error_message()\n");
    HempString format = hemp_error_format(hemp, number);

    va_list args;
    va_start(args, number);
    HempError error = hemp_error_initfv(number, format, args);
    va_end(args);

    return error;
}


HempText
hemp_error_text(
    HempError error
) {
    HempDocument doc = error->document;
    HempText     text;
    HempLocation location;
    HempString   buffer;
    
    if (doc) {
        // new way - caller sets location
        location = error->location;
        
        if (! location) {
            hemp_debug_msg("WARNING: using old-school scan position for error\n"); 
            // old way - only works for scan time errors
            location = error->location = hemp_string_location(
                doc->source->text,
                doc->scantok,
                NULL
            );
        }

        // TODO: clean this up
        static HempChar extract[80];
        HempString s = location->extract;
        HempString x = (HempString) extract;
        HempPos    n = 78;

        while (n-- && *s && *s != HEMP_LF && *s != HEMP_CR) {
            *x++ = *s++;
        }
        *x = HEMP_NUL;

        asprintf(
            &buffer, 
            "Error at line %ld, column %ld of %s:\n   Error: %s\n  Source: %s\n%*s^ here\n",
            location->line, location->column,
            hemp_source_name(doc->source), 
            error->message, (HempString) extract,
            (int) (10 + location->column), " "
        );

        text = hemp_text_from_string(buffer);
    }
    else {
        text = hemp_text_from_string(error->message);
    }
    
    return text;
}


void 
hemp_error_throw(
    Hemp   hemp,
    HempError  error
) {
    hemp_debug_call("hemp_error_throw()\n");
    error->parent = hemp->error;
    hemp->error   = error;
//    HEMP_JUMP(HEMP_JUMP_ERROR);
    HEMP_THROW(error->number);
}



/*--------------------------------------------------------------------------
 * Codecs 
 *--------------------------------------------------------------------------*/

HEMP_INLINE HempText
hemp_encode(
    Hemp       hemp,
    HempString     name,
    HempValue      input,
    HempContext    context
) {
    HempCodec      codec = hemp_codec_instance(hemp, name);

    return hemp_codec_encode(
        codec,
        input,
        context ? context : hemp->context
    );
}


HEMP_INLINE HempValue
hemp_decode(
    Hemp       hemp,
    HempString     name,
    HempText       input,
    HempContext    context
) {
    HempCodec      codec = hemp_codec_instance(hemp, name);

    return hemp_codec_decode(
        codec,
        input,
        context ? context : hemp->context
    );
}





/*--------------------------------------------------------------------------
 * Miscellaneous functions
 *--------------------------------------------------------------------------*/


HempString
hemp_version() {
    return HEMP_VERSION;
}

