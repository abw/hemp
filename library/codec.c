#include <hemp/codec.h>


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



/*--------------------------------------------------------------------------
 * Codec object functions
 *--------------------------------------------------------------------------*/

HempCodec
hemp_codec_new(
    Hemp        hemp,
    HempString  name
) {
    HempCodec codec;
    HEMP_ALLOCATE(codec);

    codec->hemp    = hemp;
    codec->name    = hemp_string_clone(name, "codec name");
    codec->encoder = &hemp_codec_no_encoder;
    codec->decoder = &hemp_codec_no_decoder;

    return codec;
}


void
hemp_codec_free(
    HempCodec codec
) {
    hemp_mem_free(codec->name);
    hemp_mem_free(codec);
}


HEMP_ENCODER(hemp_codec_no_encoder) {
    hemp_fatal("No encoder function defined for %s codec", codec->name);
    return NULL;
}


HEMP_DECODER(hemp_codec_no_decoder) {
    hemp_fatal("No decoder function defined for %s codec", codec->name);
    return HempMissing;
}


