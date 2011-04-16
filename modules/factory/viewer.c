#include <hemp.h>
#include <hemp/debug.h>


HEMP_FACTORY(hemp_viewer_factory);
HEMP_HASH_ITERATOR(hemp_viewer_cleaner);


/*--------------------------------------------------------------------------
 * Binder function registers the viewer factory with the hemp instance
 *--------------------------------------------------------------------------*/

HempBool
hemp_module_binder(
    HempModule  module,
    Hemp        hemp
) {
    hemp_register_factory( hemp, HEMP_FACTORY_VIEWER, &hemp_viewer_factory );
    hemp_debug_init("loaded viewer factory module\n");
    return HEMP_TRUE;
}



/*--------------------------------------------------------------------------
 * Factory functions for loading viewers
 *--------------------------------------------------------------------------*/
HEMP_FACTORY(hemp_viewer_factory) {
    hemp_debug_init("instantiating viewer factory\n");
    HempFactory factory = hemp_factory_new(hemp, name);
    factory->cleaner    = hemp_viewer_cleaner;
    return factory;
}


HEMP_HASH_ITERATOR(hemp_viewer_cleaner) {
    HempViewer viewer = (HempViewer) hemp_val_ptr(item->value);
    hemp_debug_init("cleaning viewer: %s\n", viewer->name);
    hemp_viewer_free(viewer);
    return HEMP_TRUE;
}


