#include <hemp/tag.h>
#include <hemp/template.h>


HEMP_TAG(hemp_tag_comment) {
//  hemp_debug_msg(
//      "hemp_tag_comment(%p, %s, %s, %s, %s, %s)\n",
//      hemp, type, name, start, end ? end : "", 
//      grammar ? grammar->name : "no grammar"
//  );
    hemp_tag tag = hemp_tag_inline(
        hemp, type, name, start, end, grammar
    );
    tag->scan = &hemp_tag_comment_scan;
    return tag;
}


void 
hemp_tag_comment_scan(
    HEMP_TAG_SCAN_ARGS
) {
    hemp_string to = strstr(*srcptr, tag->end);
    
    if (! to)
        hemp_fatal("missing end of comment tag");

    to += strlen(tag->end);

    hemp_debug_token("comment", tagtok, to - tagtok);

    hemp_elements_append(
        tmpl->elements, HempSymbolComment,
        tagtok, pos, to - tagtok
    );

    *srcptr = to;
}

