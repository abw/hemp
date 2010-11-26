#include <hemp.h>


/*--------------------------------------------------------------------------
 * hemp object initialisation functions
 *--------------------------------------------------------------------------*/

hemp_hemp
hemp_init() {
    hemp_global_init();

    hemp_hemp hemp = (hemp_hemp) hemp_mem_alloc( 
        sizeof(struct hemp_hemp) 
    );

    if (! hemp)
        hemp_mem_fail("hemp");

    hemp->schemes       = hemp_hash_init();
    hemp->languages     = hemp_factory_new();
    hemp->dialects      = hemp_factory_new();
    hemp->grammars      = hemp_factory_new();
    hemp->elements      = hemp_factory_new();
    hemp->viewers       = hemp_factory_new();

    /* install the cleaners to automatically tidy up */
    hemp->languages->cleaner = &hemp_free_language;
    hemp->dialects->cleaner  = &hemp_free_dialect;
    hemp->grammars->cleaner  = &hemp_free_grammar;
    hemp->elements->cleaner  = &hemp_free_element;
    hemp->viewers->cleaner   = &hemp_free_viewer;

    hemp_init_schemes(hemp);
    hemp_init_templates(hemp);
    hemp_init_errors(hemp);

//    hemp_language_hemp_inite_hemp_init(hemp);
    HEMP_LANGUAGE("hemp", &hemp_language_hemp_init);
    HEMP_LANGUAGE("tt3",  &hemp_language_tt3_init);
    HEMP_LANGUAGE("test", &hemp_language_test);

    hemp_language_p language = hemp_language(hemp, "hemp");
//  debug_magenta("LANGUAGE: %s v%0.2f\n", language->name, language->version);

    HEMP_VIEWER("text", &hemp_viewer_text_init);

    return hemp;
}


void
hemp_init_schemes(
    hemp_hemp hemp
) {
    hemp_add_scheme(
        hemp,
        hemp_scheme_new(
            HEMP_TEXT,
            &hemp_scheme_text_namer,
            &hemp_scheme_text_checker,
            &hemp_scheme_text_reader
        )
    );

    hemp_add_scheme(
        hemp,
        hemp_scheme_new(
            HEMP_FILE,
            &hemp_scheme_file_namer,
            &hemp_scheme_file_checker,
            &hemp_scheme_file_reader
        )
    );
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

//  hemp_debug("setting error handling jump point in hemp at %p\n", hemp);

    hemp_errno e = setjmp(hemp->jump->buffer);

    if (e) {
//        hemp_debug("uncaught error in hemp at %p\n", hemp);
////        if (hemp && hemp->error && hemp->error->message) {
//        printf("error message at %p\n", hemp->error); //->message);
//            printf("error message: %s\n", hemp); //, hemp->error->message);
//            hemp_fatal(hemp->error->message);
////        }
//////        else if (e > 0 && e < HEMP_ERROR_MAX) {
//////            hemp_fatal("%s", hemp_errmsg[e], HEMP_WTFS);
//////        }
////        else {
//        
        hemp_fatal("uncaught error code: %d", e);
//        }
    }
}


/*--------------------------------------------------------------------------
 * hemp object cleanup functions
 *--------------------------------------------------------------------------*/

void
hemp_free(
    hemp_hemp hemp
) {
    hemp_debug_call("hemp_free()\n");

    /* templates */
//  hemp_debug("freeing templates\n");
    hemp_hash_each(hemp->templates, &hemp_free_template);
    hemp_hash_free(hemp->templates);

    /* tags */
    //  hemp_hash_each(hemp->tags, &hemp_tagset_free_tag);
    //  hemp_hash_free(hemp->tags);

    /* free factories */
//  hemp_debug("freeing viewer factory\n");
    hemp_factory_free(hemp->viewers);
//  hemp_debug("freeing elements factory\n");
    hemp_factory_free(hemp->elements);
//  hemp_debug("freeing grammars factory\n");
    hemp_factory_free(hemp->grammars);
//  hemp_debug("freeing dialects factory\n");
    hemp_factory_free(hemp->dialects);
//  hemp_debug("freeing language factory\n");
    hemp_factory_free(hemp->languages);

    /* schemes */
//  hemp_debug("freeing schemes\n");
    hemp_hash_each(hemp->schemes, &hemp_free_scheme);
    hemp_hash_free(hemp->schemes);

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

    /* hemp */
    hemp_mem_free(hemp);

    /* release global data if we're the last hemp object */
    hemp_global_free();
}



