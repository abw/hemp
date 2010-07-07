#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "hemp/scanner.h"


void hemp_scan_text(
    hemp_text_t     text, 
    hemp_tagset_t   tagset
) {
    hemp_text_t 
        src     = text,
        from    = text,
        cmptr, tag_pos;
    hemp_pos_t  
        pos     = 0,
        line    = 0;
    hemp_tag_t
        tag;
    hemp_pnode_t
        *inline_head  = tagset->inline_tags->head,
        *outline_head = tagset->outline_tags->head,
        pnode;
    hemp_elements_t
        elements = hemp_elements_init(0);

    debug_magenta("Source text: %s\n", text);
//    debug("Allocated %d x %d bytes = %d: %d\n", capacity, sizeof(struct attic_element), sizeof(struct attic_element) * capacity );
//    debug("Elements at: %p\n", elements);
//    debug("First element: %p\n", element);
//    debug("pos: %d\n", pos);
    
    while (*src) {
        /* at start of line */
        line++;
        debug("\n%d (%02d) : ", line, src - text);

        if ((pnode = outline_head[(unsigned char) *src])) {
            debug_yellow("OUTLINE:%c", *src);
//            hemp_scan_text_pnode(pnode, &src);
        }
        
        while (*src) {
            if (*src == LF) {
                src++;
                break;
            }
            else if (*src == CR) {
                src++;
                if (*src == LF)
                    src++;
                break;
            }
            else if ((pnode = inline_head[(unsigned char) *src])) {
                debug_red("INLINE:%c", *src);

                tag     = NULL;
                tag_pos = src;
                cmptr   = pnode->key;

                // nuisance: we do the first comparison twice
                while (1) {
                    if (*src == *cmptr) {
                        src++;

                        if (pnode->equal) {
//                          debug_yellow("[%c->equal]", *cmptr);
                            tag   = pnode->value;     // payload - but only if pnode is set - don't want to trash previous match
                            pnode = pnode->equal;
                            cmptr = pnode->key;
                        }
                        else if (* ++cmptr) {
//                          debug_yellow("[%c->more => %c]", *(cmptr - 1), cmptr);
                        }
                        else {
                            tag = pnode->value;
                            break;
                        }
                    }
                    else if (*src < *cmptr && pnode->before) {
//                      debug_yellow("[%c is before %c]", *src, *cmptr);
                        pnode = pnode->before;
                        cmptr = pnode->key;
                    }
                    else if (*src > *cmptr && pnode->after) {
//                      debug_yellow("[%c is after %c]", *src, *cmptr);
                        pnode = pnode->after;
                        cmptr = pnode->key;
                    }
                    else {
                        break;
                    }
                }
                if (tag) {
                    debug_green("[%s]", tag->name);

                    if (tag_pos > from) {
                        //debug_cyan("[INLINE@%d] element: %p\n", pos, element);
                        //debug_cyan("got %d characters of preceding text\n", src - from);
                        hemp_elements_append(
                            elements, tagset->text_element,
                            from, pos, tag_pos - from
                        );
                        pos += tag_pos - from;
                        from = tag_pos;
                    }

                    hemp_elements_append(
                        elements, HempElementTagStart,
                        tag_pos, pos, src - tag_pos
                    );
                    from = src;
                    pos += src - tag_pos;
                }
                /* TODO: call tag handler which consumes everything up to tag
                 * end.  Set from to src position
                 */
                 //src++;
            }
            else {
                debug("%c", *src);
                src++;
            }
        }
    }

    if (src > from) {
        debug("got %d characters of trailing text\n", src - from);
        hemp_elements_append(
            elements, tagset->text_element,
            from, pos, src - from
        );
        pos += src - from;
    }
    
    debug("\n");
    hemp_elements_eof(elements, src, pos);

    hemp_elements_dump(elements);
    hemp_elements_free(elements);
}
