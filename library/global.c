#include <hemp/core.h>
#include <hemp/namespace.h>
#include <hemp/element.h>
#include <hemp/module.h>


/*--------------------------------------------------------------------------
 * Global data structure for managing hemp instances and other shared data.
 *--------------------------------------------------------------------------*/

struct hemp_global HempGlobalData = {
    0,          /* nhemps - number of live hemp objects   */
    0,          /* namespaces allocated (+1 for next id)  */
    NULL,       /* root namespace                         */
    NULL,       /* module hash                            */
    NULL,       /* config hash                            */
    NULL        /* element namespace                      */
};


/*--------------------------------------------------------------------------
 * Global initialisation and cleanup functions
 *--------------------------------------------------------------------------*/

HempGlobal
hemp_global_data() {
    return &HempGlobalData;
}


HempGlobal
hemp_global_init() {
    HempGlobal global = &HempGlobalData;

    if (! global->n_hemps++) {
//      hemp_debug_msg("Initialising global hemp data: %p\n", global);
        hemp_global_init_namespaces(global);
        hemp_global_types_init(global);
        hemp_global_init_symbols(global);
        hemp_global_init_modules(global);
        hemp_global_init_config(global);
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
    HempGlobal global = &HempGlobalData;

    hemp_debug_init(
        "Releasing local hemp interpreter #%d\n", 
        global->n_hemps
    );

    if (! --global->n_hemps) {
        hemp_debug_init("Releasing global hemp data\n");
        hemp_global_free_config(global);
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
    HempGlobal global
) {
    /* return silently if we've already done this */
    if (global->namespace)
        return;

    global->namespace_id = 0;
    global->namespace    = hemp_namespace_init(0, "", NULL);

    hemp_debug_init("global namespace: %p\n", global->namespace);
}


void hemp_global_free_namespaces(
    HempGlobal global
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
    HempGlobal   global
) {
    hemp_debug_init("hemp_global_init_symbols()\n");

    /* return silently if it looks like we've already done this step */
    if (HempElementSpace)
        return;

    /* construct the global symbol objects */
    HempElementSpace     = hemp_global_element_space(global);
    HempElementComment   = hemp_global_element_comment(global);
    HempElementTagStart  = hemp_global_element_tag_start(global);
    HempElementTagEnd    = hemp_global_element_tag_end(global);
    HempElementBlock     = hemp_global_element_block(global);
    HempElementText      = hemp_global_element_text(global);
    HempElementLiteral   = hemp_global_element_literal(global);
    HempElementWord      = hemp_global_element_word(global);
    HempElementNumber    = hemp_global_element_number(global);
    HempElementInteger   = hemp_global_element_integer(global);
    HempElementEOF       = hemp_global_element_eof(global);
}


void
hemp_global_free_symbols(
    HempGlobal global
) {
    hemp_debug_init("hemp_global_free_symbols()\n");
    hemp_element_free(HempElementSpace);      HempElementSpace     = NULL;
    hemp_element_free(HempElementComment);    HempElementComment   = NULL;
    hemp_element_free(HempElementTagStart);   HempElementTagStart  = NULL;
    hemp_element_free(HempElementTagEnd);     HempElementTagEnd    = NULL;
    hemp_element_free(HempElementBlock);      HempElementBlock     = NULL;
    hemp_element_free(HempElementText);       HempElementText      = NULL;
    hemp_element_free(HempElementLiteral);    HempElementLiteral   = NULL;
    hemp_element_free(HempElementWord);       HempElementWord      = NULL;
    hemp_element_free(HempElementNumber);     HempElementNumber    = NULL;
    hemp_element_free(HempElementInteger);    HempElementInteger   = NULL;
    hemp_element_free(HempElementEOF);        HempElementEOF       = NULL;
}



/*--------------------------------------------------------------------------
 * Dynamic module management
 *--------------------------------------------------------------------------*/

void hemp_global_init_modules(
    HempGlobal global
) {
    hemp_debug_init("initialising modules\n");

    /* return silently if we've already done this */
    if (global->modules)
        return;

    global->modules = hemp_hash_new();
}


HempModule
hemp_global_module(
    HempGlobal     global,
    HempString     name
) {
    /* see if the module is already defined */
    HempValue modval = hemp_hash_fetch(global->modules, name);

    if (hemp_is_defined(modval))
        return (HempModule) hemp_val_ptr(modval);

    /* otherwise create and cache a module record... */
    HempModule module = hemp_module_new(name);
    hemp_hash_store(global->modules, name, hemp_ptr_val(module));

    /* ...and attempt to load it (errors are stored inside module) */
    hemp_module_load(module);

    return module;
}


void hemp_global_free_modules(
    HempGlobal global
) {
    hemp_debug_init("freeing modules\n");
    /* return silently if this has already been done */
    if (! global->modules)
        return;

    hemp_hash_each(global->modules, &hemp_global_free_module);
    hemp_hash_free(global->modules);
    global->modules = NULL;
}


HempBool
hemp_global_free_module(
    HempHash       modules,
    HempPos        position,
    HempSlot       item
) {
    hemp_module_free((HempModule) hemp_val_ptr(item->value));
    return HEMP_TRUE;
}



/*--------------------------------------------------------------------------
 * Configuration
 *--------------------------------------------------------------------------*/

void hemp_global_init_config(
    HempGlobal global
) {
    hemp_debug_init("initialising config\n");

    /* return silently if we've already done this */
    if (global->config)
        return;

    global->config = hemp_hash_new();

    /* set up the global 'hemp' configuration hash */
    HempHash hemp = hemp_hash_new();
    hemp_hash_store_hash(global->config, HEMP_NAME, hemp);

    /* add the global hemp.XXX items */
    hemp_hash_store_string(hemp, "version",     HEMP_VERSION);
    hemp_hash_store_string(hemp, "dir",         HEMP_DIR);
    hemp_hash_store_string(hemp, "module_dir",  HEMP_MODULE_DIR);
    hemp_hash_store_string(hemp, "module_ext",  HEMP_MODULE_EXT);
}


void hemp_global_free_config(
    HempGlobal global
) {
    hemp_debug_init("freeing config\n");

    /* return silently if this has already been done */
    if (! global->config)
        return;

    /* free the global 'hemp' configuration hash we created earlier */
    HempHash hemp = hemp_hash_fetch_hash(global->config, HEMP_NAME);

    if (hemp)
        hemp_hash_free(hemp);

    /* free the config hash itself */
    hemp_hash_free(global->config);
    global->config = NULL;
}
