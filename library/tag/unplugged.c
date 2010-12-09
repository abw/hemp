#include <hemp.h>

HEMP_TAG(hemp_tag_unplugged);
//void hemp_tag_unplugged_scan(HEMP_TAG_SCAN_ARGS);


/*--------------------------------------------------------------------------
 * Tag constructor
 *--------------------------------------------------------------------------*/

HEMP_TAG(hemp_tag_unplugged) {
    hemp_tag tag = hemp_tag_new(
        type, name, start, end, grammar
    );
    tag->style  = HEMP_UNPLUGGED_TAG;
//    tag->scan   = &hemp_tag_unplugged_scan;
    return tag;
}


/*--------------------------------------------------------------------------
 * Tag scanner
 *--------------------------------------------------------------------------*/
