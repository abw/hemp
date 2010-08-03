#include <hemp.h>
#include "tap.h"

void test_tagset();


int
main(int argc, char **argv, char **env)
{
    plan_tests(7);
    test_tagset();
    hemp_mem_trace_ok();
    return exit_status();
}


void test1_scan(
    hemp_tag_p  tag,
    hemp_cstr_p start, 
    hemp_pos_t  pos, 
    hemp_cstr_p *src
) {
    debug("custom scanner");
}


void test_tagset() {
    hemp_tag_p    inline_tag, outline_tag;
    hemp_tagset_p tagset;

    inline_tag = hemp_tag_init(
        "inline", HEMP_INLINE, &test1_scan, "[%", "%]"
    );
    ok( inline_tag, "created inline tag object" );

    outline_tag = hemp_tag_init(
        "outline", HEMP_OUTLINE, &test1_scan, "%%", NULL
    );
    ok( outline_tag, "created outline tag object" );

    tagset = hemp_tagset_init();
    ok( tagset, "created tagset object" );

    ok( hemp_tagset_add_tag(tagset, inline_tag),  "added inline tag" );
    ok( hemp_tagset_add_tag(tagset, outline_tag), "added outline tag" );

    ok( 
        hemp_tagset_new_tag(
            tagset, 
            "wibble", HEMP_INLINE, &test1_scan, "<*", "*>"
        ), 
        "added wibble tag" 
    );

    hemp_tagset_free(tagset);
}


