#include <hemp/tag.h>


hemp_tag
hemp_tag_init(
    hemp_string       name,
    hemp_tag_style style,
    hemp_string       start,
    hemp_string       end,
    hemp_tag_scan_f  scan,
    hemp_grammar   grammar
) {
    hemp_tag tag = (hemp_tag) hemp_mem_alloc(
        sizeof(struct hemp_tag)
    );

    if (! tag)
        hemp_mem_fail("tag");

    tag->style   = style;
    tag->name    = hemp_string_copy(name);
    tag->start   = hemp_string_copy(start);
    tag->end     = end ? hemp_string_copy(end) : NULL;
    tag->scan    = scan;
    tag->grammar = grammar;

    if (! tag->name || ! tag->start || (end && ! tag->end))
        hemp_mem_fail("tag");

    /* feck... this is going to need a little re-organisation... but for
     * now I'll just leave this here...
     */
    switch (style) {
        case HEMP_INLINE_TAG:
            tag->to_end_of_line = &hemp_inline_tag_to_end_of_line;
            break;

        case HEMP_OUTLINE_TAG:
            tag->to_end_of_line = &hemp_outline_tag_to_end_of_line;
            break;
    }

    return tag;
}


hemp_tag
hemp_tag_copy(
    hemp_tag tag
) {
    return hemp_tag_init(
        tag->name,
        tag->style,
        tag->start,
        tag->end,
        tag->scan,
        tag->grammar
    );
}


void
hemp_tag_set_name(
    hemp_tag tag,
    hemp_string name
) {
    hemp_mem_free(tag->name);

    tag->name = hemp_string_copy(name);

    if (! tag->name)
        hemp_mem_fail("tag name");
}


void
hemp_tag_set_start(
    hemp_tag tag,
    hemp_string start
) {
    hemp_mem_free(tag->start);

    tag->start = hemp_string_copy(start);

    if (! tag->start)
        hemp_mem_fail("tag start");
}


void
hemp_tag_set_end(
    hemp_tag tag,
    hemp_string end
) {
    if (tag->end)
        hemp_mem_free(tag->end);

    if (end) {
        tag->end = hemp_string_copy(end);

        if (! tag->end)
            hemp_mem_fail("tag end");
    }
    else {
        /* end can be set NULL to indicate no explicit end tag */
        tag->end = NULL;
    }
}


void
hemp_tag_set_start_end(
    hemp_tag tag,
    hemp_string start,
    hemp_string end
) {
    hemp_tag_set_start(tag, start);
    hemp_tag_set_end(tag, end);
}


void
hemp_tag_free(
    hemp_tag tag
) {
    hemp_mem_free(tag->name);
    hemp_mem_free(tag->start);

    if (tag->end)
        hemp_mem_free(tag->end);
    
    hemp_mem_free(tag);
}

