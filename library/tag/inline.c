#include <limits.h>
#include <errno.h>
#include "hemp/element.h"
#include "hemp/tags.h"
#include "hemp/scanner.h"


static struct hemp_tag 
    hemp_inline_tag = { 
        HEMP_INLINE_TAG,
        "hemp.tag.inline", 
        "[%", "%]", 
        &hemp_scan_inline_tag 
    };


hemp_tag_t HempTagInline = &hemp_inline_tag;



void 
hemp_scan_inline_tag(
    hemp_template_t tmpl,
    hemp_tag_t      tag,
    hemp_cstr_t     tagtok,
    hemp_pos_t      pos,
    hemp_cstr_t     *srcptr
) {
    hemp_cstr_t     src    = *srcptr,
                    from   = src,
                    point;
    hemp_name_t     tagend = tag->end;
    hemp_size_t     endlen = strlen(tagend);
    hemp_element_t  element;
    hemp_num_t      num_val;
    hemp_int_t      int_val;
    hemp_bool_t     is_int, is_fixed;

    debug_call("hemp_scan_inline_tag()\n");

    // add the tag start token
    hemp_elements_append(
        tmpl->elements, HempElementTagStart,
        tagtok, pos, src - tagtok
    );
    pos += src - tagtok;

    while (*src) {
        if (isspace(*src)) {
            /* whitespace */
            hemp_scan_while(src, isspace);
            debug_token("SPACE", from, src-from);
            hemp_elements_append(
                tmpl->elements, HempElementSpace,
                from, pos, src - from
            );
        }
        else if (hemp_cstrn_eq(src, tagend, endlen)) {      // TODO: end flags
            /* tag end */
            debug_token("TAG END", from, endlen);
            hemp_elements_append(
                tmpl->elements, HempElementTagEnd,
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
                    tmpl->elements, HempElementInteger,
                    from, pos, src - from
                );
                // TODO: pre-set value of element to int_val
            }
            else {
                debug_token("NUMBER", from, src-from);
                hemp_elements_append(
                    tmpl->elements, HempElementNumber,
                    from, pos, src - from
                );
                // TODO: pre-set value of element to num_val
            }
        }
        else if (isalpha(*src)) {
            /* word */
            hemp_scan_while(src, isalnum);
            // TODO: lookup keyword
            debug_token("WORD", from, src-from);
            hemp_elements_append(
                tmpl->elements, HempElementWord,
                from, pos, src - from
            );
        }
        else if (*src == HEMP_CHAR_SQUOTE) {
            /* single quotes */
            is_fixed = HEMP_TRUE;

            /* walk to the end */
            while ( * ++src && *src != HEMP_CHAR_SQUOTE ) {
                if (*src == HEMP_CHAR_SLASH) {
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
                tmpl->elements, HempElementSQuote,
                from, pos, src - from
            );

            if (is_fixed) {
                /* we can generate the output text from the source token */
                hemp_set_flag(element, HEMP_FLAG_FIXED);
            }
            else {
                /* we need to create a new string with escapes resolved */
                hemp_cstr_t squote = 
                element->value.text = hemp_mem_init(src - from - 1);
                hemp_cstr_t sqfrom = from + 1;
                    
                while (sqfrom < src) {
                    /* skip past the '\' if we've got "\\" or "\'" */
                    if (*sqfrom == HEMP_CHAR_SLASH 
                    && ( *(sqfrom + 1) == HEMP_CHAR_SQUOTE 
                    ||   *(sqfrom + 1) == HEMP_CHAR_SLASH ))
                        sqfrom++;

                    *squote++ = *sqfrom++;
                }
                *--squote = HEMP_CHAR_NUL;
            }
        }
        else if (*src == HEMP_CHAR_DQUOTE) {
            /* double quotes */
            is_fixed = HEMP_TRUE;

            /* walk to the end */
            while ( * ++src && *src != HEMP_CHAR_DQUOTE ) {
                if (*src == HEMP_CHAR_SLASH) {
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
                tmpl->elements, HempElementDQuote,
                from, pos, src - from
            );

            if (is_fixed) {
                /* we can generate the output text from the source token */
                hemp_set_flag(element, HEMP_FLAG_FIXED);
            }
            else {
                /* we need to create a new string with escapes resolved */
                hemp_cstr_t dquote  = 
                element->value.text = hemp_mem_init(src - from - 1);
                hemp_cstr_t dqfrom  = from + 1;

                while (dqfrom < src) {
                    /* skip past the '\' if we've got "\\" or "\'" */
                    if (*dqfrom == HEMP_CHAR_SLASH) {
                        switch (*(dqfrom + 1)) {
                            case HEMP_CHAR_DQUOTE:
                            case HEMP_CHAR_SLASH:
                                /* \" or \\  =>  " or \ */
                                dqfrom++;
                                break;

                            case 'n':
                                /* \n => newline (currently just LF) */
                                *dquote++ = HEMP_CHAR_NL;
                                dqfrom += 2;
                                break;

                            case 't':
                                /* \t => tab */
                                *dquote++ = HEMP_CHAR_TAB;
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
                *--dquote = HEMP_CHAR_NUL;
            }
        }
        else if (*src == HEMP_CHAR_COMMENT) {
            /* walk to the end of line or end of tag */
            while (* ++src) {
                if (*src == HEMP_CHAR_LF) {
                    src++;
                    break;
                }
                else if (*src == HEMP_CHAR_CR) {
                    src++;
                    if (*src == HEMP_CHAR_LF)
                        src++;
                    break;
                }
                else if (*src == *tagend && hemp_cstrn_eq(src, tagend, endlen)) {
                    break;
                }
            }
            hemp_elements_append(
                tmpl->elements, HempElementComment,
                from, pos, src - from
            );
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

