#include <hemp/scanner.h>



hemp_bool_t
hemp_scan_text(
    hemp_template_p tmpl
) {
    hemp_elements_p elements = tmpl->elements;
    hemp_tagset_p   tagset   = tmpl->tagset;
    hemp_pnode_p    *inhead  = tagset->inline_tags->head,
                    *outhead = tagset->outline_tags->head,
                    pnode;
    hemp_cstr_p     text     = hemp_source_read(tmpl->source),
                    src      = text,
                    from     = text,
                    cmptr, tagstr;
    hemp_pos_t      pos      = 0,
                    line     = 0;
    hemp_tag_p      tag;

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


