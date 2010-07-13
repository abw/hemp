#include <limits.h>
#include <errno.h>
#include "hemp/tags.h"


static struct hemp_tag 
    hemp_inline_tag = { 
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
    hemp_bool_t     is_int;

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
            break;
        }
        else if (isdigit(*src)) {
            /* number - try integer first */
            int_val = strtol(src, &src, 0);
            is_int  = HEMP_TRUE;

            /* If there's a decimal point and a digit following then it's a 
             * floating point number.  We also look out for e/E which also
             * indicate fp numbers in scientific notation, e.g. 1.23e6.
             * Note that we don't accept the decimal point if the next 
             * character is not a digit.  This is required to support methods
             * called against numeric constants, e.g. 12345.hex 
             */
            if (
                (*src == '.' && isdigit(*(src + 1)))
             || (*src == 'e' || *src == 'E')
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
            debug_token("WORD", from, src-from);
            hemp_elements_append(
                tmpl->elements, HempElementWord,
                from, pos, src - from
            );
        }
        else if (*src == '\'') {
            /* single quotes */
            // TODO: check for escaped quotes and backslashes
            do { src++; } while ( *src && *src != '\'' );
            src++;
            // hemp_scan_while(src, isalnum);
            debug_token("SQUOTE", from, src-from);
            hemp_todo("single quoted strings");
            hemp_elements_append(
                tmpl->elements, HempElementSQuote,
                from, pos, src - from
            );
        }
        else if (*src == '"') {
            /* double quotes */
            // TODO: check for escaped quotes, backslashes and \n, \t, etc
            do { src++; } while ( *src && *src != '"' );
            src++;  // TODO: check runnaways
            // hemp_scan_while(src, isalnum);
            debug_token("DQUOTE", from, src-from);
            hemp_todo("double quoted strings");
            hemp_elements_append(
                tmpl->elements, HempElementDQuote,
                from, pos, src - from
            );
        }

        // TODO: comment, checking for tag end
        // TODO: operators
        else {
            debug_yellow("something else\n");
            break;
        }

        pos += src - from;
        from = src;
    }
    
    *srcptr = src;
}





