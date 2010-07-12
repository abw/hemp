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
    hemp_ident_t    tagend = tag->end;
    hemp_size_t     endlen = strlen(tagend);
    char tmp[100];

    debug_call("hemp_scan_tag()\n");

    // add the tag start token
    hemp_elements_append(
        tmpl->elements, HempElementTagStart,
        tagtok, pos, src - tagtok
    );
    pos += src - tagtok;

    while (*src) {
        if (isspace(*src)) {
            while (isspace(*++src)) {                     // TODO: and not tag end
                // advance
            }
            debug_token("SPACE", from, src-from);
            hemp_elements_append(
                tmpl->elements, HempElementSpace,
                from, pos, src - from
            );
        }
        else if (! strncmp(src, tagend, endlen)) {      // TODO: end flags
            debug_token("TAG END", from, endlen);
            hemp_elements_append(
                tmpl->elements, HempElementTagEnd,
                from, pos, endlen
            );
            src += endlen;
            break;
        }
        else if (isdigit(*src)) {                       // TODO: leading '.' ?
            point = NULL;
        
            /* 0x1234ABCD hexadecimal numbers */
            if (*src == '0' && *(src + 1) == 'x') {
                hemp_todo("hexadecimal numbers");
            }

            /* consume integral part */
            do { ++src; } while (isdigit(*src));

            /* look for decimal point and consume fractional part but only if
             * the next character is numeric, so we can support 123.method */
            if (*src == '.' && isdigit(*(src + 1))) {
                point = src;
                do { src++; } while (isdigit(*src));
            } 
            
            if (*src == 'e') {
                hemp_todo("numbers with exponents");
            }
                
            debug_token("NUMBER", from, src-from);
            hemp_elements_append(
                tmpl->elements, HempElementNumber,
                from, pos, src - from
            );
        }
        else if (isalpha(*src)) {
            while (isalnum(*++src)) {                     // TODO: keyword check
                // advance
            }
            debug_token("WORD", from, src-from);
            hemp_elements_append(
                tmpl->elements, HempElementNumber,
                from, pos, src - from
            );
        }
        // TODO: single quoted strings, double quoted strings
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





