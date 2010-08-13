#include <hemp.h>

struct hemp_global_s hemp_global = {
    0                           /* nhemps - number of live hemp objects   */
};
    

/*--------------------------------------------------------------------------
 * hemp global initialisation and cleanup functions
 *--------------------------------------------------------------------------*/

hemp_size_t
hemp_global_init() {
    if (! hemp_global.n_hemps++) {
        hemp_debug_init("Initialising global hemp data\n");
        /* TODO: do init */
    }
    
    hemp_debug_init(
        "Initialising local hemp interpreter #%d\n", 
        hemp_global.n_hemps
    );

    return hemp_global.n_hemps;
}

hemp_size_t
hemp_global_free() {
    hemp_debug_init(
        "Releasing local hemp interpreter #%d\n", 
        hemp_global.n_hemps
    );

    if (! --hemp_global.n_hemps) {
        hemp_debug_init("Releasing global hemp data\n");
    }

    return hemp_global.n_hemps;
}


/*--------------------------------------------------------------------------
 * hemp object initialisation functions
 *--------------------------------------------------------------------------*/

hemp_p
hemp_init() {
    hemp_global_init();

    hemp_p hemp = (hemp_p) hemp_mem_alloc( 
        sizeof(struct hemp_s) 
    );

    if (! hemp)
        hemp_mem_fail("hemp");

    hemp->schemes   = hemp_hash_init();
    hemp->languages = hemp_factory_init();
    hemp->dialects  = hemp_factory_init();
    hemp->grammars  = hemp_factory_init();
    hemp->elements  = hemp_factory_init();

    /* install the cleaners to automatically tidy up */
    hemp->languages->cleaner = &hemp_free_language;
    hemp->dialects->cleaner  = &hemp_free_dialect;
    hemp->grammars->cleaner  = &hemp_free_grammar;
    hemp->elements->cleaner  = &hemp_free_element;

    hemp_init_schemes(hemp);
    hemp_init_templates(hemp);
    hemp_init_errors(hemp);

//    hemp_language_hemp_inite_hemp_init(hemp);
    HEMP_LANGUAGE("hemp", &hemp_language_hemp_init);
    HEMP_LANGUAGE("tt3",  &hemp_language_tt3_init);

    hemp_language_p language = hemp_language(hemp, "hemp");
//  debug_magenta("LANGUAGE: %s v%0.2f\n", language->name, language->version);

    return hemp;
}


void
hemp_init_schemes(
    hemp_p hemp
) {
    hemp_add_scheme(
        hemp,
        hemp_scheme_init(
            HEMP_TEXT,
            &hemp_scheme_text_namer,
            &hemp_scheme_text_checker,
            &hemp_scheme_text_reader
        )
    );

    hemp_add_scheme(
        hemp,
        hemp_scheme_init(
            HEMP_FILE,
            &hemp_scheme_file_namer,
            &hemp_scheme_file_checker,
            &hemp_scheme_file_reader
        )
    );
}


void
hemp_init_errors(
    hemp_p hemp
) {
    /* install error messages - may want to localise these one day */
    hemp->errmsg = hemp_errmsg;
    hemp->error  = NULL;

    /* define parent longjmp buffer for error handling */
    hemp->jump = (hemp_jump_p) hemp_mem_alloc(
        sizeof(hemp_jump_t)
    );
    if (! hemp->jump)
        hemp_mem_fail("jump buffer");
    
    hemp->jump->parent = NULL;
    hemp->jump->depth  = 0;

//  hemp_debug("setting error handling jump point in hemp at %p\n", hemp);

    hemp_errno_e e = setjmp(hemp->jump->buffer);

    if (e) {
//        hemp_debug("uncaught error in hemp at %p\n", hemp);
//        if (hemp->error && hemp->error->message) {
//            hemp_fatal(hemp->error->message);
//        }
////        else if (e > 0 && e < HEMP_ERROR_MAX) {
////            hemp_fatal("%s", hemp_errmsg[e], HEMP_WTFS);
////        }
//        else {
            hemp_fatal("uncaught error code: %d", e);
//        }
    }
}


/*--------------------------------------------------------------------------
 * hemp object cleanup functions
 *--------------------------------------------------------------------------*/

