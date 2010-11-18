#include <hemp/frame.h>


HEMP_INLINE hemp_frame_p
hemp_frame_init(
    hemp_frame_p frame
) {
    hemp_debug_call("hemp_frame_init(%p)\n", frame);
    hemp_frame_allocate(frame);

    frame->context = NULL;
    frame->parent  = NULL;
    frame->element = NULL;
    frame->args    = hemp_list_new();
    frame->vars    = hemp_hash_new();

    return frame;
}


HEMP_INLINE void
hemp_frame_release(
    hemp_frame_p frame
) {
    hemp_list_free(frame->args);
    hemp_hash_free(frame->vars);
}
    

HEMP_INLINE void
hemp_frame_free(
    hemp_frame_p frame
) {
    hemp_frame_release(frame);
    hemp_mem_free(frame);
}

