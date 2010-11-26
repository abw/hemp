#include <hemp/tagset.h>


hemp_tagset_p
hemp_tagset_init() {
    hemp_tagset_p tagset = (hemp_tagset_p) hemp_mem_alloc(
        sizeof(struct hemp_tagset_s)
    );

    if (! tagset)
        hemp_mem_fail("tagset");

    tagset->text_symbol  = HempSymbolText;
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
    hemp_tag      tag
) {
    hemp_ptree_p ptree;

    if (hemp_hash_fetch_pointer(tagset->tags, tag->name))
        hemp_fatal("Duplicate tag in tagset: %s", tag->name);
    
    switch (tag->style) {
        case HEMP_INLINE_TAG:
            ptree = tagset->inline_tags;
            break;

        case HEMP_OUTLINE_TAG:
            ptree = tagset->outline_tags;
            break;
        
        default:
            hemp_fatal("Invalid tag style for %s tag: %d", tag->name, tag->style);
    }

    hemp_hash_store_pointer(tagset->tags, tag->name, tag);

    return hemp_ptree_store(ptree, tag->start, (hemp_memory) tag);
}


hemp_pnode_p
hemp_tagset_new_tag(
    hemp_tagset_p    tagset, 
    hemp_string       name,
    hemp_tag_style_t style,
    hemp_string       start,
    hemp_string       end,
    hemp_tag_scan_f  scan,
    hemp_grammar   grammar
) {
    return hemp_tagset_add_tag(
        tagset,
        hemp_tag_init(name, style, start, end, scan, grammar)
    );
}


hemp_bool
hemp_tagset_free_tag(
    hemp_hash tags,
    hemp_pos  index,
    hemp_slot item
) {
    hemp_tag_free((hemp_tag) hemp_val_ptr(item->value));
    return HEMP_TRUE;
}



void
hemp_tagset_dump(
    hemp_tagset_p tagset
) {
    hemp_ptree_dump(tagset->inline_tags);
    hemp_ptree_dump(tagset->outline_tags);
}


