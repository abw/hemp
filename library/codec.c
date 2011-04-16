#include <hemp/codec.h>

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


