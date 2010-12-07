#include <hemp/codec.h>


hemp_codec
hemp_codec_new(
    hemp_hemp       hemp,
    hemp_string     name
) {
    hemp_codec codec;
    HEMP_ALLOCATE(codec);

    codec->hemp    = hemp;
    codec->name    = hemp_string_clone(name, "codec name");
    codec->encoder = &hemp_codec_no_encoder;
    codec->decoder = &hemp_codec_no_decoder;

    return codec;
}


void
hemp_codec_free(
    hemp_codec codec
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


HEMP_AUTOLOAD(hemp_codec_autoload) {
    hemp_debug_call("hemp_codec_autoload(%s)\n", name);
    return hemp_use_module(factory->hemp, "codec", name)
        ? HEMP_TRUE
        : HEMP_FALSE;
}

