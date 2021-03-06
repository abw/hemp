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
    HempString          name;
    HempString          error;
    HempMemory          handle;
    HempLoader          loader;
    HempBinder          binder;
//  hemp_module_status  status;
};


/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

HempModule
hemp_use_module(
    Hemp            hemp,
    HempString      type,
    HempString      name
);


HempModule
hemp_module_new(
    HempString      name
);


void
hemp_module_free(
    HempModule      module
);


HempBool
hemp_module_load(
    HempModule      module
);

HempBool
hemp_module_unload(
    HempModule      module
);

HempBool
hemp_module_failed(
    HempModule      module,
    HempString      error,
    ...
);


#endif /* HEMP_MODULE_H */
