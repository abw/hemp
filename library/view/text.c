#include <hemp/view.h>


HEMP_VIEWER_FUNC(hemp_view_text_init) {
    hemp_debug("hemp_view_text_init(%p, %s)\n", hemp, name);

    hemp_view_p view = hemp_view_init(
        hemp, name
    );

    return view;
}


