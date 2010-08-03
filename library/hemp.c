#include <hemp.h>



/*--------------------------------------------------------------------------
 * hemp object initialisation and cleanup functions
 *--------------------------------------------------------------------------*/

hemp_p
hemp_init() {
    hemp_p hemp = (hemp_p) hemp_mem_alloc( 
        sizeof(struct hemp_s) 
    );

    if (! hemp)
        hemp_mem_fail("hemp");

    hemp_init_schemes(hemp);
    hemp_init_tags(hemp);

    return hemp;
}


void
hemp_free(
    hemp_p hemp
) {
    /* free schemes */
    hemp_hash_each(hemp->schemes, &hemp_free_scheme);
    hemp_hash_free(hemp->schemes);

    /* free tags */
    hemp_hash_each(hemp->tags, &hemp_tagset_free_tag);
    hemp_hash_free(hemp->tags);

    hemp_mem_free(hemp);
}



/*--------------------------------------------------------------------------
 * scheme management
 *--------------------------------------------------------------------------*/

void
hemp_init_schemes(
    hemp_p hemp
) {
    hemp->schemes = hemp_hash_init();

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
hemp_add_scheme(
    hemp_p          hemp,
    hemp_scheme_p   scheme
) {
    hemp_scheme_p old = hemp_scheme(hemp, scheme->name);
    
    if (old)
        hemp_scheme_free(old);

    hemp_hash_store(hemp->schemes, scheme->name, scheme);
}


hemp_bool_t
hemp_free_scheme(
    hemp_hash_p         schemes,
    hemp_pos_t          position,
    hemp_hash_item_p    item
) {
    hemp_scheme_p scheme = (hemp_scheme_p) item->value;
    hemp_scheme_free(scheme);
    return HEMP_TRUE;
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
        hemp_fatal("invalid scheme: %s", scheme_name);

    return hemp_source_init(scheme, source_name);
}


/*--------------------------------------------------------------------------
 * tag management
 *--------------------------------------------------------------------------*/

void
hemp_init_tags(
    hemp_p hemp
) {
    hemp->tags = hemp_hash_init();

    hemp_add_tag(
        hemp,
        hemp_tag_init(
            "comment", 
            HEMP_INLINE,
            &hemp_scan_comment_tag,
            "[#", "#]"
        )
    );

    hemp_add_tag(
        hemp,
        hemp_tag_init(
            "control", 
            HEMP_INLINE,
            &hemp_scan_control_tag,
            "[?", "?]"
        )
    );

//    hemp_add_tag(
//        hemp,
//        hemp_tag_init(
//            "inline", 
//            HEMP_INLINE,
//            &hemp_scan_inline_tag 
//            "[%", "%]",
//        )
//    );

    hemp_add_tag(
        hemp,
        hemp_tag_init(
            "outline", 
            HEMP_OUTLINE,
            &hemp_scan_outline_tag,
            "%%", NULL
        )
    );
}


void 
hemp_add_tag(
    hemp_p       hemp,
    hemp_tag_p   tag
) {
    hemp_tag_p old = hemp_tag(hemp, tag->name);
    
    if (old)
        hemp_tag_free(old);

    hemp_hash_store(hemp->tags, tag->name, tag);
}



