#include <hemp/viewer.h>

/* This is still something of an experiment in progress */


HEMP_VIEW_FUNC(hemp_viewer_text_view_hemp_block);


HEMP_VIEWER_FUNC(hemp_viewer_text_init) {
    hemp_debug("hemp_viewer_text_init(%p, %s)\n", hemp, name);

    hemp_viewer_p viewer = hemp_viewer_init(
        hemp, name
    );
    
    HEMP_VIEW("hemp.block", &hemp_viewer_text_view_hemp_block);
    HEMP_VIEW("hemp.number.minus", &hemp_viewer_text_view_hemp_block);

    return viewer;
}


HEMP_VIEW_FUNC(hemp_viewer_text_view_hemp_block) {
    hemp_debug_call("hemp_viewer_text_view_hemp_block()\n");
    hemp_list_p     exprs = element->args.block.exprs;
    hemp_element_p  expr;
    hemp_size_t     n;

    hemp_text_p text;
    hemp_prepare_output(output, text, 0);
    
    for (n = 0; n < exprs->length; n++) {
        expr = (hemp_element_p) hemp_val_ptr( hemp_list_item(exprs, n) );
        hemp_viewer_element(viewer, expr, context, output);
    }

//  hemp_debug("returning block text (%d bytes): %s\n", text->length, text->string);

    return output;
}
