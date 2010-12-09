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
    hemp_document document,
    hemp_tag      tag,
    hemp_string      tagtok,
    hemp_pos      pos,
    hemp_string     *srcptr
) {
    hemp_todo("embed tag");
}

