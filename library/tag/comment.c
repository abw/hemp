#include <hemp/tag.h>


void 
hemp_scan_comment_tag(
//    hemp_template_t tmpl,
    hemp_tag_p      tag,
    hemp_cstr_p     tagtok,
    hemp_pos_t      pos,
    hemp_cstr_p     *srcptr
) {
    hemp_cstr_p     to = strstr(*srcptr, tag->end);
    
    if (! to)
        hemp_fatal("missing end of comment tag");

    to += strlen(tag->end);

    debug_token("comment", tagtok, to - tagtok);
    hemp_todo("comment tag");

//    hemp_elements_append(
//        tmpl->elements, HempElementComment,
//        tagtok, pos, to - tagtok
//    );
//    
    *srcptr = to;
}

