#include <hemp/dialect.h>


/*--------------------------------------------------------------------------
 * Factory functions for loading dialects
 *--------------------------------------------------------------------------*/

HEMP_FACTORY(hemp_dialect_factory) {
    hemp_debug_init("instantiating dialect factory\n");
    hemp_factory factory = hemp_factory_new(hemp, name);
    factory->cleaner     = &hemp_dialect_cleaner;
    factory->autoload    = NULL;        // TODO: load language
    return factory;
}


HEMP_HASH_ITERATOR(hemp_dialect_cleaner) {
    hemp_dialect dialect = (hemp_dialect) hemp_val_ptr(item->value);
    hemp_debug_init("cleaning dialect: %s\n", dialect->name);
    hemp_dialect_free(dialect);
    return HEMP_TRUE;
}


/*--------------------------------------------------------------------------
 * Dialect constructor/destructor functions.
 *--------------------------------------------------------------------------*/

hemp_dialect
hemp_dialect_new(
    hemp_hemp   hemp,
    hemp_string name
) {
    hemp_dialect dialect;
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
    hemp_dialect dialect
) {
    hemp_mem_free(dialect->name);
//  hemp_hash_free(dialect->tags);
//  hemp_tagset_free(dialect->tagset);
    hemp_mem_free(dialect);
}


hemp_document
hemp_dialect_document(
    hemp_dialect    dialect,
    hemp_source     source
) {
    hemp_document document = hemp_document_new(
        dialect, source
    );
    
    /* call any custom dialect initialiser */
    if (dialect->prepare)
        document = dialect->prepare(document);

    return document;
}

