#include "hemp/tags.h"


static struct hemp_tag 
    hemp_outline_tag = { 
        "hemp.tag.outline", 
        "%%", "\n", 
        &hemp_scan_outline_tag
    };


hemp_tag_t HempTagOutline = &hemp_outline_tag;


void 
hemp_scan_outline_tag(
    hemp_template_t tmpl,
    hemp_tag_t      tag,
    hemp_cstr_t     tagtok,
    hemp_pos_t      pos,
    hemp_cstr_t     *srcptr
) {
    hemp_todo("outline tag");
}

