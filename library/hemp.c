#include <hemp.h>


/*--------------------------------------------------------------------------
 * hemp object initialisation functions
 *--------------------------------------------------------------------------*/

hemp_hemp
hemp_new() {
    hemp_hemp hemp;         /* So good they named it thrice! */

    HEMP_ALLOCATE(hemp);

    hemp->global = hemp_global_init();

    hemp_init_errors(hemp);
    hemp_init_factories(hemp);
    hemp_init_schemes(hemp);
    hemp_init_languages(hemp);
    hemp_init_documents(hemp);
    hemp_init_viewers(hemp);

    hemp->context = hemp_context_new(hemp);

    // YUK.  We have to do this to force all the hemp stuff to be loaded.
    // Needs work to make this more auto-load on demand.
    hemp_language_instance(hemp, "hemp");
//  hemp_debug_msg("LANGUAGE: %s v%0.2f\n", language->name, language->version);

    return hemp;
}


void
hemp_init_errors(
    hemp_hemp hemp
) {
    /* install error messages - may want to localise these one day */
    hemp->errmsg = hemp_errmsg;
    hemp->error  = NULL;

    /* define parent longjmp buffer for error handling */
    hemp->jump = (hemp_jump) hemp_mem_alloc(
        sizeof(struct hemp_jump)
    );
    if (! hemp->jump)
        hemp_mem_fail("jump buffer");
    
    hemp->jump->parent = NULL;
    hemp->jump->depth  = 0;

    hemp_errno e = setjmp(hemp->jump->buffer);

    if (e) {
        // TODO: proper handling... but if memory serves this is iffy 
        // because we've lost the hemp reference.
        hemp_fatal("uncaught error code: %d", e);
    }
}


void
hemp_init_factories(
    hemp_hemp   hemp
) {
    /* create new factory objects to manage resources */
    hemp->codec             = hemp_factory_new(hemp);
    hemp->dialect           = hemp_factory_new(hemp);
    hemp->element           = hemp_factory_new(hemp);
    hemp->grammar           = hemp_factory_new(hemp);
    hemp->language          = hemp_factory_new(hemp);
    hemp->scheme            = hemp_factory_new(hemp);
    hemp->tag               = hemp_factory_new(hemp);
    hemp->viewer            = hemp_factory_new(hemp);

    hemp->scheme->autoload      = &hemp_scheme_autoload;
    hemp->codec->autoload       = &hemp_codec_autoload;
    hemp->language->autoload    = &hemp_language_autoload;
    hemp->tag->autoload         = &hemp_tag_autoload;

    /* install the cleaners to automatically tidy up */
    hemp->dialect->cleaner  = &hemp_free_dialect;
    hemp->codec->cleaner    = &hemp_free_codec;
//  hemp->element->cleaner  = &hemp_free_element;
    hemp->grammar->cleaner  = &hemp_free_grammar;
    hemp->language->cleaner = &hemp_free_language;
    hemp->scheme->cleaner   = &hemp_free_scheme;
    hemp->tag->cleaner      = &hemp_free_tag;
    hemp->viewer->cleaner   = &hemp_free_viewer;
}


void
hemp_init_schemes(
    hemp_hemp hemp
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
    hemp_hemp hemp
) {
    // TODO: languages should be loaded on demand from modules
    // UPDATE: json is now.
    hemp_register_language(
        hemp, HEMP_HEMP, &hemp_language_hemp_new
    );
    hemp_register_language(
        hemp, HEMP_TT3, &hemp_language_tt3
    );
    hemp_register_language(
        hemp, "test", &hemp_language_test
    );
}


void
hemp_init_documents(
    hemp_hemp hemp
) {
    hemp->documents = hemp_hash_new();
}


void
hemp_init_viewers(
    hemp_hemp hemp
) {
    hemp_register_viewer(
        hemp, HEMP_TEXT, &hemp_viewer_text
    );
}



/*--------------------------------------------------------------------------
 * hemp object cleanup functions
 *--------------------------------------------------------------------------*/

void
hemp_free(
    hemp_hemp hemp
) {
    hemp_debug_call("hemp_free()\n");

    hemp_context_free(hemp->context);

    /* free all the components */
    hemp_free_documents(hemp);
    hemp_free_factories(hemp);
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
    hemp_hemp hemp
) {
    hemp_hash_each(hemp->documents, &hemp_free_document);
    hemp_hash_free(hemp->documents);
}


void
hemp_free_factories(
    hemp_hemp hemp
) {
    hemp_factory_free(hemp->viewer);
    hemp_factory_free(hemp->element);
    hemp_factory_free(hemp->grammar);
    hemp_factory_free(hemp->tag);
    hemp_factory_free(hemp->dialect);
    hemp_factory_free(hemp->language);
    hemp_factory_free(hemp->scheme);
    hemp_factory_free(hemp->codec);
}


