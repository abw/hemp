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


/*--------------------------------------------------------------------------
 * quick hack for test purposes - should be moved into separate dynamically
 * loaded module.... but this is a chicken-and-egg problem - we need the URI
 * codec to decode URIs so we can implement filesystem paths, so we can locate
 * and load codec modules....
 *--------------------------------------------------------------------------*/

HEMP_CODEC(hemp_codec_uri) {
    hemp_debug_msg("instantiating uri codec\n");
    hemp_codec codec = hemp_codec_new(hemp, name);
    codec->encoder   = hemp_codec_uri_encoder;
    codec->decoder   = hemp_codec_uri_decoder;
    return codec;
}


HEMP_ENCODER(hemp_codec_uri_encoder) {
    hemp_debug_msg("hemp_codec_uri_encoder()\n");

    hemp_text output = hemp_context_tmp_text(context);
    hemp_text_append_string(output, "TODO: uri encode: ");
    
    if (hemp_is_string(input)) {
        hemp_debug_msg("%s encoder input is a string: %s\n", codec->name, hemp_val_str(input));
        hemp_text_append_string(output, hemp_val_str(input));
    }
    else if (hemp_is_text(input)) {
        hemp_debug_msg("%s encoder input is text: %s\n", codec->name, hemp_val_text(input)->string);
        hemp_text_append_string(output, hemp_val_text(input)->string);
    }
    else {
        hemp_fatal("%s cannot encode %s data\n", codec->name, hemp_type_name(input));
    }

    return output;
}


HEMP_DECODER(hemp_codec_uri_decoder) {
    hemp_debug_msg("hemp_codec_uri_decoder()\n");
    return HempMissing;
}

