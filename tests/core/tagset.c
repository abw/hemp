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
    hemp_tag_p    inline_tag, outline_tag;
    hemp_tagset_p tagset;

    inline_tag = hemp_tag_init(
        "inline", 
        HEMP_INLINE_TAG, 
        "[%", "%]", 
        &test1_scan, NULL
    );
    ok( inline_tag, "created inline tag object" );

    outline_tag = hemp_tag_init(
        "outline", 
        HEMP_OUTLINE_TAG, 
        "%%", NULL,
        &test1_scan, NULL
    );
    ok( outline_tag, "created outline tag object" );

    tagset = hemp_tagset_init();
    ok( tagset, "created tagset object" );

    ok( hemp_tagset_add_tag(tagset, inline_tag),  "added inline tag" );
    ok( hemp_tagset_add_tag(tagset, outline_tag), "added outline tag" );

    ok( 
        hemp_tagset_new_tag(
            tagset, "wibble", 
            HEMP_INLINE_TAG, 
            "<*", "*>",
            &test1_scan, NULL
        ), 
        "added wibble tag" 
    );

    hemp_tagset_free(tagset);
}


