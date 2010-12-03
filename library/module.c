#include "hemp/module.h"
#include "hemp/debug.h"

#define HEMP_ONLOAD "hemp_onload"


hemp_bool
hemp_module_load(
    hemp_hemp    hemp,
    hemp_string  name
) {
    hemp_memory      plugin = dlopen(name, RTLD_NOW);
    hemp_text        loader = hemp_text_from_string("hemp_onload_");
    hemp_memory      onload;
    hemp_string      error;
    
    if (plugin == NULL) {
        hemp_debug_load("failed to load plugin %s: \n", name, dlerror());
        return HEMP_FALSE;
    }

    hemp_text_append_string(loader, name);
    hemp_debug_msg("onload handler: %s\n", loader->string);

    onload = dlsym(plugin, loader->string);
    
    hemp_text_free(loader);

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
