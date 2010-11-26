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
    hemp_template tmpl,
    hemp_tag      tag,
    hemp_string      tagtok,
    hemp_pos      pos,
    hemp_string     *srcptr
) {
    hemp_todo("variable tag");
}

