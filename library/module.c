#include "hemp/module.h"
#include "hemp/debug.h"

#define HEMP_ONLOAD "hemp_onload"


hemp_module
hemp_use_module(
    hemp_hemp       hemp,
    hemp_string     name
) {
    hemp_debug_msg("hemp_use_module(%s)\n", name);
    hemp_module module = hemp_global_module(hemp->global, name);
    hemp_debug_msg("module: %p\n", module);

    if (module->plugin) {
        hemp_debug_msg("calling plugin function\n");
        module->plugin(hemp);
    }
    else if (module->error) {
        hemp_debug_msg("reporting module error\n");
        hemp_fatal(module->error);
    }
    else {
        /* should never happen - famous last word */
        hemp_fatal("No plugin function for %s module", name);
    }

    return module;
}


hemp_module
hemp_module_new(
    hemp_string     name
) {
    hemp_module module;
    HEMP_ALLOCATE(module);
    module->name   = hemp_string_clone(name, "module name");
    module->error  = NULL;
    module->handle = NULL;
    module->plugin = NULL;
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


HEMP_INLINE hemp_bool
hemp_module_failed(
    hemp_module     module,
    hemp_string     error,
    ...
) {
    if (module->error)
        hemp_mem_free(module->error);

    va_list args;
    va_start(args, error);
    module->error = hemp_string_vprintf(error, args);
    va_end(args);

    hemp_debug("ERROR: %s\n", module->error);

    return HEMP_FALSE;
}


hemp_bool
hemp_module_load(
    hemp_module     module
) {
    hemp_debug_msg("loading module: %s\n", module->name);

    /* just in case we've been here before */
    if (module->handle) {
        hemp_debug_msg("module is already loaded: %s\n", module->name);
        return HEMP_TRUE;
    }
    else if (module->error) {
        hemp_debug_msg("module has already failed: %s (%s)\n", module->name, module->error);
        return HEMP_FALSE;
    }

    module->handle = dlopen(module->name, RTLD_NOW);

    if (! module->handle) {
        return hemp_module_failed(
            module,
            "Failed to load %s module: %s", 
            module->name, dlerror()
        );
//      hemp_module_failed(module, dlerror());
//        hemp_debug_msg("failed to load module: %s\n", module->error);
//        return HEMP_FALSE;
    }

    hemp_debug_msg("loaded module: %s\n", module->name);

    module->plugin = (hemp_plugin) dlsym(module->handle, HEMP_PLUGIN_INIT);
    
    if (! module->plugin) {
        return hemp_module_failed(
            module,
            "Missing %s() function in %s module",
            HEMP_PLUGIN_INIT, module->name
        );
    }

    hemp_debug_msg("found plugin init: %p\n", module->plugin);

    return HEMP_TRUE;
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

