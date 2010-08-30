#include "hemp/tags.h"


static struct hemp_tag 
    hemp_embed_tag = {
        HEMP_INLINE_TAG,
        "hemp.tag.embed", 
        "${", "}", 
        &hemp_scan_embed_tag
    };


hemp_tag_t HempTagEmbed = &hemp_embed_tag;


void 
hemp_scan_embed_tag(
    hemp_template_p tmpl,
    hemp_tag_p      tag,
    hemp_str_p      tagtok,
    hemp_pos_t      pos,
    hemp_str_p     *srcptr
) {
    hemp_todo("embed tag");
}

