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
    hemp_template_p tmpl,
    hemp_tag_p      tag,
    hemp_str_p      tagtok,
    hemp_pos_t      pos,
    hemp_str_p     *srcptr
) {
    hemp_todo("variable tag");
}

