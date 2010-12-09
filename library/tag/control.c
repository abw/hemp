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
    
    tag->scanner = &hemp_tag_control_scanner;

    return tag;
}


HEMP_SCANNER(hemp_tag_control_scanner) {
    hemp_todo("control tag");
    return HEMP_FALSE;
}

