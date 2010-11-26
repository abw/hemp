#include <hemp/core.h>


/*--------------------------------------------------------------------------
 * Global data structure for managing hemp instances and other shared data.
 *--------------------------------------------------------------------------*/

struct hemp_global HempGlobal = {
    0,          /* nhemps - number of live hemp objects   */
    0,          /* namespaces allocated (+1 for next id)  */
    NULL        /* root namespace hash                    */
};


/*--------------------------------------------------------------------------
 * Global initialisation and cleanup functions
 *--------------------------------------------------------------------------*/

hemp_size
hemp_global_init() {
    if (! HempGlobal.n_hemps++) {
        hemp_debug_init("Initialising global hemp data\n");
        hemp_global_namespace_init(&HempGlobal);
        hemp_global_types_init(&HempGlobal);
        hemp_global_symbols_init(&HempGlobal);
    }
    
    hemp_debug_init(
        "Initialising local hemp interpreter #%d\n", 
        HempGlobal.n_hemps
    );

    return HempGlobal.n_hemps;
}


hemp_size
hemp_global_free() {
    hemp_debug_init(
        "Releasing local hemp interpreter #%d\n", 
        HempGlobal.n_hemps
    );

    if (! --HempGlobal.n_hemps) {
        hemp_debug_init("Releasing global hemp data\n");
        hemp_global_types_free(&HempGlobal);
        hemp_global_symbols_free(&HempGlobal);
        hemp_global_namespace_free(&HempGlobal);
    }

    return HempGlobal.n_hemps;
}


