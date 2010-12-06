#ifndef HEMP_CODEC_H

#include <hemp/core.h>
#include <hemp/context.h>


/*--------------------------------------------------------------------------
 * macros and general definitions
 *--------------------------------------------------------------------------*/

#define HEMP_CODEC(f)                               \
    hemp_codec f(                                   \
        hemp_hemp   hemp,                           \
        hemp_string name                            \
    )

#define HEMP_ENCODE_ARGS                            \
    hemp_codec      codec,                          \
    hemp_value      input,                          \
    hemp_context    context

#define HEMP_DECODE_ARGS                            \
    hemp_codec      codec,                          \
    hemp_text       input,                          \
    hemp_context    context

#define HEMP_ENCODER(f)                             \
    hemp_text f(HEMP_ENCODE_ARGS)

#define HEMP_DECODER(f)                             \
    hemp_value f(HEMP_DECODE_ARGS)

#define hemp_codec_encode(codec, input, context)    \
    codec->encoder(codec, input, context)

#define hemp_codec_decode(codec, input, context)    \
    codec->decoder(codec, input, context)


/*--------------------------------------------------------------------------
 * type definitions
 *--------------------------------------------------------------------------*/

typedef hemp_text  (* hemp_encoder) (HEMP_ENCODE_ARGS);
typedef hemp_value (* hemp_decoder) (HEMP_DECODE_ARGS);

struct hemp_codec {
    hemp_hemp       hemp;
    hemp_string     name;
    hemp_encoder    encoder;
    hemp_decoder    decoder;
};


/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

hemp_codec
hemp_codec_new(
    hemp_hemp       hemp,
    hemp_string     name
);

void
hemp_codec_free(
    hemp_codec      codec
);

HEMP_ENCODER(hemp_codec_no_encoder);
HEMP_DECODER(hemp_codec_no_decoder);



/*--------------------------------------------------------------------------
 * tmp - move to separate dynamically loaded module
 *--------------------------------------------------------------------------*/

HEMP_CODEC(hemp_codec_uri);
HEMP_ENCODER(hemp_codec_uri_encoder);
HEMP_DECODER(hemp_codec_uri_decoder);


#endif /* HEMP_CODEC_H */