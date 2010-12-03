#include "hemp/module.h"
#include "hemp/debug.h"

#define HEMP_ONLOAD "hemp_onload"


hemp_module
hemp_module_new(
    hemp_string     name
) {
    hemp_module module;
    HEMP_ALLOCATE(module);
    module->name = hemp_string_clone(name, "module name");
    return module;
}


void
hemp_module_free(
    hemp_module     module
) {
    hemp_mem_free(module->name);
}


hemp_bool
hemp_module_load(
    hemp_hemp    hemp,
    hemp_string  name
) {
    hemp_debug_msg("loading %s module\n");
    hemp_memory      plugin = dlopen(name, RTLD_NOW);
    hemp_memory      onload;
    hemp_string      error;

    if (plugin == NULL) {
        hemp_debug_load("failed to load plugin %s: \n", name, dlerror());
        return HEMP_FALSE;
    }

    onload = dlsym(plugin, HEMP_ONLOAD);
    
    if (onload) {
        hemp_debug_load("found %s onload function at %p\n", onload);
//        return onload(hemp);
        return HEMP_TRUE;
    }
    else {
        hemp_debug_load("Cannot find %s() function in %s: %s", HEMP_ONLOAD, name, dlerror());
        return HEMP_FALSE;
    }
}
