#include <hemp.h>

HEMP_TAG(hemp_tag_unplugged);


/*--------------------------------------------------------------------------
 * Tag constructor
 *--------------------------------------------------------------------------*/

HEMP_TAG(hemp_tag_unplugged) {
    HempTag tag = hemp_tag_new(
        type, name, start, end, grammar
    );
    tag->style  = HEMP_UNPLUGGED_TAG;
//    tag->scan   = &hemp_tag_unplugged_scan;
    return tag;
}


/*--------------------------------------------------------------------------
 * Tag scanner
 *--------------------------------------------------------------------------*/
