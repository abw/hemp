#include <hemp/tag.h>


HEMP_TAG(hemp_tag_inline) {
//  hemp_debug_msg(
//      "hemp_tag_inline(%p, %s, %s, %s, %s, %s)\n",
//      hemp, type, name, start, end ? end : "", 
//      grammar ? grammar->name : "no grammar"
//  );

    hemp_tag tag = hemp_tag_new(
        type, name, start, end, grammar
    );
    tag->style   = HEMP_INLINE_TAG;
    tag->scanner = &hemp_tag_inline_scanner;
    tag->to_eol  = &hemp_tag_inline_to_eol;

    return tag;
}


hemp_bool
hemp_tag_inline_scanner(
    hemp_memory     self,
    hemp_template   template
) {
    hemp_tag        tag     = (hemp_tag) self;
    hemp_string     src     = template->scanptr;
    hemp_string     tagend  = tag->end;
    hemp_size       endlen  = strlen(tagend);
    hemp_bool       closed  = HEMP_FALSE;
    hemp_pnode      pnode;
    hemp_element    element;

    hemp_template_enter_tag(template, tag);

    // add the tag start token
    hemp_template_scanned(template, HempElementTagStart);

    src = template->scanptr;

    while (*src) {
        if (isspace(*src)) {
            /* whitespace */
            hemp_scan_while(src, isspace);
            hemp_template_scanned_to(template, HempElementSpace, src);
        }
        else if (hemp_stringn_eq(src, tagend, endlen)) {      // TODO: end flags
            /* tag end */
            src += endlen;
            hemp_template_scanned_to(template, HempElementTagEnd, src);
            closed = HEMP_TRUE;
            break;                                          /* ESCAPE ROUTE */
        }
        else if (isdigit(*src)) {
            hemp_scan_number(template);
            src = template->scanptr;
        }
        else if (
            (pnode   = hemp_ptree_root(tag->grammar->operators, src))
        &&  (element = (hemp_element) hemp_pnode_match_more(pnode, &src))
        ) {
            /* We have to be check that we haven't matched the first part of
             * a longer word, e.g. matching 'le' at the start of 'length'.
             * However, we also have to account for the fact that symbols
             * may contain a mixture of word and non-word characters, e.g.
             * 'C<', 'q/' and so on.  It's OK to have a non-word followed by
             * a word, e.g. 'q/a' ('q/' is the operator, 'a' the next word),
             * or a word followed by a non-word, e.g. '+foo'.
             *
             * If we find that we are at a word/word boundary then we pretend
             * this bit never happened by jumping down to the next check...
             * don't look if you're squeamish about the use of "goto"
             */
            if (isalpha(*src) && isalpha(*(src - 1)))
                goto bareword;

            if (element->scanner) {
                template->scanptr = src;
                element->scanner(element, template);
                src = template->scanptr;
            }
            else {
                hemp_template_scanned_to(
                    template, element, src
                );
            }
        }
        else if (isalpha(*src)) {
bareword:
            /* word */
            hemp_scan_while(src, isalnum);
            // TODO: check for ':' following after, e.g. file:/blah/blah
            hemp_template_scanned_to(
                template, HempElementWord, src
            );
        }
        else {
            hemp_debug_msg("unexpected token: %s\n", src);
            hemp_throw(template->dialect->hemp, HEMP_ERROR_TOKEN, src);
            break;
        }

//        template->scantok = src;
    }
    
    template->scanptr = src;

    if (tag->end && ! closed)
        hemp_fatal("Missing tag end: %s", tag->end);

    hemp_template_leave_tag(template);

    return HEMP_TRUE;
}



hemp_string
hemp_tag_inline_to_eol(
    HEMP_TAG_SKIP_ARGS
) {
    hemp_debug_call("hemp_tag_inline_to_eol()\n");
    hemp_string tag_end = tag->end;
    hemp_size   tag_len = strlen(tag->end);

    /* walk to the end of line or end of tag */
    while ( *src
        &&  hemp_not_newline(src) 
        &&  hemp_not_tag_end(src, tag_end, tag_len)
    ) {
        src++;
    }
    
    return src;
}
