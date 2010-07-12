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
    long            integer;
    double          floater;
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
            integer = strtol(src, &src, 0);
            
            if (errno) {
                switch (errno) {
                    case EINVAL:
                        debug_red("invalid number\n");          // TODO: proper handling
                        break;
                    case ERANGE:
                        debug_red("number out of range\n");     // TODO: proper handling
                        break;
                    default:
                        debug_red("unknown error parsing number\n");
                        perror("num");
                        break;
                }
            }
            
            /* look for decimal point and consume fractional part but only if
             * the next character is numeric, so we can support 123.method */
            if (
                (*src == '.' && isdigit(*(src + 1)))
             || (*src == 'e' || *src == 'E')
            )  {
                debug_yellow("found decimal point\n");
                floater = strtod(from, &src);
                if (from == src) {
                    debug_red("invalid floating point number"); // TODO
                }
                else {
                    debug_token("FLOAT", from, src-from);
                    debug_yellow("[VALUE: %lf]\n", floater);
                }
            }
            else {
                debug_token("INTEGER", from, src-from);
                debug_yellow("[VALUE: %ld]", integer);
            }
                
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





