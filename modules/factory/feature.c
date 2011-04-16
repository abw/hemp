#include <hemp.h>
#include <hemp/debug.h>


HEMP_FACTORY(hemp_feature_factory);


/*--------------------------------------------------------------------------
 * Binder function registers the feature factory with the hemp instance
 *--------------------------------------------------------------------------*/

HempBool
hemp_module_binder(
    HempModule  module,
    Hemp        hemp
) {
    hemp_register_factory( hemp, HEMP_FACTORY_FEATURE, &hemp_feature_factory );
    hemp_debug_init("loaded feature factory module\n");
    return HEMP_TRUE;
}


/*--------------------------------------------------------------------------
 *  Constructor for feature factory which instantiates language features
 *--------------------------------------------------------------------------*/

HEMP_FACTORY(hemp_feature_factory) {
    hemp_debug_init("instantiating feature factory\n");
    HempFactory factory = hemp_factory_new(hemp, name);
    return factory;
}

