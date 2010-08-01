#include "hemp/tags.h"


static struct hemp_tag 
    hemp_tag_comment = { 
        HEMP_INLINE_TAG,
        "hemp.tag.comment",
        "[#", "#]",
        &hemp_scan_comment_tag
    };


hemp_tag_t HempTagComment = &hemp_tag_comment;


void 
hemp_scan_comment_tag(
    hemp_template_t tmpl,
    hemp_tag_t      tag,
    hemp_cstr_t     tagtok,
    hemp_pos_t      pos,
    hemp_cstr_t     *srcptr
) {
    hemp_cstr_t     to;
    
    to = strstr(*srcptr, tag->end);
    
    if (! to) {
        fprintf(stderr, "missing end of comment tag\n");
        return;
    }

    to += strlen(tag->end);

    debug_token("comment", tagtok, to - tagtok);

    hemp_elements_append(
        tmpl->elements, HempElementComment,
        tagtok, pos, to - tagtok
    );
    
    *srcptr = to;
}