void
hemp_free(
    hemp_p hemp
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
    hemp_jump_p j = hemp->jump;
    while (j->parent && j->parent != j) {
        j = j->parent;
    }
    hemp_mem_free(j);

    /* free errors */
    hemp_error_p error = hemp->error;
    hemp_error_p next;

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



hemp_bool_t
hemp_free_scheme(
    hemp_hash_p         schemes,
    hemp_pos_t          position,
    hemp_hash_item_p    item
) {
//  hemp_debug("cleaning %s scheme\n", ((hemp_scheme_p) item->value)->name);
    hemp_scheme_free( (hemp_scheme_p) item->value );
    return HEMP_TRUE;
}


hemp_bool_t
hemp_free_language(
    hemp_hash_p         languages,
    hemp_pos_t          position,
    hemp_hash_item_p    item
) {
    hemp_language_free( (hemp_language_p) item->value );
    return HEMP_TRUE;
}


hemp_bool_t
hemp_free_dialect(
    hemp_hash_p         dialects,
    hemp_pos_t          position,
    hemp_hash_item_p    item
) {
//  hemp_debug("cleaning %s dialect\n", ((hemp_dialect_p) item->value)->name);
    hemp_dialect_free( (hemp_dialect_p) item->value );
    return HEMP_TRUE;
}


hemp_bool_t
hemp_free_grammar(
    hemp_hash_p         grammars,
    hemp_pos_t          position,
    hemp_hash_item_p    item
) {
//  hemp_debug("cleaning %s grammar\n", ((hemp_grammar_p) item->value)->name);
    hemp_grammar_free( (hemp_grammar_p) item->value );
    return HEMP_TRUE;
}


hemp_bool_t
hemp_free_element(
    hemp_hash_p         elements,
    hemp_pos_t          position,
    hemp_hash_item_p    item
) {
//    hemp_debug("cleaning %s element\n", ((hemp_etype_p) item->value)->name);
//    hemp_element_free( (hemp_element_p) item->value );
    return HEMP_TRUE;
}




/*--------------------------------------------------------------------------
 * element management
 *--------------------------------------------------------------------------*/

void 
hemp_register_elements(
    hemp_p         hemp, 
    hemp_symbols_p symbols
) {
    while (symbols && symbols->name) {
        hemp_factory_register(
            hemp->elements, symbols->name, (hemp_actor_f) symbols->constructor, hemp
        );
        symbols++;
    }
}


/*--------------------------------------------------------------------------
 * dialect management
 *--------------------------------------------------------------------------*/

//void 
//hemp_add_dialect(
//    hemp_p          hemp,
//    hemp_dialect_p  dialect
//) {
//    hemp_dialect_p old = hemp_dialect(hemp, dialect->name);
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
    hemp_p          hemp,
    hemp_scheme_p   scheme
) {
    hemp_scheme_p old = hemp_scheme(hemp, scheme->name);
    
    if (old)
        hemp_scheme_free(old);

    hemp_hash_store(hemp->schemes, scheme->name, scheme);
}




/*--------------------------------------------------------------------------
 * source management
 *--------------------------------------------------------------------------*/

hemp_source_p
hemp_source(
    hemp_p      hemp,
    hemp_cstr_p scheme_name,
    hemp_cstr_p source_name
) {
    hemp_scheme_p scheme = hemp_scheme(hemp, scheme_name);

    if (! scheme)
        hemp_fatal("Invalid scheme: %s", scheme_name);

    return hemp_source_init(scheme, source_name);
}



/*--------------------------------------------------------------------------
 * template functions
 *--------------------------------------------------------------------------*/


void
hemp_init_templates(
    hemp_p hemp
) {
    hemp->templates = hemp_hash_init();
}


hemp_bool_t
hemp_free_template(
    hemp_hash_p         templates,
    hemp_pos_t          position,
    hemp_hash_item_p    item
) {
    hemp_template_free( (hemp_template_p) item->value );
    return HEMP_TRUE;
}


hemp_template_p
hemp_template(
    hemp_p      hemp,
    hemp_cstr_p dialect,
    hemp_cstr_p scheme,
    hemp_cstr_p source
) {
    hemp_template_p     tmpl;
    static hemp_md5_t   md5;

    hemp_md5_init(&md5);
    hemp_md5_update_cstr(&md5, scheme);
    hemp_md5_update_cstr(&md5, source);
    hemp_md5_final(&md5);
    
//  hemp_debug("MD5 for %s template [%s] is %s\n", scheme, source, md5.output);

    tmpl = hemp_hash_fetch(hemp->templates, md5.output);
    
    if (tmpl) {
        if (hemp_cstr_eq(tmpl->source->scheme->name, scheme)
        &&  hemp_cstr_eq(tmpl->source->name, source)) {
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
        hemp_dialect(hemp, dialect),
        hemp_source(hemp, scheme, source)
    );

//  hemp_debug("caching new template\n");

    /* let the source allocate memory for storing md5 permanently */
    hemp_source_set_md5(tmpl->source, md5.output);
    hemp_hash_store(hemp->templates, tmpl->source->md5, tmpl);
    
    return tmpl;
}


/*--------------------------------------------------------------------------
 * error handling
 *--------------------------------------------------------------------------*/

hemp_cstr_p
hemp_error_format(
    hemp_p       hemp,
    hemp_errno_e number
) {
    if (number < 0 || number >= HEMP_ERROR_MAX) 
        hemp_fatal("Invalid error number: %d", number);

    hemp_cstr_p format = hemp->errmsg[number];

    /* Should we ever have NULL message entries? */
    if (! format)
        format = hemp->errmsg[HEMP_ERROR_UNKNOWN];

    if (! format)
        hemp_fatal("No error message format for error number %d", number);
        
    return format;
}


hemp_error_p
hemp_error_message(
    hemp_p       hemp,
    hemp_errno_e number,
    ...
) {
    hemp_debug_call("hemp_error_message()\n");
    hemp_cstr_p format = hemp_error_format(hemp, number);

    va_list args;
    va_start(args, number);
    hemp_error_p error = hemp_error_initfv(number, format, args);
    va_end(args);

    return error;
}


hemp_text_p
hemp_error_text(
    hemp_error_p error
) {
    hemp_scan_pos_p sp = error->scan_pos;
    hemp_text_p text;
    hemp_cstr_p buffer;
    
    if (sp) {
        asprintf(
            &buffer, 
            "error at pos %d of %s:\n   Error: %s\n  Source: %s",
            sp->pos, hemp_source_name(sp->tmpl->source), error->message,
            sp->start
        );
        text = hemp_text_from_cstr(buffer);
    }
    else {
        text = hemp_text_from_cstr(error->message);
    }
    
    return text;
}


void 
hemp_error_throw(
    hemp_p       hemp,
    hemp_error_p error
) {
    hemp_debug_call("hemp_error_throw()\n");
    error->parent = hemp->error;
    hemp->error = error;
    HEMP_THROW(error->number);
}


