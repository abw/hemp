#include <hemp/scanner.h>



hemp_bool_t
hemp_scan_text(
    hemp_template_p tmpl
) {
    hemp_tagset_p   tagset   = tmpl->tagset;
    hemp_cstr_p     text     = hemp_source_read(tmpl->source),
                    src      = text,
                    from     = text,
                    cmptr, 
                    tagstr;
    hemp_pos_t      pos      = 0,
                    line     = 0;
    hemp_tag_p      tag;
    hemp_pnode_p    *inhead  = tagset->inline_tags->head,
                    *outhead = tagset->outline_tags->head,
                    pnode;
    hemp_elements_p elements = tmpl->elements;

    debug_magenta("-- source ---\n%s\n------------\n", text);
    
    while (*src) {
        /* at start of line */
        line++;
        debug_scan("\n%d (%02d) : ", line, src - text);

        if ((pnode = outhead[(hemp_char_t) *src % HEMP_PTREE_SIZE])) {
            debug_yellow("OUTLINE:%c", *src);
            tagstr = src;
            
//          if ((tag = hemp_scan_tag_start(pnode, &src))) {
            if ((tag = (hemp_tag_p) hemp_pnode_match_more(pnode, &src))) {
                if (tagstr > from) {
                    hemp_elements_append(
                        elements, tagset->text_symbol,
                        from, pos, tagstr - from
                    );
                    pos += tagstr - from;
                    from = tagstr;
                }
                tag->scan(tmpl, tag, tagstr, pos, &src);
                from = src;
                pos += src - tagstr;
            }
        }
        
        while (*src) {
            if (*src == HEMP_LF) {
                src++;
                break;
            }
            else if (*src == HEMP_CR) {
                src++;
                if (*src == HEMP_LF)
                    src++;
                break;
            }
            else if ((pnode = inhead[(hemp_char_t) *src % HEMP_PTREE_SIZE])) {
                debug_yellow("INLINE:%c", *src);

                tagstr = src;

//              if ((tag = hemp_scan_tag_start(pnode, &src))) {
                if ((tag = (hemp_tag_p) hemp_pnode_match_more(pnode, &src))) {
                    if (tagstr > from) {
                        hemp_elements_append(
                            elements, tagset->text_symbol,
                            from, pos, tagstr - from
                        );
                        pos += tagstr - from;
                        from = tagstr;
                    }
                    tag->scan(tmpl, tag, tagstr, pos, &src);
                    from = src;
                    pos += src - tagstr;
                }
                else {
                    src++;
                }
                /* TODO: call tag handler which consumes everything up to tag
                 * end.  Set from to src position
                 */
            }
            else {
                //debug("%c", *src);
                src++;
            }
        }
    }

    if (src > from) {
//      debug("got %d characters of trailing text\n", src - from);
        hemp_elements_append(
            elements, tagset->text_symbol,
            from, pos, src - from
        );
        pos += src - from;
    }
    
    hemp_elements_eof(elements, pos);

#if DEBUG
    hemp_elements_dump(elements);
#endif
//  hemp_elements_free(elements);
    
    return HEMP_TRUE;
}


hemp_tag_p
OLD_hemp_scan_tag_start(
    hemp_pnode_p    pnode, 
    hemp_cstr_p     *srcptr
) {
//  debug_blue("hemp_scan_tag_start(%p[%s])]", pnode, pnode->key);

    hemp_cstr_p cmptr  = pnode->key;
    hemp_cstr_p src    = *srcptr;
    hemp_tag_p  tag    = NULL;

    // nuisance: we do the first comparison twice
    while (1) {
        if (*src == *cmptr) {
            src++;

            if (pnode->equal) {
//              debug_yellow("[%c->equal]", *cmptr);
                tag   = pnode->value;     // payload - but only if pnode is set - don't want to trash previous match
                pnode = pnode->equal;
                cmptr = pnode->key;
            }
            else if (* ++cmptr) {
//              debug_yellow("[%c->more => %c]", *(cmptr - 1), cmptr);
            }
            else {
                tag = pnode->value;
                break;
            }
        }
        else if (*src < *cmptr && pnode->before) {
//          debug_blue("[%c is before %c]", *src, *cmptr);
            pnode = pnode->before;
            cmptr = pnode->key;
        }
        else if (*src > *cmptr && pnode->after) {
//          debug_blue("[%c is after %c]", *src, *cmptr);
            pnode = pnode->after;
            cmptr = pnode->key;
        }
        else {
            break;
        }
    }
  
    if (tag) {
        *srcptr = src;
        debug_green("<%s>", tag->name);
    }
    else {
        debug_red("[MISS]");
    }

    return tag;
}

