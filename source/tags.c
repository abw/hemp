#include "hemp/tags.h"
#include "hemp/elements.h"
#include "hemp/debug.h"

void hemp_scan_tag();                  // TODO: args


hemp_tagset_t
hemp_tagset_init() {
    hemp_tagset_t tagset;

    if ((tagset = (hemp_tagset_t) hemp_mem_init(sizeof(struct hemp_tagset)))) {
        tagset->text_element = HempElementText;
        tagset->inline_tags  = hemp_ptree_init(HEMP_DEFAULT_TAGSET_SIZE);
        tagset->outline_tags = hemp_ptree_init(HEMP_DEFAULT_TAGSET_SIZE);
        
        if (tagset->inline_tags && tagset->outline_tags) {
            debug_cyan(
                "Allocated new tagset at %p for %d inline/outline tags\n", 
                tagset, HEMP_DEFAULT_TAGSET_SIZE
            );
        }
        else {
            hemp_tagset_free(tagset);
            tagset = NULL;
        }
    }
    
    // TODO: handle tagset == NULL

    return tagset;
}

void
hemp_tagset_free(
    hemp_tagset_t tagset
) {
    if (tagset->inline_tags) {
        debug_cyan("Releasing tagset inline tags at %p\n", tagset->inline_tags);
        hemp_ptree_free(tagset->inline_tags);
    }
    if (tagset->outline_tags) {
        debug_cyan("Releasing tagset outline tags at %p\n", tagset->outline_tags);
        hemp_ptree_free(tagset->outline_tags);
    }
    hemp_mem_free(tagset);
}


hemp_pnode_t
hemp_tagset_add_inline_tag(
    hemp_tagset_t   tagset, 
    hemp_tag_t      tag
) {
    return hemp_ptree_insert(
        tagset->inline_tags, 
        (hemp_text_t) tag->start, 
        (hemp_ptr_t)  tag
    );
}


hemp_pnode_t
hemp_tagset_add_outline_tag(
    hemp_tagset_t   tagset, 
    hemp_tag_t      tag
) {
    return hemp_ptree_insert(
        tagset->outline_tags, 
        (hemp_text_t) tag->start, 
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



/*=======================================================================*/


static struct hemp_tag 
    hemp_inline_tag = { 
        "tt.tag.inline", 
        "[%", "%]", 
        &hemp_scan_tag 
    };

static struct hemp_tag 
    hemp_outline_tag = { 
        "tt.tag.outline", 
        "%%", "\n", 
        &hemp_scan_tag
    };

static struct hemp_tag 
    hemp_comment_tag = { 
        "tt.tag.comment",
        "[#", "#]",
        &hemp_scan_tag
    };

static struct hemp_tag 
    hemp_control_tag = { 
        "tt.tag.control",
        "[?", "?]",
        &hemp_scan_tag
    };

static struct hemp_tag 
    hemp_variable_tag = { 
        "tt.tag.variable",
        "$", NULL,
        &hemp_scan_tag
    };

static struct hemp_tag 
    hemp_embed_tag = { 
        "tt.tag.embed",
        "${", "}",
        &hemp_scan_tag
    };


hemp_tag_t  HempTagInline   = &hemp_inline_tag;
hemp_tag_t  HempTagOutline  = &hemp_outline_tag;
hemp_tag_t  HempTagComment  = &hemp_comment_tag;
hemp_tag_t  HempTagControl  = &hemp_control_tag;
hemp_tag_t  HempTagVariable = &hemp_variable_tag;
hemp_tag_t  HempTagEmbed    = &hemp_embed_tag;


void hemp_scan_tag() {
    debug_red("TODO: hemp_scan_tag()\n");
}



