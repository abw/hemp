#include "hemp/dialect.h"
#include "hemp/template.h"
#include "hemp/scanner.h"
#include "hemp/debug.h"
#include "hemp/types.h"

#define CR  13
#define LF  10
#define TT3 "tt3"


hemp_dialect_t 
hemp_dialect_init(
    hemp_ident_t name
) {
    hemp_dialect_t dialect;
    
    if ((dialect = (hemp_dialect_t) hemp_mem_init(sizeof(struct hemp_dialect)))) {
        dialect->name    = strdup(name);
        dialect->tagset  = hemp_tagset_init();
        dialect->scanner = &hemp_scan_text;

        if (dialect->name && dialect->tagset) {
            debug_mem("Allocated new %s dialect at %p\n", name, dialect);
        }
        else {
            hemp_dialect_null(dialect);
        }
    }
    
    // TODO: handle dialect == NULL

    return dialect;
}


void
hemp_dialect_free(
    hemp_dialect_t dialect
) {
    debug_mem("Releasing dialect at %p\n", dialect);

    if (dialect->name)
        hemp_mem_free(dialect->name);

    if (dialect->tagset)
        hemp_tagset_free(dialect->tagset);

    hemp_mem_free(dialect);
}


// tmp hack until we can build a tagset into a dialect

hemp_dialect_t 
hemp_dialect_tt3() {
    hemp_dialect_t dialect = hemp_dialect_init(TT3);
    hemp_tagset_t tagset   = dialect->tagset;
    
    hemp_tagset_add_inline_tag(tagset, HempTagInline);
    hemp_tagset_add_inline_tag(tagset, HempTagComment);
    hemp_tagset_add_inline_tag(tagset, HempTagControl);
    hemp_tagset_add_inline_tag(tagset, HempTagVariable);
    hemp_tagset_add_inline_tag(tagset, HempTagEmbed);
    hemp_tagset_add_outline_tag(tagset, HempTagOutline);
    
    return dialect;
}



