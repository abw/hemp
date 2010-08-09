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
    hemp_bool_t     is_int, is_fixed;
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
                num_val = strtod(from, &src);
                is_int  = HEMP_FALSE;
            }

            if (errno == ERANGE) {
                debug_red("number out of range\n");     // TODO: proper handling
            }
            else if (errno) {
                /* should never happen (famous last words) as we pre-check 
                 * that there is at least one valid digit available to be 
                 * parsed, but we check anyway
                 */
                debug_red("number error\n");     // TODO: proper handling
                perror("number");
            }
            else if (is_int) {
                debug_token("INTEGER", from, src-from);
                hemp_elements_append(
                    tmpl->elements, HempSymbolInteger,
                    from, pos, src - from
                );
                // TODO: pre-set value of element to int_val
            }
            else {
                debug_token("NUMBER", from, src-from);
                hemp_elements_append(
                    tmpl->elements, HempSymbolNumber,
                    from, pos, src - from
                );
                // TODO: pre-set value of element to num_val
            }
        }
        else if (isalpha(*src)) {
            /* word */
            hemp_scan_while(src, isalnum);
            // TODO: check for ':' following after, e.g. file:/blah/blah
            // TODO: lookup keyword
            debug_token("WORD", from, src-from);
            hemp_elements_append(
                tmpl->elements, HempSymbolWord,
                from, pos, src - from
            );
        }
        else if (*src == HEMP_SQUOTE) {
            /* single quotes */
            is_fixed = HEMP_TRUE;

            /* walk to the end */
            while ( * ++src && *src != HEMP_SQUOTE ) {
                if (*src == HEMP_BACKSLASH) {
                    src++;
                    is_fixed = HEMP_FALSE;
                }
            }

            /* check we hit a quote and not the end of string */
            if (! *src)
                hemp_fatal("unterminated single quoted string: %s", from);

            src++;

            debug_token("SQUOTE", from, src-from);

            element = hemp_elements_append(
                tmpl->elements, HempSymbolSQuote,
                from, pos, src - from
            );

            if (is_fixed) {
                /* we can generate the output text from the source token */
                hemp_set_flag(element, HEMP_IS_FIXED);
            }
            else {
                /* we need to create a new string with escapes resolved */
                hemp_cstr_p squote  = 
                element->value.text = (hemp_cstr_p) hemp_mem_alloc(src - from - 1);
                hemp_cstr_p sqfrom  = from + 1;
                    
                while (sqfrom < src) {
                    /* skip past the '\' if we've got "\\" or "\'" */
                    if (*sqfrom == HEMP_BACKSLASH 
                    && ( *(sqfrom + 1) == HEMP_SQUOTE 
                    ||   *(sqfrom + 1) == HEMP_BACKSLASH ))
                        sqfrom++;

                    *squote++ = *sqfrom++;
                }
                *--squote = HEMP_NUL;
            }
        }
        else if (*src == HEMP_DQUOTE) {
            /* double quotes */
            is_fixed = HEMP_TRUE;

            /* walk to the end */
            while ( * ++src && *src != HEMP_DQUOTE ) {
                if (*src == HEMP_BACKSLASH) {
                    src++;
                    is_fixed = HEMP_FALSE;
                }
                /* TODO: also look for embedded vars, e.g. $foo */
            } 

            /* check we hit a quote and not the end of string */
            if (! *src)
                hemp_fatal("unterminated double quoted string: %s", from);

            src++;

            debug_token("DQUOTE", from, src-from);

            element = hemp_elements_append(
                tmpl->elements, HempSymbolDQuote,
                from, pos, src - from
            );

            if (is_fixed) {
                /* we can generate the output text from the source token */
                hemp_set_flag(element, HEMP_IS_FIXED);
            }
            else {
                /* we need to create a new string with escapes resolved */
                hemp_cstr_p dquote  = 
                element->value.text = hemp_mem_alloc(src - from - 1);
                hemp_cstr_p dqfrom  = from + 1;

                while (dqfrom < src) {
                    /* skip past the '\' if we've got "\\" or "\'" */
                    if (*dqfrom == HEMP_BACKSLASH) {
                        switch (*(dqfrom + 1)) {
                            case HEMP_DQUOTE:
                            case HEMP_BACKSLASH:
                                /* \" or \\  =>  " or \ */
                                dqfrom++;
                                break;

                            case 'n':
                                /* \n => newline (currently just LF) */
                                *dquote++ = HEMP_NL;
                                dqfrom += 2;
                                break;

                            case 't':
                                /* \t => tab */
                                *dquote++ = HEMP_TAB;
                                dqfrom += 2;
                                break;

                            /* \X => X */
                            default:
                                *dquote++ = *dqfrom++;
                        }
                    }
                    else {
                        *dquote++ = *dqfrom++;
                    }
                }
                *--dquote = HEMP_NUL;
            }
        }
        else if (
            (pnode  = HEMP_IN_PTREE(ophead, src))
        &&  (symbol = (hemp_symbol_p) hemp_pnode_match_more(pnode, &src))
        ) {
            debug("[matched operator: %s]\n", symbol->name);
            if (symbol->scanner) {
                debug("symbol has dedicated scanner\n");
                symbol->scanner(tmpl, tag, from, pos, &src, symbol);
            }
            else {
                hemp_elements_append(
                    tmpl->elements, symbol,
                    from, pos, src - from
                );
            }
        }
        else {
            // TODO: operators
            hemp_fatal("unrecognised token\n");
            break;
        }

        pos += src - from;
        from = src;
    }
    
    *srcptr = src;
}

