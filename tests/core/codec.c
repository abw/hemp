#include <hemp/test.h>


void test_codec();

int main(
    int argc, char **argv, char **env
) {
    plan(5);
    test_codec();
    return done();
}


void test_codec() {
    Hemp hemp = hemp_new();
    ok( hemp, "created hemp object" );

    HEMP_TRY;

        HempCodec codec = hemp_codec_new(hemp, "hemp.codec.test");
        ok( codec, "created test codec" );
        is( codec->name, "hemp.codec.test", "codec name" );
        hemp_codec_free(codec);

        codec = hemp_codec_instance(hemp, "uri");
        ok( codec, "fetched uri codec" );
        is( codec->name, "uri", "codec name" );

        HempText text = hemp_encode(
            hemp, "uri", hemp_str_val("blah blah"), NULL
        );
        hemp_debug_msg("output: %s\n", text->string);

        HempText t2 = hemp_encode(
            hemp, "uri", hemp_text_val(text), NULL
        );
        hemp_debug_msg("output: %s\n", t2->string);

    HEMP_CATCH_ALL;
        printf("caught error: %s\n", hemp->error->message);

    HEMP_END;

    printf("*** tests done, freeing hemp\n");
/*
    HempText t3;

        t3 = hemp_encode(
            hemp, "wibble", hemp_text_val(text), NULL
        );
        hemp_debug_msg("output: %s\n", t3->string);
    HEMP_CATCH_ALL;
        printf("caught error: %s\n", hemp->error->message);
    HEMP_END;
*/


    hemp_free(hemp);
}
