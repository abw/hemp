#include <hemp/scanner.h>


/*--------------------------------------------------------------------------
 * scan_pos is a temporary hack to store scan position in an exception
 *--------------------------------------------------------------------------*/

hemp_scan_pos
hemp_scan_pos_init(HEMP_SCAN_ARGS) {
    hemp_scan_pos scan_pos = (hemp_scan_pos) hemp_mem_alloc(
        sizeof(struct hemp_scan_pos)
    );

    if (! scan_pos)
        hemp_mem_fail("scanner position");
    
    scan_pos->tmpl    = tmpl;
    scan_pos->tag     = tag;
    scan_pos->start   = start;
    scan_pos->pos     = pos;
    scan_pos->current = *srcptr;
    scan_pos->symbol  = symbol;

    return scan_pos;
}


void
hemp_scan_pos_free(
    hemp_scan_pos scan_pos
) {
    hemp_mem_free(scan_pos);
}




/*
hemp_bool
hemp_scan_unplugged(
    hemp_template tmpl
) {
    hemp_elements   elements = tmpl->elements;
    hemp_tagset     tagset   = tmpl->tagset;
    hemp_tag        tag      = tagset->unplugged_tag;
    hemp_string     text     = hemp_source_read(tmpl->source),
                    src      = text,
                    from     = text;
    hemp_pos        pos      = 0;

    if (! tag)
        hemp_fatal("No unplugged tag is defined in tagset\n");

    tag->scan(tmpl, tag, src, pos, &src);
    pos += src - from;
    from = src;

    while (*src)
        src++;

    if (src > from) {
        hemp_elements_append(
            elements, tagset->text_symbol,
            from, pos, src - from
        );
        pos += src - from;
    }
    
    hemp_elements_eof(elements, pos);

    return HEMP_TRUE;
}

*/


