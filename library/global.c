#include <hemp/core.h>
#include <hemp/namespace.h>
#include <hemp/symbol.h>
#include <hemp/module.h>


/*--------------------------------------------------------------------------
 * Global data structure for managing hemp instances and other shared data.
 *--------------------------------------------------------------------------*/

struct hemp_global HempGlobal = {
    0,          /* nhemps - number of live hemp objects   */
    0,          /* namespaces allocated (+1 for next id)  */
    NULL,       /* root namespace hash                    */
    NULL        /* module hash                            */
};


/*--------------------------------------------------------------------------
 * Global initialisation and cleanup functions
 *--------------------------------------------------------------------------*/

hemp_global
hemp_global_init() {
    hemp_global global = &HempGlobal;

    if (! global->n_hemps++) {
        hemp_debug_init("Initialising global hemp data\n");
        hemp_global_init_namespaces(global);

        hemp_global_types_init(global);

        hemp_global_init_symbols(global);
        hemp_global_init_modules(global);
    }
    
    hemp_debug_init(
        "Initialising local hemp interpreter #%d\n", 
        global->n_hemps
    );

    return global;
}



// TODO: change this name to something that doesn't suggest the memory is
// being freed.

void
hemp_global_free() {
    hemp_global global = &HempGlobal;

    hemp_debug_init(
        "Releasing local hemp interpreter #%d\n", 
        global->n_hemps
    );

    if (! --global->n_hemps) {
        hemp_debug_init("Releasing global hemp data\n");
        hemp_global_free_modules(global);

        hemp_global_types_free(global);

        hemp_global_free_symbols(global);
        hemp_global_free_namespaces(global);


    }
}



/*--------------------------------------------------------------------------
 * namespace management
 *--------------------------------------------------------------------------*/

void hemp_global_init_namespaces(
    hemp_global global
) {
    /* return silently if we've already done this */
    if (global->namespaces)
        return;

    global->namespace_id = 0;
    global->namespaces   = hemp_hash_new();
}


void hemp_global_free_namespaces(
    hemp_global global
) {
    /* return silently if this has already been done */
    if (! global->namespaces)
        return;

    hemp_hash_each(global->namespaces, &hemp_namespace_free_child);
    hemp_hash_free(global->namespaces);
    global->namespaces   = NULL;
    global->namespace_id = 0;
}



/*--------------------------------------------------------------------------
 * global symbols
 *--------------------------------------------------------------------------*/

void
hemp_global_init_symbols(
    hemp_global   global
) {
    hemp_debug_init("hemp_global_init_symbols()\n");

    /* return silently if it looks like we've already done this step */
    if (HempSymbolSpace)
        return;

    /* construct the global symbol objects */
    HempSymbolSpace     = hemp_symbol_space();
    HempSymbolComment   = hemp_symbol_comment();
    HempSymbolTagStart  = hemp_symbol_tag_start();
    HempSymbolTagEnd    = hemp_symbol_tag_end();
    HempSymbolBlock     = hemp_symbol_block();
    HempSymbolText      = hemp_symbol_text();
    HempSymbolWord      = hemp_symbol_word();
    HempSymbolNumber    = hemp_symbol_number();
    HempSymbolInteger   = hemp_symbol_integer();
    HempSymbolEOF       = hemp_symbol_eof();
}


void
hemp_global_free_symbols(
    hemp_global global
) {
    hemp_debug_init("hemp_global_free_symbols()\n");
    hemp_symbol_free(HempSymbolSpace);      HempSymbolSpace     = NULL;
    hemp_symbol_free(HempSymbolComment);    HempSymbolComment   = NULL;
    hemp_symbol_free(HempSymbolTagStart);   HempSymbolTagStart  = NULL;
    hemp_symbol_free(HempSymbolTagEnd);     HempSymbolTagEnd    = NULL;
    hemp_symbol_free(HempSymbolBlock);      HempSymbolBlock     = NULL;
    hemp_symbol_free(HempSymbolText);       HempSymbolText      = NULL;
    hemp_symbol_free(HempSymbolWord);       HempSymbolWord      = NULL;
    hemp_symbol_free(HempSymbolNumber);     HempSymbolNumber    = NULL;
    hemp_symbol_free(HempSymbolInteger);    HempSymbolInteger   = NULL;
    hemp_symbol_free(HempSymbolEOF);        HempSymbolEOF       = NULL;
}



/*--------------------------------------------------------------------------
 * Dynamic module management
 *--------------------------------------------------------------------------*/

void hemp_global_init_modules(
    hemp_global global
) {
    hemp_debug_init("initialising modules\n");

    /* return silently if we've already done this */
    if (global->modules)
        return;

    global->modules = hemp_hash_new();
}


hemp_module
hemp_global_module(
    hemp_global     global,
    hemp_string     name
) {
    /* see if the module is already defined */
    hemp_value modval = hemp_hash_fetch(global->modules, name);

    if (hemp_is_defined(modval))
        return (hemp_module) hemp_val_ptr(modval);

    /* otherwise create and cache a module record... */
    hemp_module module = hemp_module_new(name);
    hemp_hash_store(global->modules, name, hemp_ptr_val(module));

    /* ...and attempt to load it (errors are stored inside module) */
    hemp_module_load(module);

    return module;
}


void hemp_global_free_modules(
    hemp_global global
) {
    hemp_debug_init("freeing modules\n");
    /* return silently if this has already been done */
    if (! global->modules)
        return;

    hemp_hash_each(global->modules, &hemp_global_free_module);
    hemp_hash_free(global->modules);
    global->modules = NULL;
}


hemp_bool
hemp_global_free_module(
    hemp_hash       modules,
    hemp_pos        position,
    hemp_slot       item
) {
    hemp_module_free((hemp_module) hemp_val_ptr(item->value));
    return HEMP_TRUE;
}


