#include "hemp/tags.h"


static struct hemp_tag 
    hemp_variable_tag = { 
        HEMP_INLINE_TAG,
        "hemp.tag.variable", 
        "$", NULL, 
        &hemp_scan_variable_tag
    };


hemp_tag_t HempTagVariable = &hemp_variable_tag;


void 
hemp_scan_variable_tag(
    hemp_template_t tmpl,
    hemp_tag_t      tag,
    hemp_cstr_t     tagtok,
    hemp_pos_t      pos,
    hemp_cstr_t     *srcptr
) {
    hemp_todo("variable tag");
}

