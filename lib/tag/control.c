#include "hemp/tags.h"


static struct hemp_tag 
    hemp_control_tag = { 
        "hemp.tag.control",
        "[?", "?]",
        &hemp_scan_control_tag
    };


hemp_tag_t HempTagControl = &hemp_control_tag;


void 
hemp_scan_control_tag(
    hemp_template_t tmpl,
    hemp_tag_t      tag,
    hemp_cstr_t     tagtok,
    hemp_pos_t      pos,
    hemp_cstr_t     *srcptr
) {
    hemp_todo("control tag");
}

