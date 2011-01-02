#ifndef HEMP_GLOBAL_H
#define HEMP_GLOBAL_H


/*--------------------------------------------------------------------------
 * Type definitions.
 *--------------------------------------------------------------------------*/

struct hemp_global {
    HempSize        n_hemps;
    HempU16         namespace_id;
    HempNamespace   namespace;
    HempHash        modules;
    HempHash        config;
    /* shortcut to hemp.element namespace which gets used often */
    HempNamespace   elements;
};

extern struct hemp_global HempGlobalData;


/*--------------------------------------------------------------------------
 * Function prototypes
 *--------------------------------------------------------------------------*/

HempGlobal
hemp_global_data();

HempGlobal
hemp_global_init();

void
hemp_global_free();

void 
hemp_global_init_namespaces(
    HempGlobal      global
);

void 
hemp_global_free_namespaces(
    HempGlobal      global
);

void
hemp_global_init_symbols(
    HempGlobal      global
);

void 
hemp_global_free_symbols(
    HempGlobal      global
);

void 
hemp_global_init_modules(
    HempGlobal      global
);

HempModule
hemp_global_module(
    HempGlobal      global,
    HempString      name
);

void 
hemp_global_free_modules(
    HempGlobal      global
);

HempBool
hemp_global_free_module(
    HempHash        modules,
    HempPos         position,
    HempSlot        item
);

void hemp_global_init_config(
    HempGlobal      global
);

void 
hemp_global_free_config(
    HempGlobal      global
);


#endif /* HEMP_GLOBAL_H */
