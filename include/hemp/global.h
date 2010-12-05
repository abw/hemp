#ifndef HEMP_GLOBAL_H
#define HEMP_GLOBAL_H

struct hemp_global {
    hemp_size       n_hemps;
    hemp_u16        namespace_id;
    hemp_hash       namespaces;
    hemp_hash       modules;
};

extern struct hemp_global HempGlobal;


hemp_global
hemp_global_init();

void
hemp_global_free();

void 
hemp_global_init_namespaces(
    hemp_global     global
);

void 
hemp_global_free_namespaces(
    hemp_global     global
);

void
hemp_global_init_symbols(
    hemp_global     global
);

void 
hemp_global_free_symbols(
    hemp_global     global
);

void 
hemp_global_init_modules(
    hemp_global     global
);

hemp_module
hemp_global_module(
    hemp_global     global,
    hemp_string     name
);

void 
hemp_global_free_modules(
    hemp_global     global
);

hemp_bool
hemp_global_free_module(
    hemp_hash       modules,
    hemp_pos        position,
    hemp_slot       item
);




#endif /* HEMP_GLOBAL_H */
