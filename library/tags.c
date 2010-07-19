#include <ctype.h>
#include <string.h>
#include "hemp/tags.h"
#include "hemp/elements.h"
#include "hemp/template.h"
#include "hemp/debug.h"

void hemp_scan_tag();                  // TODO: args
void 
debug_token(
    hemp_cstr_t type,
    hemp_cstr_t str,
    hemp_pos_t  len
);


hemp_tagset_t
hemp_tagset_init() {
    hemp_tagset_t tagset;

    if ((tagset = (hemp_tagset_t) hemp_mem_init(sizeof(struct hemp_tagset)))) {
        tagset->text_element = HempElementText;
        tagset->inline_tags  = hemp_ptree_init(HEMP_DEFAULT_TAGSET_SIZE);
        tagset->outline_tags = hemp_ptree_init(HEMP_DEFAULT_TAGSET_SIZE);
        
        if (tagset->inline_tags && tagset->outline_tags) {
            debug_mem(
                "Allocated new tagset at %p for %d inline/outline tags\n", 
                tagset, HEMP_DEFAULT_TAGSET_SIZE
            );
        }
        else {
            hemp_tagset_null(tagset);
        }
    }
    
    // TODO: handle tagset == NULL

    return tagset;
}


void
hemp_tagset_free(
    hemp_tagset_t tagset
) {
    debug_mem("Releasing tagset at %p\n", tagset);

    if (tagset->inline_tags) {
        debug_mem("Releasing tagset inline tags at %p\n", tagset->inline_tags);
        hemp_ptree_free(tagset->inline_tags);
    }
    if (tagset->outline_tags) {
        debug_mem("Releasing tagset outline tags at %p\n", tagset->outline_tags);
        hemp_ptree_free(tagset->outline_tags);
    }
    
    hemp_mem_free(tagset);
}


hemp_pnode_t
hemp_tagset_add_inline_tag(
    hemp_tagset_t   tagset, 
    hemp_tag_t      tag
) {
    return hemp_ptree_store(
        tagset->inline_tags, 
        (hemp_cstr_t) tag->start, 
        (hemp_ptr_t)  tag
    );
}


hemp_pnode_t
hemp_tagset_add_outline_tag(
    hemp_tagset_t   tagset, 
    hemp_tag_t      tag
) {
    return hemp_ptree_store(
        tagset->outline_tags, 
        (hemp_cstr_t) tag->start, 
        (hemp_ptr_t)  tag
    );
}


void
hemp_tagset_dump(
    hemp_tagset_t   tagset
) {
    hemp_ptree_dump(tagset->inline_tags);
    hemp_ptree_dump(tagset->outline_tags);
}


