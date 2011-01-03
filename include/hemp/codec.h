#ifndef HEMP_CODEC_H

#include <hemp/core.h>
#include <hemp/context.h>
#include <hemp/factory.h>
#include <hemp/module.h>


/*--------------------------------------------------------------------------
 * macros and general definitions
 *--------------------------------------------------------------------------*/

#define HEMP_ENCODE_ARGS                            \
    HempCodec      codec,                           \
    HempValue      input,                           \
    HempContext    context

#define HEMP_DECODE_ARGS                            \
    HempCodec      codec,                           \
    HempText       input,                           \
    HempContext    context

#define HEMP_ENCODER(f)                             \
    HempText f(HEMP_ENCODE_ARGS)

#define HEMP_DECODER(f)                             \
    HempValue f(HEMP_DECODE_ARGS)

#define hemp_codec_encode(codec, input, context)    \
    codec->encoder(codec, input, context)

#define hemp_codec_decode(codec, input, context)    \
    codec->decoder(codec, input, context)


/*--------------------------------------------------------------------------
 * type definitions
 *--------------------------------------------------------------------------*/

typedef HempText  (* hemp_encoder) (HEMP_ENCODE_ARGS);
typedef HempValue (* hemp_decoder) (HEMP_DECODE_ARGS);

struct hemp_codec {
    Hemp            hemp;
    HempString      name;
    hemp_encoder    encoder;
    hemp_decoder    decoder;
};


/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

HEMP_FACTORY(hemp_codec_factory);
HEMP_HASH_ITERATOR(hemp_codec_cleaner);

HempCodec
hemp_codec_new(
    Hemp            hemp,
    HempString      name
);

void
hemp_codec_free(
    HempCodec       codec
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