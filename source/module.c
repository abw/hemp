#include "hemp/module.h"
#include "hemp/debug.h"

#define HEMP_ONLOAD "hemp_onload"


hemp_bool_t
hemp_module_load(
    hemp_t          hemp,
    hemp_cstr_t     name
) {
    hemp_ptr_t      plugin = dlopen(name, RTLD_NOW);
    hemp_onload_fn  onload;
    hemp_cstr_t     error;
    
    if (plugin == NULL) {
        debug_load("failed to load plugin %s: \n", name, dlerror());
        return HEMP_FALSE;
    }

    onload = dlsym(plugin, HEMP_ONLOAD);
    if (onload) {
        debug_load("found onload function at %p\n", onload);
        return onload(hemp);
    }
    else {
        debug_load("Cannot find %s() function in %s: %s", HEMP_ONLOAD, name, dlerror());
        return HEMP_FALSE;
    }
}