void
hemp_free_errors(
    hemp_hemp hemp
) {
    /* free parent jump buffer, discard all others (statically allocated) */
    hemp_jump j = hemp->jump;
    while (j->parent && j->parent != j) {
        j = j->parent;
    }
    hemp_mem_free(j);

    /* free errors */
    hemp_error error = hemp->error;
    hemp_error next;

    while (error) {
        next = error->parent;
        hemp_error_free(error);
        error = next;
    }
}



/*--------------------------------------------------------------------------
 * Cleanup functions for individual component instances
 *--------------------------------------------------------------------------*/

hemp_bool
hemp_free_scheme(
    hemp_hash schemes,
    hemp_pos  position,
    hemp_slot item
) {
    hemp_debug_init("cleaning %s scheme\n", ((hemp_scheme) hemp_val_ptr(item->value))->name);
    hemp_scheme_free( (hemp_scheme) hemp_val_ptr(item->value) );
    return HEMP_TRUE;
}


hemp_bool
hemp_free_language(
    hemp_hash languages,
    hemp_pos  position,
    hemp_slot item
) {
    hemp_debug_init("cleaning %s language\n", ((hemp_dialect) item->value)->name);
    hemp_language_free( (hemp_language) hemp_val_ptr(item->value) );
    return HEMP_TRUE;
}


hemp_bool
hemp_free_codec(
    hemp_hash codecs,
    hemp_pos  position,
    hemp_slot item
) {
    hemp_codec codec = (hemp_codec) hemp_val_ptr(item->value);
    hemp_debug_msg("cleaning %s codec\n", codec->name);
    hemp_codec_free(codec);
    return HEMP_TRUE;
}


hemp_bool
hemp_free_dialect(
    hemp_hash dialects,
    hemp_pos  position,
    hemp_slot item
) {
    hemp_debug_init("cleaning %s dialect\n", ((hemp_dialect) item->value)->name);
    hemp_dialect_free( (hemp_dialect) hemp_val_ptr(item->value) );
    return HEMP_TRUE;
}


hemp_bool
hemp_free_tag(
    hemp_hash tags,
    hemp_pos  position,
    hemp_slot item
) {
    hemp_debug_init("cleaning %s tag\n", ((hemp_tag) item->value)->name);
//    hemp_tag_free( (hemp_tag) hemp_val_ptr(item->value) );
    return HEMP_TRUE;
}


hemp_bool
hemp_free_grammar(
    hemp_hash grammars,
    hemp_pos  position,
    hemp_slot item
) {
    hemp_debug_init("cleaning %s grammar\n", ((hemp_grammar) item->value)->name);
    hemp_grammar_free( (hemp_grammar) hemp_val_ptr(item->value) );
    return HEMP_TRUE;
}


//hemp_bool
//hemp_free_element(
//    hemp_hash elements,
//    hemp_pos  position,
//    hemp_slot item
//) {
//    hemp_debug_init("cleaning %s element\n", ((hemp_etype_p) item->value)->name);
////  hemp_element_free( (hemp_element) item->value );        // no, that's not right
//    return HEMP_TRUE;
//}


hemp_bool
hemp_free_document(
    hemp_hash documents,
    hemp_pos  position,
    hemp_slot item
) {
    hemp_debug_init("cleaning document\n");
    hemp_document_free( (hemp_document) hemp_val_ptr(item->value) );
    return HEMP_TRUE;
}


hemp_bool
hemp_free_viewer(
    hemp_hash viewers,
    hemp_pos  position,
    hemp_slot item
) {
    hemp_debug_init("cleaning %s viewer\n", ((hemp_eviewer) item->value)->name);
    hemp_viewer_free( (hemp_viewer) hemp_val_ptr(item->value) );
    return HEMP_TRUE;
}



/*--------------------------------------------------------------------------
 * dialect, tag and element management
 *--------------------------------------------------------------------------*/


void 
hemp_register_dialects(
    hemp_hemp       hemp,
    hemp_dialects   dialect
) {
    hemp_register_all(hemp, dialect, dialect);
}


void 
hemp_register_tags(
    hemp_hemp   hemp,
    hemp_tags   tag
) {
    hemp_register_all(hemp, tag, tag);
}


void 
hemp_register_grammars(
    hemp_hemp       hemp,
    hemp_grammars   grammar
) {
    hemp_register_all(hemp, grammar, grammar);
}


