#include <hemp/tag.h>

hemp_tag
hemp_tag_new(
    hemp_string     type,
    hemp_string     name,
    hemp_string     start,
    hemp_string     end,
    hemp_grammar    grammar
) {
    hemp_tag tag;
    HEMP_ALLOCATE(tag);
    
    /* Some code assumes that all tags always have a start tag, but that's
     * no longer true now that we have unplugged tags.  This'll do for now
     * but should be cleaned up when this code is next refactored.
     */
    if (! start)
        start = HEMP_STR_BLANK;


    tag->tagset  = NULL;
    tag->style   = HEMP_INLINE_TAG;                     // ymmv
    tag->grammar = grammar;
    tag->type    = hemp_string_clone(type,  "tag type");
    tag->name    = hemp_string_clone(name,  "tag name");
    tag->start   = hemp_string_clone(start, "tag start");
    tag->end     = end 
                 ? hemp_string_clone(end,   "tag end") 
                 : NULL;

    // TODO: put some sensible defaults in here
    tag->scan    = NULL;
    tag->scanner = NULL;
    tag->to_eol  = &hemp_tag_to_eol;
    return tag;
}


hemp_tag
hemp_tag_copy(
    hemp_tag tag
) {
    hemp_tag copy = hemp_tag_new(
        tag->type,
        tag->name,
        tag->start,
        tag->end,
        tag->grammar
    );
    copy->style   = tag->style;
    copy->scan    = tag->scan;
    copy->to_eol  = tag->to_eol;
    return copy;
}


void
hemp_tag_set_name(
    hemp_tag    tag,
    hemp_string name
) {
    hemp_mem_free(tag->name);
    tag->name = hemp_string_clone(name, "tag name");
}


void
hemp_tag_set_start(
    hemp_tag tag,
    hemp_string start
) {
    hemp_mem_free(tag->start);
    tag->start = hemp_string_clone(start, "tag start");
}


void
hemp_tag_set_end(
    hemp_tag tag,
    hemp_string end
) {
    if (tag->end)
        hemp_mem_free(tag->end);

    if (end) {
        tag->end = hemp_string_clone(end, "tag end");
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
    hemp_mem_free(tag->type);
    hemp_mem_free(tag->name);
    hemp_mem_free(tag->start);

    if (tag->end)
        hemp_mem_free(tag->end);
    
    hemp_mem_free(tag);
}


hemp_string
hemp_tag_to_eol(
    HEMP_TAG_SKIP_ARGS
) {
    while ( *src
        &&  hemp_not_newline(src) 
    ) {
        src++;
    }

    return src;
}


HEMP_AUTOLOAD(hemp_tag_autoload) {
    return hemp_use_module(factory->hemp, "tag", name)
        ? HEMP_TRUE
        : HEMP_FALSE;
}