hemp_bool
hemp_free_scheme(
    hemp_hash schemes,
    hemp_pos  position,
    hemp_slot item
) {
//  hemp_debug("cleaning %s scheme\n", ((hemp_scheme) item->value)->name);
    hemp_scheme_free( (hemp_scheme) hemp_val_ptr(item->value) );
    return HEMP_TRUE;
}


hemp_bool
hemp_free_language(
    hemp_hash languages,
    hemp_pos  position,
    hemp_slot item
) {
    hemp_language_free( (hemp_language_p) hemp_val_ptr(item->value) );
    return HEMP_TRUE;
}


hemp_bool
hemp_free_dialect(
    hemp_hash dialects,
    hemp_pos  position,
    hemp_slot item
) {
//  hemp_debug("cleaning %s dialect\n", ((hemp_dialect) item->value)->name);
    hemp_dialect_free( (hemp_dialect) hemp_val_ptr(item->value) );
    return HEMP_TRUE;
}


hemp_bool
hemp_free_grammar(
    hemp_hash grammars,
    hemp_pos  position,
    hemp_slot item
) {
//  hemp_debug("cleaning %s grammar\n", ((hemp_grammar) item->value)->name);
    hemp_grammar_free( (hemp_grammar) hemp_val_ptr(item->value) );
    return HEMP_TRUE;
}


hemp_bool
hemp_free_element(
    hemp_hash elements,
    hemp_pos  position,
    hemp_slot item
) {
//    hemp_debug("cleaning %s element\n", ((hemp_etype_p) item->value)->name);
//    hemp_element_free( (hemp_element) item->value );
    return HEMP_TRUE;
}


hemp_bool
hemp_free_viewer(
    hemp_hash viewers,
    hemp_pos  position,
    hemp_slot item
) {
    hemp_viewer_free( (hemp_viewer_p) hemp_val_ptr(item->value) );
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
            hemp->elements, symbols->name, (hemp_actor) symbols->constructor, hemp
        );
        symbols++;
    }
}


/*--------------------------------------------------------------------------
 * dialect management
 *--------------------------------------------------------------------------*/

//void 
//hemp_add_dialect(
//    hemp_hemp          hemp,
//    hemp_dialect  dialect
//) {
//    hemp_dialect old = hemp_dialect_instance(hemp, dialect->name);
//    
//    if (old)
//        hemp_dialect_free(old);
//
//    hemp_todo("hemp_add_dialect()");
//    hemp_debug("adding dialect: %s\n", dialect->name);
//
//    hemp_hash_store(hemp->dialects, dialect->name, dialect);
//}


void 
hemp_add_scheme(
    hemp_hemp          hemp,
    hemp_scheme   scheme
) {
    hemp_scheme old = hemp_scheme_instance(hemp, scheme->name);
    
    if (old)
        hemp_scheme_free(old);

    hemp_hash_store_pointer(hemp->schemes, scheme->name, scheme);
}




/*--------------------------------------------------------------------------
 * source management
 *--------------------------------------------------------------------------*/

hemp_source
hemp_source_instance(
    hemp_hemp     hemp,
    hemp_string scheme_name,
    hemp_string source_name
) {
    hemp_scheme scheme = hemp_scheme_instance(hemp, scheme_name);

    if (! scheme)
        hemp_fatal("Invalid scheme: %s", scheme_name);

    return hemp_source_init(scheme, source_name);
}



/*--------------------------------------------------------------------------
 * template functions
 *--------------------------------------------------------------------------*/


void
hemp_init_templates(
    hemp_hemp hemp
) {
    hemp->templates = hemp_hash_init();
}


hemp_bool
hemp_free_template(
    hemp_hash templates,
    hemp_pos  position,
    hemp_slot item
) {
    hemp_template_free( (hemp_template) hemp_val_ptr(item->value) );
    return HEMP_TRUE;
}


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
    hemp_source_set_md5(tmpl->source, md5.output);
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

