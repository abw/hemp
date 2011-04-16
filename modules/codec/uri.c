#include <hemp.h>

HEMP_CODEC(hemp_codec_uri);
HEMP_ENCODER(hemp_codec_uri_encoder);
HEMP_DECODER(hemp_codec_uri_decoder);


/*--------------------------------------------------------------------------
 * Binder function called to bind the used module into a hemp instance
 *--------------------------------------------------------------------------*/

HempBool
hemp_module_binder(
    HempModule module,
    Hemp   hemp
) {
    hemp_register_codec(hemp, "uri", &hemp_codec_uri);
    return HEMP_TRUE;
}


/*--------------------------------------------------------------------------
 * Codec functions
 *--------------------------------------------------------------------------*/

HEMP_CODEC(hemp_codec_uri) {
    hemp_debug_msg("instantiating uri codec\n");
    HempCodec codec = hemp_codec_new(hemp, name);
    codec->encoder  = hemp_codec_uri_encoder;
    codec->decoder  = hemp_codec_uri_decoder;
    return codec;
}


HEMP_ENCODER(hemp_codec_uri_encoder) {
    hemp_debug_call("hemp_codec_uri_encoder()\n");

    HempString string   = hemp_to_string(input, context);
    HempText   output   = hemp_context_tmp_text_size(context, strlen(string) + 1);
    hemp_text_append_string(output, "TODO: uri encode: ");
    hemp_text_append_string(output, string);

    // hmmm... how to do this efficiently?
    // Perhaps use a local static block of, say, 1024 chars to create a
    // locally encoded buffer and then flush it to the text auto-sizing
    // buffer via text_append_string() periodically.,

    return output;
}


HEMP_DECODER(hemp_codec_uri_decoder) {
    hemp_debug_call("hemp_codec_uri_decoder()\n");

    HempString string  = input->string;
    HempText   output  = hemp_context_tmp_text_size(context, strlen(string) + 1);
    hemp_text_append_string(output, "TODO: uri decode: ");
    hemp_text_append_string(output, string);

    return HempMissing;
}

