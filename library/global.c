#include <hemp/core.h>
#include <hemp/namespace.h>
#include <hemp/element.h>
#include <hemp/module.h>


/*--------------------------------------------------------------------------
 * Global data structure for managing hemp instances and other shared data.
 *--------------------------------------------------------------------------*/

struct hemp_global HempGlobal = {
    0,          /* nhemps - number of live hemp objects   */
    0,          /* namespaces allocated (+1 for next id)  */
    NULL,       /* root namespace                         */
    NULL,       /* module hash                            */
    NULL        /* element namespace                      */
};


/*--------------------------------------------------------------------------
 * Global initialisation and cleanup functions
 *--------------------------------------------------------------------------*/

hemp_global
hemp_global_data() {
    return &HempGlobal;
}


hemp_global
hemp_global_init() {
    hemp_global global = &HempGlobal;

    if (! global->n_hemps++) {
//      hemp_debug_msg("Initialising global hemp data: %p\n", global);
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
    if (global->namespace)
        return;

    global->namespace_id = 0;
    global->namespace    = hemp_namespace_init(0, "", NULL);

    hemp_debug_init("global namespace: %p\n", global->namespace);
}


void hemp_global_free_namespaces(
    hemp_global global
) {
    /* return silently if this has already been done */
    if (! global->namespace)
        return;

    hemp_namespace_free(global->namespace);
    global->elements     = NULL;
    global->namespace    = NULL;
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
    if (HempElementSpace)
        return;

    /* construct the global symbol objects */
    HempElementSpace     = hemp_global_element_space();
    HempElementComment   = hemp_global_element_comment();
    HempElementTagStart  = hemp_global_element_tag_start();
    HempElementTagEnd    = hemp_global_element_tag_end();
    HempElementBlock     = hemp_global_element_block();
    HempElementText      = hemp_global_element_text();
    HempElementWord      = hemp_global_element_word();
    HempElementNumber    = hemp_global_element_number();
    HempElementInteger   = hemp_global_element_integer();
    HempElementEOF       = hemp_global_element_eof();
}


void
hemp_global_free_symbols(
    hemp_global global
) {
    hemp_debug_init("hemp_global_free_symbols()\n");
    hemp_element_free(HempElementSpace);      HempElementSpace     = NULL;
    hemp_element_free(HempElementComment);    HempElementComment   = NULL;
    hemp_element_free(HempElementTagStart);   HempElementTagStart  = NULL;
    hemp_element_free(HempElementTagEnd);     HempElementTagEnd    = NULL;
    hemp_element_free(HempElementBlock);      HempElementBlock     = NULL;
    hemp_element_free(HempElementText);       HempElementText      = NULL;
    hemp_element_free(HempElementWord);       HempElementWord      = NULL;
    hemp_element_free(HempElementNumber);     HempElementNumber    = NULL;
    hemp_element_free(HempElementInteger);    HempElementInteger   = NULL;
    hemp_element_free(HempElementEOF);        HempElementEOF       = NULL;
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