/* TODO: sort out the names: element/symbols */
void 
hemp_register_elements(
    hemp_hemp       hemp,
    hemp_elements   elements
) {
    while (elements && elements->name) {
        hemp_factory_register(
            hemp->element, elements->name, (hemp_actor) elements->ctor, hemp
        );
        elements++;
    }
}



/*--------------------------------------------------------------------------
 * document functions
 *--------------------------------------------------------------------------*/

hemp_document
hemp_document_instance(
    hemp_hemp     hemp,
    hemp_string dialect,
    hemp_string scheme,
    hemp_string source
) {
    hemp_document     document;
    static hemp_md5_t   md5;

    hemp_md5_init(&md5);
    hemp_md5_update_string(&md5, scheme);
    hemp_md5_update_string(&md5, source);
    hemp_md5_final(&md5);
    
//  hemp_debug("MD5 for %s document [%s] is %s\n", scheme, source, md5.output);

    document = hemp_hash_fetch_pointer(hemp->documents, (hemp_string) md5.output);
    
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

    document = hemp_dialect_document(
        hemp_dialect_instance(hemp, dialect),
        hemp_source_instance(hemp, scheme, source)
    );

//  hemp_debug("caching new document\n");

    /* let the source allocate memory for storing md5 permanently */
    hemp_source_md5(document->source, (hemp_string) md5.output);
    hemp_hash_store_pointer(hemp->documents, document->source->md5, document);
    
    return document;
}


/*--------------------------------------------------------------------------
 * runtime context
 *--------------------------------------------------------------------------*/

hemp_context
hemp_context_instance(
    hemp_hemp   hemp
) {
    return hemp_context_new(hemp);
}


/*--------------------------------------------------------------------------
 * error handling
 *--------------------------------------------------------------------------*/

hemp_string
hemp_error_format(
    hemp_hemp   hemp,
    hemp_errno  number
) {
    if (number < 0 || number >= HEMP_ERROR_MAX) 
        hemp_fatal("Invalid error number: %d", number);

    hemp_string format = hemp->errmsg[number];

    /* Should we ever have NULL message entries? */
    if (! format)
        format = hemp->errmsg[HEMP_ERROR_UNKNOWN];

    if (! format)
        hemp_fatal("No error message format for error number %d", number);
        
    return format;
}


hemp_error
hemp_error_message(
    hemp_hemp   hemp,
    hemp_errno  number,
    ...
) {
    hemp_debug_call("hemp_error_message()\n");
    hemp_string format = hemp_error_format(hemp, number);

    va_list args;
    va_start(args, number);
    hemp_error error = hemp_error_initfv(number, format, args);
    va_end(args);

    return error;
}


hemp_text
hemp_error_text(
    hemp_error error
) {
    hemp_document doc = error->document;
    hemp_text     text;
    hemp_string   buffer;
    
    if (doc) {
        struct hemp_str_pos spos = hemp_string_position(
            doc->source->text,
            doc->scantok
        );
        hemp_debug_msg("ok\n");

        static hemp_char extract[80];
        hemp_string s = spos.extract;
        hemp_string x = (hemp_string) extract;
        hemp_pos    n = 78;

        while (n-- && *s && *s != HEMP_LF && *s != HEMP_CR) {
            *x++ = *s++;
        }
        *x = HEMP_NUL;

        asprintf(
            &buffer, 
            "Error at line %ld, column %ld (position %ld) of %s:\n   Error: %s\n  Source: %s\n%*s^ here\n",
            spos.line, spos.column, spos.position,
            hemp_source_name(doc->source), 
            error->message, (hemp_string) extract,
            10 + spos.column, " "
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
    hemp_hemp   hemp,
    hemp_error  error
) {
    hemp_debug_call("hemp_error_throw()\n");
    error->parent = hemp->error;
    hemp->error = error;
    HEMP_THROW(error->number);
}



/*--------------------------------------------------------------------------
 * Codecs 
 *--------------------------------------------------------------------------*/

HEMP_INLINE hemp_text
hemp_encode(
    hemp_hemp       hemp,
    hemp_string     name,
    hemp_value      input,
    hemp_context    context
) {
    hemp_codec      codec = hemp_codec_instance(hemp, name);

    return hemp_codec_encode(
        codec,
        input,
        context ? context : hemp->context
    );
}


HEMP_INLINE hemp_value
hemp_decode(
    hemp_hemp       hemp,
    hemp_string     name,
    hemp_text       input,
    hemp_context    context
) {
    hemp_codec      codec = hemp_codec_instance(hemp, name);

    return hemp_codec_decode(
        codec,
        input,
        context ? context : hemp->context
    );
}





/*--------------------------------------------------------------------------
 * Miscellaneous functions
 *--------------------------------------------------------------------------*/


hemp_string
hemp_version() {
    return HEMP_VERSION;
}

