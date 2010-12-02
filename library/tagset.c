#include <hemp/tagset.h>


hemp_tagset
hemp_tagset_new(
    hemp_template   template
) {
    hemp_tagset tagset;
    HEMP_ALLOCATE(tagset);

    tagset->template      = template;
    tagset->text_symbol   = HempSymbolText;
    tagset->tags          = hemp_hash_new();
    tagset->inline_tags   = hemp_ptree_new(HEMP_TAGSET_SIZE);
    tagset->outline_tags  = hemp_ptree_new(HEMP_TAGSET_SIZE);
    tagset->unplugged_tag = NULL;

    return tagset;
}


void
hemp_tagset_free(
    hemp_tagset tagset
) {
    hemp_hash_each(tagset->tags, &hemp_tagset_free_tag);
    hemp_hash_free(tagset->tags);
    hemp_ptree_free(tagset->inline_tags);
    hemp_ptree_free(tagset->outline_tags);

    hemp_mem_free(tagset);
}


hemp_tag
hemp_tagset_add_tag(
    hemp_tagset   tagset, 
    hemp_tag      tag
) {
    hemp_ptree ptree = NULL;

    if (hemp_hash_fetch_pointer(tagset->tags, tag->name))
        hemp_fatal("Duplicate tag in tagset: %s", tag->name);
    
    switch (tag->style) {
        case HEMP_INLINE_TAG:
            ptree = tagset->inline_tags;
            break;

        case HEMP_OUTLINE_TAG:
            ptree = tagset->outline_tags;
            break;

        case HEMP_UNPLUGGED_TAG:
            // it might be better to store an unplugged tag in the hash
            // (which we do anyway) and then look it up by name... although
            // it might require us to use a specific name, like "unplugged"
            // hmmm... it's also in the hash... maybe this should be an error?
            if (tagset->unplugged_tag)
                hemp_fatal("Tagset already has an unplugged tag");
//                hemp_tag_free(tagset->unplugged_tag);

            tagset->unplugged_tag = tag;
            break;
        
        default:
            hemp_fatal("Invalid tag style for %s tag: %d", tag->name, tag->style);
    }

    tag->tagset = tagset;

    hemp_hash_store_pointer(tagset->tags, tag->name, tag);

    if (ptree)
        hemp_ptree_store(ptree, tag->start, (hemp_memory) tag);
    
    return tag;
}


hemp_tag
hemp_tagset_new_tag(
    hemp_tagset     tagset, 
    hemp_string     type,
    hemp_string     name,
    hemp_string     start,
    hemp_string     end,
    hemp_grammar    grammar
) {
//    hemp_debug("tagset: %p  template: %p\n", tagset, tagset->template);
//    hemp_dialect d = tagset->template->dialect;
//    hemp_debug("dialect: %p  hemp: %p\n", d, d->hemp);
//    hemp_debug("type: %s  name: %s   start: %s   end: %s  grammar: %p\n", type, name, start, end ? end : "", grammar);
//
//
//    hemp_action _cons = hemp_constructor(d->hemp, tag, type);
//    hemp_debug("calling constructor action: %p => [%p, %p]\n", _cons, _cons->actor, _cons->script);
//    hemp_action_run(_cons, type, name, start, grammar);
//    hemp_debug("called constructor\n");
//
//    hemp_tag tag = hemp_tag_construct(
//        tagset->template->dialect->hemp,        // a rather tenuous link
//        type, name, start, end, grammar
//    );
//    hemp_debug("created tag: %p\n", tag);
//    
    return hemp_tagset_add_tag(
        tagset,
        hemp_tag_construct(
            tagset->template->dialect->hemp,        // a rather tenuous link
            type, name, start, end, grammar
        )
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
    hemp_tagset tagset
) {
    hemp_ptree_dump(tagset->inline_tags);
    hemp_ptree_dump(tagset->outline_tags);
}


