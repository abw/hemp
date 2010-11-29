#include <hemp/tag.h>


HEMP_TAG(hemp_tag_control) {
//  hemp_debug_msg(
//      "hemp_tag_control(%p, %s, %s, %s, %s, %s)\n",
//      hemp, type, name, start, end ? end : "", 
//      grammar ? grammar->name : "no grammar"
//  );
    hemp_tag tag = hemp_tag_inline(
        hemp, type, name, start, end, grammar
    );
    
    tag->scan = &hemp_tag_control_scan;

    return tag;
}


void 
hemp_tag_control_scan(
    HEMP_TAG_SCAN_ARGS
) {
    hemp_todo("control tag");
}

