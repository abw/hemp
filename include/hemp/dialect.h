#ifndef HEMP_DIALECT_H
#define HEMP_DIALECT_H

#include <hemp/tagset.h>
#include <hemp/template.h>


/*--------------------------------------------------------------------------
 * data structures
 *--------------------------------------------------------------------------*/

struct hemp_dialect_s {
    hemp_p          hemp;
    hemp_cstr_p     name;
    hemp_hash_p     tags;
    hemp_prep_f     prepare;
    hemp_scan_f     scanner;
    hemp_clean_f    cleanup;
//  hemp_tagset_t   tagset;
};


/*--------------------------------------------------------------------------
 * function prototypes for generic dialects
 *--------------------------------------------------------------------------*/

hemp_dialect_p
    hemp_dialect_init(
        hemp_p      hemp,                   /* parent hemp object           */
        hemp_cstr_p name                    /* dialect name                 */
    );

void
    hemp_dialect_free(
        hemp_dialect_p
    );

hemp_bool_t
    hemp_dialect_add_tag(
        hemp_cstr_p name,
        hemp_tag_p  tag
    );

hemp_template_p
    hemp_dialect_template(
        hemp_dialect_p dialect,
        hemp_source_p  source
    );


/*--------------------------------------------------------------------------
 * function prototypes for specific dialects
 *--------------------------------------------------------------------------*/

hemp_dialect_p
    hemp_dialect_tt3();

hemp_template_p
    hemp_dialect_tt3_prepare(
        hemp_template_p tmpl
    );

hemp_bool_t
    hemp_dialect_tt3_scanner(
        hemp_template_p tmpl
    );

void
    hemp_dialect_tt3_cleanup(
        hemp_template_p tmpl
    );


#endif /* HEMP_DIALECT_H */