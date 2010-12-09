#include <hemp/tag.h>
#include <hemp/template.h>


HEMP_TAG(hemp_tag_comment) {
//  hemp_debug_msg(
//      "hemp_tag_comment(%p, %s, %s, %s, %s, %s)\n",
//      hemp, type, name, start, end ? end : "", 
//      grammar ? grammar->name : "no grammar"
//  );
    hemp_tag tag = hemp_tag_inline(
        hemp, type, name, start, end, grammar
    );
    tag->scanner = &hemp_tag_comment_scanner;
    return tag;
}


HEMP_SCANNER(hemp_tag_comment_scanner) {
    hemp_tag    tag = (hemp_tag) self;
    hemp_string end = strstr(template->scanptr, tag->end);
    
    if (! end)
        hemp_fatal("missing end of comment tag");

    end += strlen(tag->end);

    hemp_template_scanned_to(
        template, HempElementComment, end
    );

    return HEMP_TRUE;
}

