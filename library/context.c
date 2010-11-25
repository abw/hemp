#include <hemp/context.h>

// local syntactic sugar macros for lazy programmers

//#define hemp_context_init_pool(type)            \
//    context->##type##_pool = hemp_pool_init(      \
//        sizeof(struct hemp_##type##_s),         \
//        HEMP_TMP_POOL_SIZE                      \
//    );                                          \
//    context->##type##_pool->cleaner = &hemp_context_##type##_pool_cleaner;
    

/*--------------------------------------------------------------------------
 * 
 *--------------------------------------------------------------------------*/


hemp_context_p
hemp_context_init(
    hemp_p hemp
) {
    hemp_context_p context = (hemp_context_p) hemp_mem_alloc(
        sizeof(struct hemp_context_s)
    );
    
    if (! context)
        hemp_mem_fail("context");

    context->hemp      = hemp;
    context->frame     = NULL;
    context->vars      = hemp_hash_init();
    context->text_pool = hemp_pool_new(
        sizeof(struct hemp_text_s),
        HEMP_TMP_POOL_SIZE,
        &hemp_context_text_pool_cleaner
    );
    context->list_pool = hemp_pool_new(
        sizeof(struct hemp_list_s), 
        HEMP_TMP_POOL_SIZE,
        &hemp_context_list_pool_cleaner
    );
    context->code_pool = hemp_pool_new(
        sizeof(struct hemp_code_s),
        HEMP_TMP_POOL_SIZE,
        &hemp_context_code_pool_cleaner
    );

    return context;
}


void
hemp_context_free(
    hemp_context_p context
) {
//  hemp_debug("cleaning context at %p\n", context);

    while (context->frame) {
        hemp_context_leave(context);
    }

    hemp_pool_free(context->text_pool);
    hemp_pool_free(context->list_pool);
    hemp_pool_free(context->code_pool);

    hemp_hash_free(context->vars);
    hemp_mem_free(context);
}


HEMP_INLINE hemp_frame_p
hemp_context_enter(
    hemp_context_p context,
    hemp_element_p element
) {
    hemp_debug_call("hemp_context_enter(%p, %p)\n", context, element);

    /* Frame points at master context, parent frame and current element.
     * Also has local vars hash which are chained to current context vars
     * and then installed in the context as the new master set
     */
    hemp_frame_p frame  = hemp_frame_new();
    frame->element      = element;
    frame->context      = context;
    frame->parent       = context->frame;
    frame->vars->parent = context->vars;
    context->frame      = frame;
    context->vars       = frame->vars;

//  hemp_debug_msg(
//      "entered new frame for %s: %p  vars: %p\n", 
//      element ? element->type->name : "NULL element", 
//      frame, context->vars
//  );

    return frame;
}


HEMP_INLINE hemp_frame_p
hemp_context_frame(
    hemp_context_p context
) {
    if (! context->frame)
        hemp_fatal("No current frame in context");

    return context->frame;
}


HEMP_INLINE hemp_element_p
hemp_context_leave(
    hemp_context_p context
) {
    hemp_debug_call("hemp_context_leave(%p, %p)\n", context, context->frame);
    hemp_frame_p    frame   = hemp_context_frame(context);
    hemp_element_p  element = frame->element;

    /* restore pointer to parent frame and parent frame's vars */
    context->frame = frame->parent;
    context->vars  = frame->vars->parent;

//  hemp_debug_msg(
//      "left frame for %s: %p  vars: %p\n", 
//      element ? element->type->name : "NULL element", 
//      frame, context->vars
//  );

    hemp_frame_free(frame);
    
    return element;
}


hemp_text_p
hemp_context_tmp_text(
    hemp_context_p context
) {
    hemp_text_p text = (hemp_text_p) hemp_pool_take(context->text_pool);
//    hemp_debug("*** got new text pointer at %p\n", text);
    hemp_text_init(text);
    return text;
}


hemp_text_p
hemp_context_tmp_text_size(
    hemp_context_p context,
    hemp_size_t    size
) {
    hemp_text_p text = (hemp_text_p) hemp_pool_take(context->text_pool);
    return hemp_text_init_size(text, size);
}

hemp_list_p
hemp_context_tmp_list(
    hemp_context_p context
) {
    hemp_list_p list = (hemp_list_p) hemp_pool_take(context->list_pool);
//  hemp_debug("*** got new list pointer at %p\n", list);
    return hemp_list_init(list);
}

hemp_code_p
hemp_context_tmp_code(
    hemp_context_p context
) {
    hemp_code_p code = (hemp_code_p) hemp_pool_take(context->code_pool);
    hemp_debug_msg("*** got new code pointer at %p\n", code);
    return hemp_code_init(code);
}


hemp_bool_t
hemp_context_text_pool_cleaner(
    hemp_mem_p item
) {
//    hemp_debug("hemp_context_text_pool_cleaner(%p)\n", item);
    hemp_text_release((hemp_text_p) item);
    return HEMP_TRUE;
}

hemp_bool_t
hemp_context_list_pool_cleaner(
    hemp_mem_p item
) {
//    hemp_debug_call("hemp_context_list_pool_cleaner(%p)\n", item);
    hemp_list_release((hemp_list_p) item);
    return HEMP_TRUE;
}


hemp_bool_t
hemp_context_code_pool_cleaner(
    hemp_mem_p item
) {
    hemp_debug_msg("hemp_context_code_pool_cleaner(%p)\n", item);
    hemp_code_release((hemp_code_p) item);
    return HEMP_TRUE;
}

