#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "hemp/scanner.h"



hemp_bool_t
hemp_scan_text(
    hemp_template_t tmpl
) {
    hemp_tagset_t   tagset   = tmpl->dialect->tagset;
    hemp_cstr_t     text     = hemp_source_read(tmpl->source),
                    src      = text,
                    from     = text,
                    cmptr, 
                    tagstr;
    hemp_pos_t      pos      = 0,
                    line     = 0;
    hemp_tag_t      tag;
    hemp_pnode_t    *inhead  = tagset->inline_tags->head,
                    *outhead = tagset->outline_tags->head,
                    pnode;
    hemp_elements_t elements = tmpl->elements;

    debug_magenta("-- source ---\n%s\n------------\n", text);
    
    while (*src) {
        /* at start of line */
        line++;
        debug_scan("\n%d (%02d) : ", line, src - text);

        if ((pnode = outhead[(unsigned char) *src])) {
            debug_yellow("OUTLINE:%c", *src);
            tagstr = src;
            
            if ((tag = hemp_scan_tag_start(pnode, &src))) {
                if (tagstr > from) {
                    hemp_elements_append(
                        elements, tagset->text_element,
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
            else if ((pnode = inhead[(unsigned char) *src])) {
                debug_yellow("INLINE:%c", *src);

                tagstr = src;

                if ((tag = hemp_scan_tag_start(pnode, &src))) {
                    if (tagstr > from) {
                        hemp_elements_append(
                            elements, tagset->text_element,
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
            elements, tagset->text_element,
            from, pos, src - from
        );
        pos += src - from;
    }
    
    hemp_elements_eof(elements, src, pos);

#if DEBUG
    hemp_elements_dump(elements);
#endif
//  hemp_elements_free(elements);
    
    return 1;
}


hemp_tag_t
hemp_scan_tag_start(
    hemp_pnode_t    pnode, 
    hemp_cstr_t     *srcptr
) {
//  debug_blue("hemp_scan_tag_start(%p[%s])]", pnode, pnode->key);

    hemp_cstr_t cmptr  = pnode->key;
    hemp_cstr_t src    = *srcptr;
    hemp_tag_t  tag    = NULL;

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

