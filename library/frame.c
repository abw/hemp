#include <hemp/frame.h>


HEMP_INLINE hemp_frame
hemp_frame_init(
    hemp_frame frame
) {
    hemp_debug_call("hemp_frame_init(%p)\n", frame);
    HEMP_ALLOCATE(frame);

    frame->context  = NULL;
    frame->parent   = NULL;
    frame->fragment = NULL;
    frame->params   = hemp_params_new();
//  frame->vars     = frame->params->nominals;
    frame->vars     = hemp_hash_new();

    return frame;
}


HEMP_INLINE void
hemp_frame_release(
    hemp_frame frame
) {
    hemp_params_free(frame->params);
    hemp_hash_free(frame->vars);
}


HEMP_INLINE void
hemp_frame_free(
    hemp_frame frame
) {
    hemp_frame_release(frame);
    hemp_mem_free(frame);
}

