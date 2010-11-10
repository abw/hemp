#include <hemp/view.h>


/*--------------------------------------------------------------------------
 * view initialisation and cleanup functions 
 *-------------------------------------------------------------------------*/

hemp_view_p
hemp_view_init(
    hemp_p      hemp,
    hemp_str_p  name
) {
    hemp_view_p view = (hemp_view_p) hemp_mem_alloc(
        sizeof(struct hemp_view_s)
    );

    if (! view)
        hemp_mem_fail("view");

    view->hemp   = hemp;
    view->name   = hemp_string_clone(name, "view name");
    view->parent = NULL;
    view->viewer = NULL;
    
    /* TODO: pre-allocate buffers */

    return view;
}


void
hemp_view_free(
    hemp_view_p view
) {
    if (view->viewer)
        hemp_mem_free(view->viewer);
    
    hemp_mem_free(view->name);
    hemp_mem_free(view);
}


HEMP_VIEW_FUNC(hemp_view_element) {
    hemp_debug("hemp_view_element(%s, %s)\n", view->name, element->type->name);
    hemp_namespace_p namespace = element->type->namespace;
    
    while (namespace) {
        hemp_debug(
            "TODO: look for viewer for namespace #%d: %s\n", 
            namespace->id, namespace->name
        );
        namespace = namespace->parent;
    }

    return HempMissing;
}


HEMP_VIEW_FUNC(hemp_view_hemp_block_as_text) {
    hemp_debug_call("hemp_view_hemp_block_as_text()\n");
    hemp_list_p     exprs = element->args.block.exprs;
    hemp_element_p  expr;
    hemp_size_t     n;

    hemp_text_p text;
    hemp_prepare_output(output, text, 0);
    
    for (n = 0; n < exprs->length; n++) {
        expr = (hemp_element_p) hemp_val_ptr( hemp_list_item(exprs, n) );
        hemp_view_element(view, expr, context, output);
    }

//  hemp_debug("returning block text (%d bytes): %s\n", text->length, text->string);

    return output;
}
