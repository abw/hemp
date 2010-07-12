#include "hemp/tags.h"


static struct hemp_tag 
    hemp_embed_tag = { 
        "hemp.tag.embed", 
        "${", "}", 
        &hemp_scan_embed_tag
    };


hemp_tag_t HempTagEmbed = &hemp_embed_tag;


void 
hemp_scan_embed_tag(
    hemp_template_t tmpl,
    hemp_tag_t      tag,
    hemp_cstr_t     tagtok,
    hemp_pos_t      pos,
    hemp_cstr_t     *srcptr
) {
    hemp_todo("embed tag");
}

