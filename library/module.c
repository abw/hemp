#include "hemp/module.h"
#include "hemp/debug.h"

#define HEMP_ONLOAD "hemp_onload"


hemp_module
hemp_use_module(
    hemp_hemp       hemp,
    hemp_string     type,
    hemp_string     name
) {
    hemp_debug_call("hemp_use_module(%s => %s)\n", type, name);

    /* quick hack to get something working */
    hemp_string modpath = getenv("HEMP_MODULE_PATH");

    if (! modpath) {
        hemp_debug_msg("No HEMP_MODULE_PATH environment variable set\n");
        return HEMP_FALSE;
    }

    hemp_text path = hemp_text_from_string(modpath);
    hemp_text_append_string(path, "/");
    hemp_text_append_string(path, type);
    hemp_text_append_string(path, "/");
    hemp_text_append_string(path, name);
    hemp_text_append_string(path, HEMP_MODULE_EXT);
    hemp_debug_msg("constructed module path: %s\n", path->string);

    hemp_module module = hemp_global_module(hemp->global, path->string);

    if (module->binder) {
        module->binder(module, hemp);
    }
    else if (module->error) {
        hemp_fatal(module->error);
    }
    else {
        /* should never happen - famous last word */
        hemp_fatal("No binder function for %s module", name);
    }

    hemp_text_free(path);
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
    module->loader = NULL;
    module->binder = NULL;
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

//  hemp_debug("ERROR: %s\n", module->error);

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
        hemp_debug_msg("module failed: %s\n", module->name);
        return hemp_module_failed(
            module,
            "Failed to load %s module: %s", 
            module->name, dlerror()
        );
    }

  hemp_debug_msg("loaded module: %s\n", module->name);

    module->loader = (hemp_loader) dlsym(module->handle, HEMP_MODULE_LOADER);
    module->binder = (hemp_binder) dlsym(module->handle, HEMP_MODULE_BINDER);

    /* customer loader can augment module and/or perform initialisation */    
    if (module->loader) {
        hemp_debug_msg("calling module loader: %s", HEMP_MODULE_LOADER);
        if (! module->loader(module)) {
            return HEMP_FALSE;
        }
    }
    
    if (! module->binder) {
        return hemp_module_failed(
            module,
            "Missing %s() function in %s module",
            HEMP_MODULE_BINDER, module->name
        );
    }

//  hemp_debug_msg("found plugin init: %p\n", module->plugin);

    return HEMP_TRUE;
}


hemp_bool
hemp_module_unload(
    hemp_module     module
) {
    if (! module->handle)
        return HEMP_TRUE;
    
    if (dlclose(module->handle) == 0) {
        hemp_debug_init("closed '%s' module library\n", module->name);
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

