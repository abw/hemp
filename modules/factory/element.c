#include <hemp.h>
#include <hemp/debug.h>


HEMP_FACTORY(hemp_element_factory);
HEMP_HASH_ITERATOR(hemp_element_cleaner);


/*--------------------------------------------------------------------------
 * Binder function registers the element factory with the hemp instance
 *--------------------------------------------------------------------------*/

HempBool
hemp_module_binder(
    HempModule  module,
    Hemp        hemp
) {
    hemp_register_factory( hemp, HEMP_FACTORY_ELEMENT, &hemp_element_factory );
    hemp_debug_init("loaded element factory module\n");
    return HEMP_TRUE;
}


/*--------------------------------------------------------------------------
 * Factory functions for loading elements
 *--------------------------------------------------------------------------*/

HEMP_FACTORY(hemp_element_factory) {
    hemp_debug_init("instantiating element factory\n");
    HempFactory factory = hemp_factory_new(hemp, name);
    factory->cleaner     = &hemp_element_cleaner;
    factory->autoload    = NULL;        // TODO: load element
    return factory;
}


HEMP_HASH_ITERATOR(hemp_element_cleaner) {
    HempElement element = (HempElement) hemp_val_ptr(item->value);
    hemp_debug_init("cleaning element: %s\n", element->name);
    hemp_element_free(element);
    return HEMP_TRUE;
}


