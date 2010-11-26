#include <hemp/viewer.h>


/*--------------------------------------------------------------------------
 * viewer initialisation and cleanup functions 
 *-------------------------------------------------------------------------*/

hemp_viewer_p
hemp_viewer_init(
    hemp_hemp      hemp,
    hemp_string  name
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
    viewer->size   = 0;
    
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


void
hemp_viewer_resize(
    hemp_viewer_p viewer,
    hemp_u16    min_size
) {
    hemp_debug_call("hemp_viewer_resize(%s, %d)\n", viewer->name, min_size);
    hemp_u16    old_size = viewer->size;
    hemp_u16    new_size = old_size;
    hemp_memory    buffer;
    hemp_u16    size;

    if (! new_size)
        new_size = HEMP_VIEW_SIZE;

    while (min_size >= new_size) {
        new_size = new_size << 1;
    }

    size   = new_size * sizeof(hemp_view_f);
    buffer = hemp_mem_alloc(size);

    if (! buffer)
        hemp_mem_fail("viewer view buffer");

    memset(buffer, 0, size);
    hemp_debug_mem("allocated and cleared %d bytes for %d views\n", size, new_size);
        
    if (old_size) {
        size = old_size * sizeof(hemp_view_f);
        hemp_mem_copy(viewer->view, buffer, size);
        hemp_debug_mem("copied %d bytes from previous %d views\n", size, old_size);
        hemp_mem_free(viewer->view);
    }
    
    viewer->view = buffer;
    viewer->size = new_size;
}


hemp_bool
hemp_viewer_add_view(
    hemp_viewer_p viewer,
    hemp_string    name,
    hemp_view_f   view
) {
    hemp_debug_call("hemp_viewer_add_view(%s:%s => %p)\n", viewer->name, name, view);
    hemp_u16 id = hemp_namespace_id(name);
    hemp_debug("%s ID: %d\n", name, id);
    
    if (id >= viewer->size)
        hemp_viewer_resize(viewer, id);

    if (viewer->view[id]) {
        hemp_debug("already got viewer for %s\n", name);
    }
    else {
        hemp_debug("installed view #%d\n", id);
        viewer->view[id] = view;
    }

    return HEMP_TRUE;
}


HEMP_VIEW_FUNC(hemp_viewer_element) {
    hemp_debug("hemp_viewer_element(%s, %s)\n", viewer->name, element->type->name);
    hemp_namespace_p namespace = element->type->namespace;
    hemp_view_f view;
    
    while (namespace) {
        hemp_debug(
            "TODO: look for viewer for namespace #%d: %s\n", 
            namespace->id, namespace->name
        );
        if (namespace->id < viewer->size 
        && (view = viewer->view[namespace->id])) {
            hemp_debug("** YAY!  Found handler\n");
        }
        namespace = namespace->parent;
    }

    return HempMissing;
}


