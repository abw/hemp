#include <hemp/element.h>


/*--------------------------------------------------------------------------
 * tag start
 *--------------------------------------------------------------------------*/

HEMP_ELEMENT(hemp_element_tag_inline) {
    hemp_element_tag_start(hemp, element);
    element->scanner = &hemp_element_tag_inline_scanner;
    return element;
}


HEMP_SCANNER(hemp_element_tag_inline_scanner) {
    hemp_tag        tag     = (hemp_tag) self;
    hemp_string     tagtok  = document->scantok;
    hemp_pos        pos     = document->scanpos;
    hemp_string     src     = document->scanptr,
                    from    = document->scanptr;
    hemp_string     tagend  = tag->end;
    hemp_size       endlen  = strlen(tagend);
    hemp_num        num_val = 0;
    hemp_int        int_val = 0;
    hemp_fragment   fragment;
    hemp_bool       is_int, is_word;
    hemp_pnode      pnode;
    hemp_element    element;

    hemp_debug_msg("hemp_element_tag_inline_scanner()\n");

    // add the tag start token
    hemp_fragments_add_fragment(
        document->fragments, HempElementTagStart,
        tagtok, pos, src - tagtok
    );
    pos += src - tagtok;
    
    document->scanpos = pos;
    document->scantok = src;
    
    is_word = HEMP_FALSE;

    while (*src) {
        if (isspace(*src)) {
            /* whitespace */
            hemp_scan_while(src, isspace);
            hemp_debug_token("SPACE", from, src-from);
            hemp_fragments_add_fragment(
                document->fragments, HempElementSpace,
                from, pos, src - from
            );
        }
        else if (hemp_stringn_eq(src, tagend, endlen)) {      // TODO: end flags
            /* tag end */
            hemp_debug_token("TAG END", from, endlen);
            hemp_fragments_add_fragment(
                document->fragments, HempElementTagEnd,
                from, pos, endlen
            );
            src += endlen;
            break;                                          /* ESCAPE ROUTE */
        }
        else if (isdigit(*src)) {
            /* number - try integer first */
            errno   = 0;
            int_val = strtol(src, &src, 0);
            is_int  = HEMP_TRUE;

            /* If there's a decimal point and a digit following then it's a 
             * floating point number.  We also look out for e/E which also
             * indicate fp numbers in scientific notation, e.g. 1.23e6.
             * Note that we don't accept the decimal point if the next 
             * character is not a digit.  This is required to support methods
             * called against numeric constants, e.g. 12345.hex 
             */
            if ( ( *src == '.' && isdigit(*(src + 1)) )
              || ( *src == 'e' || *src == 'E' )
            )  {
                is_int  = HEMP_FALSE;
                num_val = strtod(from, &src);
            }

            if (errno == ERANGE) {
                /* TODO: trim next token out of text */
                hemp_throw(document->hemp, HEMP_ERROR_OVERFLOW, from);
            }
            else if (errno) {
                /* should never happen (famous last words) as we pre-check 
                 * that there is at least one valid digit available to be 
                 * parsed, but we check anyway
                 */
                hemp_fatal("Unknown number parsing error: %d", errno);
            }
            else if (is_int) {
                hemp_debug_token("INTEGER", from, src-from);
                fragment = hemp_fragments_add_fragment(
                    document->fragments, HempElementInteger,
                    from, pos, src - from
                );
                fragment->args.value = hemp_int_val(int_val);
            }
            else {
                hemp_debug_token("NUMBER", from, src-from);
                fragment = hemp_fragments_add_fragment(
                    document->fragments, HempElementNumber,
                    from, pos, src - from
                );
                fragment->args.value = hemp_num_val(num_val);
            }
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
                hemp_debug_msg("%s element has dedicated scanner\n", element->name);
                element->scanner(element, document);
            }
            else {
                hemp_fragments_add_fragment(
                    document->fragments, element,
                    from, pos, src - from
                );
            }
        }
        else if (isalpha(*src)) {
bareword:
            /* word */
            hemp_scan_while(src, isalnum);
            // TODO: check for ':' following after, e.g. file:/blah/blah
            hemp_debug_token("WORD", from, src-from);
            hemp_fragments_add_fragment(
                document->fragments, HempElementWord,
                from, pos, src - from
            );
        }
        else {
            hemp_throw(document->hemp, HEMP_ERROR_TOKEN, src);
            break;
        }

        pos += src - from;
        from = src;
        document->scanpos = pos;
        document->scantok = src;
    }
    
    document->scanpos = pos;
    document->scanptr = src;

    return HEMP_TRUE;
}

