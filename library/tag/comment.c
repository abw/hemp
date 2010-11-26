#include <hemp/tag.h>
#include <hemp/template.h>


void 
hemp_scan_comment_tag(
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

