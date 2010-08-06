#include <hemp/tagset.h>


hemp_tagset_p
hemp_tagset_init() {
    hemp_tagset_p tagset = (hemp_tagset_p) hemp_mem_alloc(
        sizeof(struct hemp_tagset_s)
    );

    if (! tagset)
        hemp_mem_fail("tagset");

    tagset->text_element = HempElementText;
    tagset->tags         = hemp_hash_init();
    tagset->inline_tags  = hemp_ptree_init(HEMP_TAGSET_SIZE);
    tagset->outline_tags = hemp_ptree_init(HEMP_TAGSET_SIZE);

    return tagset;
}


void
hemp_tagset_free(
    hemp_tagset_p tagset
) {
    hemp_hash_each(tagset->tags, &hemp_tagset_free_tag);
    hemp_hash_free(tagset->tags);
    hemp_ptree_free(tagset->inline_tags);
    hemp_ptree_free(tagset->outline_tags);
    hemp_mem_free(tagset);
}


hemp_pnode_p
hemp_tagset_add_tag(
    hemp_tagset_p   tagset, 
    hemp_tag_p      tag
) {
    hemp_ptree_p ptree;

    if (hemp_hash_fetch(tagset->tags, tag->name))
        hemp_fatal("Duplicate tag in tagset: %s", tag->name);
    
    switch (tag->style) {
        case HEMP_INLINE:
            ptree = tagset->inline_tags;
            break;

        case HEMP_OUTLINE:
            ptree = tagset->outline_tags;
            break;
        
        default:
            hemp_fatal("Invalid tag style for %s tag: %d", tag->name, tag->style);
    }

    hemp_hash_store(tagset->tags, tag->name, tag);

    return hemp_ptree_store(ptree, tag->start, (hemp_mem_p) tag);
}


hemp_pnode_p
hemp_tagset_new_tag(
    hemp_tagset_p    tagset, 
    hemp_cstr_p      name,
    hemp_tag_style_t style,
    hemp_cstr_p      start,
    hemp_cstr_p      end,
    hemp_tag_scan_f  scan,
    hemp_grammar_p   grammar
) {
    return hemp_tagset_add_tag(
        tagset,
        hemp_tag_init(name, style, start, end, scan, grammar)
    );
}


hemp_bool_t
hemp_tagset_free_tag(
    hemp_hash_p      tags,
    hemp_pos_t       index,
    hemp_hash_item_p item
) {
    hemp_tag_free((hemp_tag_p) item->value);
    return HEMP_TRUE;
}



void
hemp_tagset_dump(
    hemp_tagset_p tagset
) {
    hemp_ptree_dump(tagset->inline_tags);
    hemp_ptree_dump(tagset->outline_tags);
}


