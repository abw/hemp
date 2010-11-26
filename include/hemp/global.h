#ifndef HEMP_GLOBAL_H
#define HEMP_GLOBAL_H

struct hemp_global {
    hemp_size       n_hemps;
    hemp_u16        namespace_id;
    hemp_hash     namespaces;
};

extern struct hemp_global HempGlobal;


#endif /* HEMP_GLOBAL_H */
