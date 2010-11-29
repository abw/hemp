#include <hemp/test.h>

void test_tagset();


int main(
    int argc, char **argv, char **env
) {
    plan(6);
    test_tagset();
    return done();
}


void test1_scan(
    HEMP_TAG_SCAN_ARGS
) {
    hemp_debug("custom scanner");
}


void test_tagset() {
    hemp_hemp       hemp        = hemp_new();
    hemp_dialect    dialect     = hemp_dialect_new(hemp, "test.dialect");
    hemp_source     source      = hemp_source_instance(hemp, "text", "Hello world");
    hemp_template   template    = hemp_template_new(dialect, source);
    hemp_tag    inline_tag, outline_tag;
    hemp_tagset tagset;

    HEMP_TRY;
        inline_tag = hemp_tag_new(
            "hemp.inline", "inline", 
            "[%", "%]", 
            NULL
        );
        ok( inline_tag, "created inline tag object" );
        
        outline_tag = hemp_tag_new(
            "hemp.outline", "outline",
            "%%", NULL, NULL
        );
        ok( outline_tag, "created outline tag object" );

        tagset = hemp_tagset_new(template);
        ok( tagset, "created tagset object" );

        ok( hemp_tagset_add_tag(tagset, inline_tag),  "added inline tag" );
        ok( hemp_tagset_add_tag(tagset, outline_tag), "added outline tag" );

        ok( 
            hemp_tagset_new_tag(
                tagset, "hemp.inline", "wobble",
                "<*", "*>", NULL
            ), 
            "added wibble tag" 
        );
    HEMP_CATCH_ALL;
        fail("unexpected error: %s", hemp->error->message);
    HEMP_END;

    hemp_template_free(template);
    hemp_tagset_free(tagset);
    hemp_dialect_free(dialect);
    hemp_free(hemp);
//    hemp_tagset_free(tagset);
}


