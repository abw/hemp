#include <hemp/tag.h>


/* NOTE: this has been pasted in from inline.c and tweaked a little.
 * Both could do with some refactoring, cleaning up, and if possible, 
 * either a unification of the two, or abstraction of common functionality.
 */

void 
hemp_outline_tag_scanner(
    HEMP_TAG_SCAN_ARGS
) {
    hemp_string      src    = *srcptr,
                    from   = src,
                    point;

    hemp_element  element;
    hemp_num      num_val;
    hemp_int      int_val;
    hemp_bool     is_int, is_word;
    hemp_pnode    pnode;
    hemp_symbol   symbol;

    hemp_debug("hemp_scan_outline_tag()\n");

    // add the tag start token
    hemp_elements_append(
        tmpl->elements, HempSymbolTagStart,
        tagtok, pos, src - tagtok
    );
    pos += src - tagtok;
    
    is_word = HEMP_FALSE;

    while (*src) {
        /* This is similar to the inline tag scanner but differs in the way
         * whitespace and end-of-line characters are handled
         */
        if (*src == HEMP_LF) {
            src++;
            break;                                          /* ESCAPE ROUTE */
        }
        else if (*src == HEMP_CR) {
            src++;
            if (*src == HEMP_LF)
                src++;
            break;                                          /* ESCAPE ROUTE */
        }
        else if (isspace(*src)) {
            do { 
                src++; 
            } while ( 
                isspace(*src) && *src != HEMP_CR && *src != HEMP_LF
            );
            /* whitespace */
            hemp_debug_token("SPACE", from, src-from);
            hemp_elements_append(
                tmpl->elements, HempSymbolSpace,
                from, pos, src - from
            );
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
                hemp_throw(tmpl->dialect->hemp, HEMP_ERROR_OVERFLOW, from);
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
                element = hemp_elements_append(
                    tmpl->elements, HempSymbolInteger,
                    from, pos, src - from
                );
                element->args.value = hemp_int_val(int_val);
            }
            else {
                hemp_debug_token("NUMBER", from, src-from);
                element = hemp_elements_append(
                    tmpl->elements, HempSymbolNumber,
                    from, pos, src - from
                );
                element->args.value = hemp_num_val(num_val);
            }
        }
        else if (
            (pnode  = hemp_ptree_root(tag->grammar->operators, src))
        &&  (symbol = (hemp_symbol) hemp_pnode_match_more(pnode, &src))
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

            if (symbol->scanner) {
                symbol->scanner(tmpl, tag, from, pos, &src, symbol);
            }
            else {
                hemp_elements_append(
                    tmpl->elements, symbol,
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
            hemp_elements_append(
                tmpl->elements, HempSymbolWord,
                from, pos, src - from
            );
        }
        else {
            hemp_throw(tmpl->dialect->hemp, HEMP_ERROR_TOKEN, src);
            break;
        }

        pos += src - from;
        from = src;
    }

    /* tag end */
    hemp_debug_token("TAG END", from, src - from);
    hemp_elements_append(
        tmpl->elements, HempSymbolTagEnd,
        from, pos, src - from
    );
    
    *srcptr = src;
}



hemp_string
hemp_outline_tag_to_end_of_line(
    HEMP_TAG_SKIP_ARGS
) {
    while ( *src
        &&  hemp_not_newline(src) 
    ) {
        src++;
    }

    return src;
}
