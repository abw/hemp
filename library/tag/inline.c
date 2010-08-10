#include <limits.h>
#include <errno.h>
#include <hemp/core.h>
#include <hemp/ptree.h>
#include <hemp/element.h>
#include <hemp/tag.h>
#include <hemp/scanner.h>
#include <hemp/symbol.h>
#include <hemp/grammar.h>


#define HEMP_IN_PTREE(head,src) \
    head[(hemp_char_t) *src % HEMP_PTREE_SIZE]


void 
hemp_scan_inline_tag(
    hemp_template_p tmpl,
    hemp_tag_p      tag,
    hemp_cstr_p     tagtok,
    hemp_pos_t      pos,
    hemp_cstr_p     *srcptr
) {
    hemp_cstr_p     src    = *srcptr,
                    from   = src,
                    point;
    hemp_cstr_p     tagend = tag->end;
    hemp_size_t     endlen = strlen(tagend);
    hemp_element_p  element;
    hemp_num_t      num_val;
    hemp_int_t      int_val;
    hemp_bool_t     is_int;
    hemp_pnode_p    pnode;
    hemp_pnode_p    *ophead  = tag->grammar->operators->head;
    hemp_symbol_p   symbol;

    debug_call("hemp_scan_inline_tag()\n");

    // add the tag start token
    hemp_elements_append(
        tmpl->elements, HempSymbolTagStart,
        tagtok, pos, src - tagtok
    );
    pos += src - tagtok;

    while (*src) {
        if (isspace(*src)) {
            /* whitespace */
            hemp_scan_while(src, isspace);
            debug_token("SPACE", from, src-from);
            hemp_elements_append(
                tmpl->elements, HempSymbolSpace,
                from, pos, src - from
            );
        }
        else if (hemp_cstrn_eq(src, tagend, endlen)) {      // TODO: end flags
            /* tag end */
            debug_token("TAG END", from, endlen);
            hemp_elements_append(
                tmpl->elements, HempSymbolTagEnd,
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
                debug_token("INTEGER", from, src-from);
                element = hemp_elements_append(
                    tmpl->elements, HempSymbolInteger,
                    from, pos, src - from
                );
                element->args.value = HEMP_INT_VAL(int_val);
            }
            else {
                debug_token("NUMBER", from, src-from);
                element = hemp_elements_append(
                    tmpl->elements, HempSymbolNumber,
                    from, pos, src - from
                );
                element->args.value = HEMP_NUM_VAL(num_val);
            }
        }
        else if (
            (pnode  = HEMP_IN_PTREE(ophead, src))
        &&  (symbol = (hemp_symbol_p) hemp_pnode_match_more(pnode, &src))
        ) {
            debug_token("OPERATOR", from, src-from);
//          debug("[matched operator: %s]\n", symbol->name);
            if (symbol->scanner) {
//              debug("symbol has dedicated scanner\n");
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
            /* word */
            hemp_scan_while(src, isalnum);
            // TODO: check for ':' following after, e.g. file:/blah/blah
            // TODO: lookup keyword
            // TODO: on second thoughts, try pnode match first, for things
            //       like C< >... DONE, by moving this down... I think ???
            debug_token("WORD", from, src-from);
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
    
    *srcptr = src;
}

