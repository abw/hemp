#include <hemp.h>


void hemp_init_extra(hemp_p);


/*--------------------------------------------------------------------------
 * hemp object initialisation functions
 *--------------------------------------------------------------------------*/

hemp_p
hemp_init() {
    hemp_p hemp = (hemp_p) hemp_mem_alloc( 
        sizeof(struct hemp_s) 
    );

    if (! hemp)
        hemp_mem_fail("hemp");

    hemp->schemes  = hemp_hash_init();
    hemp->elements = hemp_factory_init();
    hemp->grammars = hemp_factory_init();
    hemp->dialects = hemp_factory_init();

    /* install the cleaners to automatically tidy up */
    hemp->elements->cleaner = &hemp_free_element;
    hemp->grammars->cleaner = &hemp_free_grammar;
    hemp->dialects->cleaner = &hemp_free_dialect;

    hemp_init_extra(hemp);
    hemp_init_templates(hemp);

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

    hemp_errno_e e = setjmp(hemp->jump->buffer);
    if (e) {
        hemp_fatal("uncaught error: %d", e);
    }

    return hemp;
}


void
hemp_init_extra(
    hemp_p hemp
) {
    /* extra initialisation stuff that should eventually be moved out into
     * a separate language initialisation function
     */

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

    hemp_element_constructor(
        hemp, "hemp.number.*", (hemp_actor_f) &hemp_element_number_constructor
    );

    hemp_dialect_constructor(
        hemp, HEMP_TT3, (hemp_actor_f) &hemp_dialect_tt3
    );

}


/*--------------------------------------------------------------------------
 * hemp object cleanup functions
 *--------------------------------------------------------------------------*/

void
hemp_free(
    hemp_p hemp
) {
    /* templates */
    hemp_hash_each(hemp->templates, &hemp_free_template);
    hemp_hash_free(hemp->templates);

    /* tags */
//  hemp_hash_each(hemp->tags, &hemp_tagset_free_tag);
//  hemp_hash_free(hemp->tags);

    /* schemes */
    hemp_hash_each(hemp->schemes, &hemp_free_scheme);
    hemp_hash_free(hemp->schemes);

    /* free factories */
    hemp_factory_free(hemp->elements);
    hemp_factory_free(hemp->grammars);
    hemp_factory_free(hemp->dialects);

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
}


hemp_bool_t
hemp_free_element(
    hemp_hash_p         elements,
    hemp_pos_t          position,
    hemp_hash_item_p    item
) {
//    debug("cleaning %s element\n", ((hemp_etype_p) item->value)->name);
//    hemp_element_free( (hemp_element_p) item->value );
    return HEMP_TRUE;
}


hemp_bool_t
hemp_free_dialect(
    hemp_hash_p         dialects,
    hemp_pos_t          position,
    hemp_hash_item_p    item
) {
    debug("cleaning %s dialect\n", ((hemp_dialect_p) item->value)->name);
    hemp_dialect_free( (hemp_dialect_p) item->value );
    return HEMP_TRUE;
}


hemp_bool_t
hemp_free_grammar(
    hemp_hash_p         grammars,
    hemp_pos_t          position,
    hemp_hash_item_p    item
) {
    debug("cleaning %s grammar\n", ((hemp_grammar_p) item->value)->name);
    hemp_grammar_free( (hemp_grammar_p) item->value );
    return HEMP_TRUE;
}


hemp_bool_t
hemp_free_scheme(
    hemp_hash_p         schemes,
    hemp_pos_t          position,
    hemp_hash_item_p    item
) {
    debug("cleaning %s scheme\n", ((hemp_scheme_p) item->value)->name);
    hemp_scheme_free( (hemp_scheme_p) item->value );
    return HEMP_TRUE;
}



/*--------------------------------------------------------------------------
 * dialect management
 *--------------------------------------------------------------------------*/

void 
hemp_add_dialect(
    hemp_p          hemp,
    hemp_dialect_p  dialect
) {
    hemp_dialect_p old = hemp_dialect(hemp, dialect->name);
    
    if (old)
        hemp_dialect_free(old);

    hemp_todo("hemp_add_dialect()");
    debug("adding dialect: %s\n", dialect->name);

    hemp_hash_store(hemp->dialects, dialect->name, dialect);
}


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
    
//  debug("MD5 for %s template [%s] is %s\n", scheme, source, md5.output);

    tmpl = hemp_hash_fetch(hemp->templates, md5.output);
    
    if (tmpl) {
        if (hemp_cstr_eq(tmpl->source->scheme->name, scheme)
        &&  hemp_cstr_eq(tmpl->source->name, source)) {
            // TODO: bump up LRU cache
//          debug("returning cached template\n");
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

//  debug("caching new template\n");

    /* let the source allocate memory for storing md5 permanently */
    hemp_source_set_md5(tmpl->source, md5.output);
    hemp_hash_store(hemp->templates, tmpl->source->md5, tmpl);
    
    return tmpl;
}


/*--------------------------------------------------------------------------
 * error handling
 *--------------------------------------------------------------------------*/

void 
hemp_throw(
    hemp_p       hemp,
    hemp_errno_e number,
    ...
) {
    if (number < 0 || number >= HEMP_ERROR_MAX) 
        hemp_fatal("Invalid error number: %d", number);

    hemp_cstr_p format = hemp->errmsg[number];

    /* Should we ever have NULL message entries? */
    if (! format)
        format = hemp->errmsg[HEMP_ERROR_UNKNOWN];

    if (! format)
        hemp_fatal("No error message format for error number %d", number);

    va_list args;
    va_start(args, number);
    hemp_error_p error = hemp_error_initfv(number, format, &args);
    va_end(args);

    error->parent = hemp->error;
    hemp->error = error;

    HEMP_THROW(number);
}