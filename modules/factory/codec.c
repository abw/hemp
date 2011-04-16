#include <hemp.h>
#include <hemp/debug.h>


HEMP_FACTORY(hemp_codec_factory);
HEMP_HASH_ITERATOR(hemp_codec_cleaner);


/*--------------------------------------------------------------------------
 * Binder function registers the codec factory with the hemp instance
 *--------------------------------------------------------------------------*/

HempBool
hemp_module_binder(
    HempModule  module,
    Hemp        hemp
) {
    hemp_register_factory( hemp, HEMP_FACTORY_CODEC, &hemp_codec_factory );
    hemp_debug_init("loaded codec factory module\n");
    return HEMP_TRUE;
}



/*--------------------------------------------------------------------------
 * Factory functions for loading codecs
 *--------------------------------------------------------------------------*/

HEMP_FACTORY(hemp_codec_factory) {
    hemp_debug_init("instantiating codec factory\n");
    HempFactory factory = hemp_factory_new(hemp, name);
    factory->cleaner    = hemp_codec_cleaner;
    return factory;
}


HEMP_HASH_ITERATOR(hemp_codec_cleaner) {
    HempCodec codec = (HempCodec) hemp_val_ptr(item->value);
    hemp_debug_init("cleaning codec: %s\n", codec->name);
    hemp_codec_free(codec);
    return HEMP_TRUE;
}

