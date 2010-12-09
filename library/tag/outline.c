#include <hemp/tag.h>


HEMP_TAG(hemp_tag_outline) {
//  hemp_debug_msg(
//      "hemp_tag_outline(%p, %s, %s, %s, %s, %s)\n",
//      hemp, type, name, start, end ? end : "", 
//      grammar ? grammar->name : "no grammar"
//  );
    hemp_tag tag = hemp_tag_new(
        type, name, start, end, grammar
    );
    tag->style   = HEMP_OUTLINE_TAG;
    tag->scanner = &hemp_tag_outline_scanner;
    return tag;
}


/* NOTE: this has been pasted in from inline.c and tweaked a little.
 * Both could do with some refactoring, cleaning up, and if possible, 
 * either a unification of the two, or abstraction of common functionality.
 */


HEMP_SCANNER(hemp_tag_outline_scanner) {
    hemp_tag        tag     = (hemp_tag) self;
    hemp_string     src     = template->scanptr,
                    from    = template->scanptr;
    hemp_pnode      pnode;
    hemp_element    element;

    hemp_debug("hemp_tag_outline_tag()\n");

    // add the tag start token
    hemp_template_scanned(template, HempElementTagStart);

    while (*src) {
        /* This is similar to the inline tag scanner but differs in the way
         * whitespace and end-of-line characters are handled
         */
        if (*src == HEMP_LF) {                  /* line feed                */
            src++;
            break;                              /* ESCAPE ROUTE             */
        }
        else if (*src == HEMP_CR) {             /* carriage return (+ lf)   */
            if (*++src == HEMP_LF)
                src++;
            break;                              /* ESCAPE ROUTE             */
        }
        else if (isspace(*src)) {               /* whitespace               */
            do { 
                src++; 
            } while ( 
                isspace(*src) && *src != HEMP_CR && *src != HEMP_LF
            );
            hemp_template_scanned_to(template, HempElementSpace, src);
        }
        else if (isdigit(*src)) {
            hemp_scan_number(template);
            src = template->scanptr;
        }
        else if (
            (pnode   = hemp_ptree_root(tag->grammar->operators, src))
        &&  (element = (hemp_element) hemp_pnode_match_more(pnode, &src))
        ) {
            hemp_debug_token("OPERATOR", from, src-from);

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
            hemp_throw(template->dialect->hemp, HEMP_ERROR_TOKEN, src);
            break;
        }
    }

    /* tag end - newlines characters matched but not yet consumed */
    hemp_template_scanned_to(
        template, HempElementTagEnd, src
    );

    return HEMP_TRUE;
}
