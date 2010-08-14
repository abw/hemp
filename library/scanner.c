#include <hemp/scanner.h>


/*--------------------------------------------------------------------------
 * scan_pos is a temporary hack to store scan position in an exception
 *--------------------------------------------------------------------------*/

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


/*--------------------------------------------------------------------------
 * main function for scanning template source to identify chunks of plain
 * text and embedded tags.
 *--------------------------------------------------------------------------*/

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

#if HEMP_DEBUG & HEMP_DEBUG_SCAN
    hemp_debug_magenta("-- source ---\n%s\n-------------\n", text);
#endif

    while (*src) {
        /* at start of line */
        line++;
        hemp_debug_scan("\n%d (%02d) : ", line, src - text);

        if ((pnode = outhead[(hemp_char_t) *src % HEMP_PTREE_SIZE])) {
            tagstr = src;
            
            if ((tag = (hemp_tag_p) hemp_pnode_match_more(pnode, &src))) {
                hemp_debug_scan("[OUTLINE:%c]", *tagstr);

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
                tagstr = src;

                if ((tag = (hemp_tag_p) hemp_pnode_match_more(pnode, &src))) {
                    hemp_debug_scan("[INLINE:%c]", *tagstr);

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
                src++;
            }
        }
    }

    if (src > from) {
        hemp_elements_append(
            elements, tagset->text_symbol,
            from, pos, src - from
        );
        pos += src - from;
    }
    
    hemp_elements_eof(elements, pos);

#if HEMP_DEBUG & HEMP_DEBUG_SCAN
    hemp_elements_dump(elements);
#endif

    return HEMP_TRUE;
}


