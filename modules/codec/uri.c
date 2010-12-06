#include <hemp/codec.h>

HEMP_CODEC(hemp_codec_uri);
HEMP_ENCODER(hemp_codec_uri_encoder);
HEMP_DECODER(hemp_codec_uri_decoder);


hemp_bool
hemp_plugin_init(
    hemp_hemp   hemp
) {
    hemp_register_codec(hemp, "uri", &hemp_codec_uri);
    return HEMP_TRUE;
}


HEMP_CODEC(hemp_codec_uri) {
    hemp_debug_msg("instantiating uri codec\n");
    hemp_codec codec = hemp_codec_new(hemp, name);
    codec->encoder   = hemp_codec_uri_encoder;
    codec->decoder   = hemp_codec_uri_decoder;
    return codec;
}


HEMP_ENCODER(hemp_codec_uri_encoder) {
    hemp_debug_msg("hemp_codec_uri_encoder()\n");

    hemp_string string  = hemp_val_str(hemp_to_string(input, context));
    hemp_text   output  = hemp_context_tmp_text_size(context, strlen(string) + 1);
    hemp_text_append_string(output, "TODO: uri encode: ");
    hemp_text_append_string(output, string);

    // hmmm... how to do this efficiently?
    // Perhaps use a local static block of, say, 1024 chars to create a
    // locally encoded buffer and then flush it to the text auto-sizing
    // buffer via text_append_string() periodically.,

    return output;
}


HEMP_DECODER(hemp_codec_uri_decoder) {
    hemp_debug_msg("hemp_codec_uri_decoder()\n");

    hemp_string string  = input->string;
    hemp_text   output  = hemp_context_tmp_text_size(context, strlen(string) + 1);
    hemp_text_append_string(output, "TODO: uri decode: ");
    hemp_text_append_string(output, string);

    return HempMissing;
}

