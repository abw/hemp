#include <hemp/viewer.h>


/*--------------------------------------------------------------------------
 * viewer initialisation and cleanup functions 
 *-------------------------------------------------------------------------*/

hemp_viewer_p
hemp_viewer_init(
    hemp_p      hemp,
    hemp_str_p  name
) {
    hemp_viewer_p viewer = (hemp_viewer_p) hemp_mem_alloc(
        sizeof(struct hemp_viewer_s)
    );

    if (! viewer)
        hemp_mem_fail("viewer");

    viewer->hemp   = hemp;
    viewer->name   = hemp_string_clone(name, "viewer name");
    viewer->parent = NULL;
    viewer->view   = NULL;
    
    /* TODO: pre-allocate buffers */

    return viewer;
}


void
hemp_viewer_free(
    hemp_viewer_p viewer
) {
    if (viewer->view)
        hemp_mem_free(viewer->view);
    
    hemp_mem_free(viewer->name);
    hemp_mem_free(viewer);
}


HEMP_VIEW_FUNC(hemp_viewer_element) {
    hemp_debug("hemp_viewer_element(%s, %s)\n", viewer->name, element->type->name);
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


