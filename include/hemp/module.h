#ifndef HEMP_MODULE_H
#define HEMP_MODULE_H

#include <hemp/core.h>
#include <hemp/context.h>
#include <hemp/uri.h>
//#include <ltdl.h>
#include <dlfcn.h>


#define HEMP_MODULE_LOADER "hemp_module_loader"
#define HEMP_MODULE_BINDER "hemp_module_binder"


/*--------------------------------------------------------------------------
 * type definitions
 *--------------------------------------------------------------------------*/

//enum hemp_module_status {
//    HEMP_MODULE_INIT,
//    HEMP_MODULE_LOADED,
//    HEMP_MODULE_FAILED
//};

struct hemp_module {
    hemp_string         name;
    hemp_string         error;
    hemp_memory         handle;
    hemp_loader         loader;
    hemp_binder         binder;
//  hemp_module_status  status;
};


/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

hemp_module
hemp_use_module(
    hemp_hemp       hemp,
    hemp_string     type,
    hemp_string     name
);


hemp_module
hemp_module_new(
    hemp_string     name
);


void
hemp_module_free(
    hemp_module     module
);


hemp_bool
hemp_module_load(
    hemp_module     module
);

hemp_bool
hemp_module_unload(
    hemp_module     module
);

HEMP_INLINE hemp_bool
hemp_module_failed(
    hemp_module     module,
    hemp_string     error,
    ...
);


/*--------------------------------------------------------------------------
 * macros
 *--------------------------------------------------------------------------*/

/*
#define hemp_module_errorf(format, ...)             \
    hemp_string_sprintf(format, __VA_ARGS__);
*/

#endif /* HEMP_MODULE_H */
