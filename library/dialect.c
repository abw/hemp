#include <hemp/dialect.h>


/*--------------------------------------------------------------------------
 * Dialect constructor/destructor functions.
 *--------------------------------------------------------------------------*/

HempDialect
hemp_dialect_new(
    Hemp   hemp,
    HempString name
) {
    HempDialect dialect;
    HEMP_ALLOCATE(dialect);
    dialect->prepare = NULL;
//  dialect->scanner = NULL;
    dialect->cleanup = NULL;
    dialect->hemp    = hemp;
    dialect->name    = hemp_string_clone(name, "dialect name");
//  dialect->tags    = hemp_hash_new();
//  dialect->tagset  = hemp_tagset_new();
//  dialect->scanner = &hemp_scan_text;
    return dialect;
}


void
hemp_dialect_free(
    HempDialect dialect
) {
    hemp_mem_free(dialect->name);
//  hemp_hash_free(dialect->tags);
//  hemp_tagset_free(dialect->tagset);
    hemp_mem_free(dialect);
}


HempDocument
hemp_dialect_document(
    HempDialect    dialect,
    HempSource     source
) {
    HempDocument document = hemp_document_new(
        dialect, source
    );
    
    /* call any custom dialect initialiser */
    if (dialect->prepare)
        document = dialect->prepare(document);

    return document;
}

