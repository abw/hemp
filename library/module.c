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

    hemp_value  path = hemp_config_get(hemp, HEMP_CONFIG_MODPATH);
    hemp_string string;

    if (hemp_is_defined(path)) {
        string = hemp_to_string(path, hemp->context);
//      hemp_debug_msg("already got %s: %s\n", HEMP_CONFIG_MODPATH, string);
    }
    else {
        hemp_value  dir  = hemp_config_get(hemp, HEMP_CONFIG_DIR);
        hemp_value  mod  = hemp_config_get(hemp, HEMP_CONFIG_MODDIR);
        hemp_string dstr = hemp_to_string(dir, hemp->context);
        hemp_string mstr = hemp_to_string(mod, hemp->context);

        string = hemp_uri_path_join(dstr, mstr, 1);
//      hemp_debug_msg("constructed %s: %s\n", HEMP_CONFIG_MODPATH, string);

        /* ugly work-around so we can get the context to manage memory */
        hemp_text text = hemp_context_tmp_text_size(hemp->context, strlen(string) + 1);
        hemp_text_append_string(text, string);
        hemp_mem_free(string);
        string = text->string;
//        hemp_config_set(hemp, HEMP_CONFIG_MODPATH, hemp_text_val(text));
    }

    // TODO: need a way to save dotted path (hemp.module_path) back into config

    /* quick hack to get something working */
    hemp_string modpath = getenv("HEMP_MODULE_PATH");

    if (! modpath || ! *modpath) {
        modpath = string;
//        hemp_debug_msg("No HEMP_MODULE_PATH environment variable set\n");
//        return HEMP_FALSE;
    }

    /* TODO: sort this mess out */
    hemp_string tpath = hemp_uri_path_join(modpath, type, 1);
    hemp_string mpath = hemp_uri_path_join(tpath, name, 1);
    hemp_text   mtext = hemp_text_from_string(mpath);
    hemp_text_append_string(mtext, HEMP_MODULE_EXT);
    hemp_mem_free(mpath);
    hemp_mem_free(tpath);
//  hemp_debug_msg("constructed module path: %s\n", mtext->string);

    hemp_module module = hemp_global_module(hemp->global, mtext->string);

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

    hemp_text_free(mtext);
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
    hemp_debug_call("loading module: %s\n", module->name);

    /* just in case we've been here before */
    if (module->handle) {
//      hemp_debug_msg("module is already loaded: %s\n", module->name);
        return HEMP_TRUE;
    }
    else if (module->error) {
//      hemp_debug_msg("module has already failed: %s (%s)\n", module->name, module->error);
        return HEMP_FALSE;
    }

    module->handle = dlopen(module->name, RTLD_NOW|RTLD_GLOBAL);

    if (! module->handle) {
//      hemp_debug_msg("module failed: %s\n", module->name);
        return hemp_module_failed(
            module,
            "Failed to load %s module: %s", 
            module->name, dlerror()
        );
    }

//  hemp_debug_msg("loaded module: %s\n", module->name);

    module->loader = (hemp_loader) dlsym(module->handle, HEMP_MODULE_LOADER);
    module->binder = (hemp_binder) dlsym(module->handle, HEMP_MODULE_BINDER);

    /* customer loader can augment module and/or perform initialisation */    
    if (module->loader) {
        hemp_debug_call("calling module loader: %s", HEMP_MODULE_LOADER);
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

    // TODO: add unloader

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

