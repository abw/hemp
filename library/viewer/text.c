#include <hemp/viewer.h>

/* This is still something of an experiment in progress */


HEMP_VIEW(hemp_viewer_text_view_hemp_block);


HEMP_VIEWER(hemp_viewer_text) {
    hemp_debug("hemp_viewer_text(%p, %s)\n", hemp, name);

    hemp_viewer viewer = hemp_viewer_new(
        hemp, name
    );
    
    hemp_viewer_add_view(viewer, "hemp.block", &hemp_viewer_text_view_hemp_block);
    hemp_viewer_add_view(viewer, "hemp.number.minus", &hemp_viewer_text_view_hemp_block);

    return viewer;
}


HEMP_VIEW(hemp_viewer_text_view_hemp_block) {
    hemp_debug_call("hemp_viewer_text_view_hemp_block()\n");
    hemp_list     exprs = hemp_block_exprs_list(fragment);
    hemp_fragment expr;
    hemp_size     n;

    hemp_text text;
    hemp_prepare_text(context, output, text);
    
    for (n = 0; n < exprs->length; n++) {
        expr = (hemp_fragment) hemp_val_ptr( hemp_list_item(exprs, n) );
        hemp_viewer_fragment(viewer, expr, context, output);
    }

//  hemp_debug("returning block text (%d bytes): %s\n", text->length, text->string);

    return output;
}
