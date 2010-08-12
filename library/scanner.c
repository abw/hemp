#include <hemp/scanner.h>

hemp_scan_pos_p
hemp_scan_pos_init(HEMP_SCAN_ARGS) {
    hemp_scan_pos_p scan_pos = (hemp_scan_pos_p) hemp_mem_alloc(
        sizeof(struct hemp_scan_pos_s)
    );

    if (! scan_pos)
        hemp_mem_fail("scanner position");
    
    scan_pos->tmpl    = tmpl;
    scan_pos->tag     = tag;
    scan_pos->start   = start;
    scan_pos->pos     = pos;
    scan_pos->current = *srcptr;
    scan_pos->symbol  = symbol;

    return scan_pos;
}


void
hemp_scan_pos_free(
    hemp_scan_pos_p scan_pos
) {
    hemp_mem_free(scan_pos);
}


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
            debug_parse("OUTLINE:%c", *src);
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
                debug_parse("INLINE:%c", *src);

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

#if DEBUG & DEBUG_PARSE
    hemp_elements_dump(elements);
#endif
//  hemp_elements_free(elements);
    
    return HEMP_TRUE;
}


