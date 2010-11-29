#include <hemp.h>


/*--------------------------------------------------------------------------
 * hemp object initialisation functions
 *--------------------------------------------------------------------------*/

hemp_hemp
hemp_new() {
    hemp_hemp hemp;         /* So good they named it thrice! */

    hemp_global_init();
    HEMP_ALLOCATE(hemp);

    hemp_init_errors(hemp);
    hemp_init_factories(hemp);
    hemp_init_schemes(hemp);
    hemp_init_languages(hemp);
    hemp_init_templates(hemp);
    hemp_init_viewers(hemp);

    // YUK.  We have to do this to force all the hemp stuff to be loaded.
    // Needs work to make this more auto-load on demand.
    hemp_language language = hemp_language_instance(hemp, "hemp");
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
    hemp->dialect           = hemp_factory_new();
    hemp->element           = hemp_factory_new();
    hemp->grammar           = hemp_factory_new();
    hemp->language          = hemp_factory_new();
    hemp->scheme            = hemp_factory_new();
    hemp->tag               = hemp_factory_new();
    hemp->viewer            = hemp_factory_new();

    /* install the cleaners to automatically tidy up */
    hemp->dialect->cleaner  = &hemp_free_dialect;
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
hemp_init_templates(
    hemp_hemp hemp
) {
    hemp->templates = hemp_hash_init();
}


void
hemp_init_viewers(
    hemp_hemp hemp
) {
    hemp_register_viewer(
        hemp, HEMP_TEXT, &hemp_viewer_text_init
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

    /* free all the components */
    hemp_free_templates(hemp);
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
hemp_free_templates(
    hemp_hemp hemp
) {
    hemp_hash_each(hemp->templates, &hemp_free_template);
    hemp_hash_free(hemp->templates);
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
    hemp_debug("cleaning %s scheme\n", ((hemp_scheme) hemp_val_ptr(item->value))->name);

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
hemp_free_template(
    hemp_hash templates,
    hemp_pos  position,
    hemp_slot item
) {
    hemp_debug_init("cleaning template\n");
    hemp_template_free( (hemp_template) hemp_val_ptr(item->value) );
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
 * element management
 *--------------------------------------------------------------------------*/

void 
hemp_register_elements(
    hemp_hemp       hemp,
    hemp_symbols    symbols
) {
    while (symbols && symbols->name) {
        hemp_factory_register(
            hemp->element, symbols->name, (hemp_actor) symbols->constructor, hemp
        );
        symbols++;
    }
}



/*--------------------------------------------------------------------------
 * template functions
 *--------------------------------------------------------------------------*/

hemp_template
hemp_template_instance(
    hemp_hemp     hemp,
    hemp_string dialect,
    hemp_string scheme,
    hemp_string source
) {
    hemp_template     tmpl;
    static hemp_md5_t   md5;

    hemp_md5_init(&md5);
    hemp_md5_update_string(&md5, scheme);
    hemp_md5_update_string(&md5, source);
    hemp_md5_final(&md5);
    
//  hemp_debug("MD5 for %s template [%s] is %s\n", scheme, source, md5.output);

    tmpl = hemp_hash_fetch_pointer(hemp->templates, md5.output);
    
    if (tmpl) {
        if (hemp_string_eq(tmpl->source->scheme->name, scheme)
        &&  hemp_string_eq(tmpl->source->name, source)) {
            // TODO: bump up LRU cache
//          hemp_debug("returning cached template\n");
            return tmpl;
        }
        else {
            /* different template with MD5 hash collision - free old one */
            hemp_template_free(tmpl);
        }
    }

    tmpl = hemp_dialect_template(
        hemp_dialect_instance(hemp, dialect),
        hemp_source_instance(hemp, scheme, source)
    );

//  hemp_debug("caching new template\n");

    /* let the source allocate memory for storing md5 permanently */
    hemp_source_md5(tmpl->source, md5.output);
    hemp_hash_store_pointer(hemp->templates, tmpl->source->md5, tmpl);
    
    return tmpl;
}


/*--------------------------------------------------------------------------
 * runtime context
 *--------------------------------------------------------------------------*/

hemp_context
hemp_context_instance(
    hemp_hemp   hemp
) {
    return hemp_context_init(hemp);
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
    hemp_scan_pos sp = error->scan_pos;
    hemp_text text;
    hemp_string  buffer;
    
    if (sp) {
        asprintf(
            &buffer, 
            "error at pos %d of %s:\n   Error: %s\n  Source: %s",
            sp->pos, hemp_source_name(sp->tmpl->source), error->message,
            sp->start
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
    hemp_hemp       hemp,
    hemp_error error
) {
    hemp_debug_call("hemp_error_throw()\n");
    error->parent = hemp->error;
    hemp->error = error;
    HEMP_THROW(error->number);
}



/*--------------------------------------------------------------------------
 * Miscellaneous functions
 *--------------------------------------------------------------------------*/

hemp_string
hemp_version() {
    return HEMP_VERSION;
}

