#include "hemp/module.h"
#include "hemp/debug.h"

#define HEMP_ONLOAD "hemp_onload"


hemp_module
hemp_module_new(
    hemp_string     name
) {
    hemp_module module;
    HEMP_ALLOCATE(module);
    module->name   = hemp_string_clone(name, "module name");
    module->error  = NULL;
    module->handle = NULL;
    return module;
}


void
hemp_module_free(
    hemp_module     module
) {
    if (module->handle)
        hemp_module_unload(module);

    if (module->error)
        hemp_mem_free(module->error);

    hemp_mem_free(module->name);
    hemp_mem_free(module);
}


HEMP_INLINE void
hemp_module_failed(
    hemp_module     module,
    hemp_string     error
) {
    if (module->error)
        hemp_mem_free(module->error);

    module->error = hemp_string_clone(error, "module error");
}


hemp_bool
hemp_module_load(
    hemp_module     module
) {
    hemp_debug_msg("loading %s module\n", module->name);

    /* just in case we've been here before */
    if (module->handle) {
        hemp_debug_msg("%s module is already loaded\n", module->name);
        return HEMP_TRUE;
    }
    else if (module->error) {
        hemp_debug_msg("%s module has already failed: %s\n", module->name, module->error);
        return HEMP_FALSE;
    }

    module->handle = dlopen(module->name, RTLD_NOW);

    /* winsome or failsome? */
    if (module->handle) {
        hemp_debug_msg("loaded module: %s\n", module->name);
        return HEMP_TRUE;
    }
    else {
        hemp_module_failed(module, dlerror());
        hemp_debug_msg("failed to load module: %s\n", module->error);
        return HEMP_FALSE;
    }
}


hemp_bool
hemp_module_unload(
    hemp_module     module
) {
    if (! module->handle)
        return HEMP_TRUE;
    
    if (dlclose(module->handle) == 0) {
        hemp_debug_msg("closed '%s' module library\n", module->name);
        module->handle = NULL;
        return HEMP_TRUE;
    }
    else {
        hemp_module_failed(module, dlerror());
        // TODO: should this be a fatal error or is it too late to care?
        hemp_debug_msg(
            "Failed to close '%s' module library: %s \n", 
            module->name, module->error
        );
        return HEMP_FALSE;
    }
}


/*
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
*/
