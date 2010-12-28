#include <hemp/tagset.h>


hemp_tagset
hemp_tagset_new(
    hemp_document   document
) {
    hemp_tagset tagset;
    HEMP_ALLOCATE(tagset);

    tagset->document      = document;
    tagset->text_element  = HempElementText;
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
//    hemp_debug("tagset: %p  document: %p\n", tagset, tagset->document);
//    hemp_dialect d = tagset->document->dialect;
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
//        tagset->document->dialect->hemp,        // a rather tenuous link
//        type, name, start, end, grammar
//    );
//    hemp_debug("created tag: %p\n", tag);
//    
    return hemp_tagset_add_tag(
        tagset,
        hemp_tag_construct(
            tagset->document->dialect->hemp,        // a rather tenuous link
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


/*
hemp_tagset
hemp_tagset_prepare(
    hemp_document   document
) {
    hemp_debug_call("hemp_tagset_prepare(%p)\n", document);
    hemp_tagset tagset = hemp_tagset_new(document);
    document->scanner  = hemp_action_new(
        (hemp_actor) &hemp_tagset_scanner, 
        (hemp_memory) tagset
    );
//    hemp_debug_msg(
//        "scanner: %p = [%p, %p]\n", 
//        document->scanner, 
//        document->scanner->actor,
//        document->scanner->script
//    );

    return tagset;
}

*/

//void
//hemp_tagset_cleanup(
//    hemp_document document
//) {
//    if (document->scanner) {
//        hemp_debug_init("tagset scanner cleanup\n");
//        hemp_tagset_free((hemp_tagset) document->scanner->script);
//        hemp_action_free(document->scanner);
//    }
//}


HEMP_SCANNER(hemp_tagset_scanner) {
    hemp_debug_call("hemp_tagset_scanner()\n");

    hemp_tagset     tagset   = (hemp_tagset) self;
    hemp_string     src      = document->scanptr,
                    from     = document->scantok,
                    tagstr;
    hemp_pnode      pnode;
    hemp_tag        tag;

//    hemp_debug_msg("POS [%d] TEXT [%s]\n", document->scanpos, document->scantok);

#if HEMP_DEBUG & HEMP_DEBUG_SCAN
    hemp_debug_magenta("-- source ---\n%s\n-------------\n", src);
#endif

    while (*src) {
        /* look for outline tags at the start of line */
 //     hemp_debug_scan("\n%d (%02d) : \n", line, src - from);

        while (*src) {
            pnode = hemp_ptree_root(tagset->outline_tags, src);

            if (! pnode)
                break;              /* didn't match first character of tag */
                
            tagstr = src;
            tag    = (hemp_tag) hemp_pnode_match_more(pnode, &src);
    
            if (! tag)
                break;              /* didn't match remaining characters */

            /* add any preceding text */
            if (tagstr > from) {
                hemp_document_scanned_to(
                    document, tagset->text_element, tagstr
                );
            }

            /* update document state before and after call to tag */
            document->scanptr = src;
            tag->scanner(tag, document);
            from = src = document->scanptr;
        }

        /* then look for inline tags within lines */
        while (*src) {
            if (*src == HEMP_LF) {
                src++;
                break;              /* back to the start of line */
            }

            if (*src == HEMP_CR) {
                src++;
                if (*src == HEMP_LF)
                    src++;
                break;              /* back to the start of line */
            }

            pnode = hemp_ptree_root(tagset->inline_tags, src);

            if (! pnode) {
                src++;
                continue;           /* didn't match first character, go on */
            }

            tagstr = src;
            tag    = (hemp_tag) hemp_pnode_match_more(pnode, &src);

            if (! tag) {
                src++;
                continue;           /* didn't matching remaining chars, go on */
            }

            /* create a fragment for any preceding text */
            if (tagstr > from) {
                hemp_document_scanned_to(
                    document, tagset->text_element, tagstr
                );
            }

            document->scanptr = src;
            tag->scanner(tag, document);
            from = src = document->scanptr;
        }
    }

    if (src > from) {
        hemp_document_scanned_to(
            document, tagset->text_element, src
        );
    }
    
    hemp_fragments_add_eof(document->fragments, document->scanpos);

#if HEMP_DEBUG & HEMP_DEBUG_SCAN
    hemp_fragments_dump(document->fragments);
#endif

    return HEMP_TRUE;
}


