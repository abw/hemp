#include <hemp/context.h>


/*--------------------------------------------------------------------------
 * constructor/destructor functions
 *--------------------------------------------------------------------------*/

hemp_context
hemp_context_new(
    hemp_hemp hemp
) {
    hemp_context context;
    HEMP_ALLOCATE(context);

    context->hemp      = hemp;
    context->frame     = NULL;
    context->vars      = hemp_hash_new();

    // TODO: this is all rather wasteful and annoyingly repetitive.
    context->text_pool = hemp_pool_new(
        sizeof(struct hemp_text),
        HEMP_TMP_POOL_SIZE,
        &hemp_context_text_pool_cleaner
    );
    context->list_pool = hemp_pool_new(
        sizeof(struct hemp_list), 
        HEMP_TMP_POOL_SIZE,
        &hemp_context_list_pool_cleaner
    );
    context->hash_pool = hemp_pool_new(
        sizeof(struct hemp_hash), 
        HEMP_TMP_POOL_SIZE,
        &hemp_context_hash_pool_cleaner
    );
    context->code_pool = hemp_pool_new(
        sizeof(struct hemp_code),
        HEMP_TMP_POOL_SIZE,
        &hemp_context_code_pool_cleaner
    );
    context->params_pool = hemp_pool_new(
        sizeof(struct hemp_params),
        HEMP_TMP_POOL_SIZE,
        &hemp_context_params_pool_cleaner
    );

    return context;
}


void
hemp_context_free(
    hemp_context context
) {
//  hemp_debug("cleaning context at %p\n", context);

    while (context->frame) {
        hemp_context_leave(context);
    }

    hemp_pool_free(context->text_pool);
    hemp_pool_free(context->list_pool);
    hemp_pool_free(context->hash_pool);
    hemp_pool_free(context->code_pool);
    hemp_pool_free(context->params_pool);

    hemp_hash_free(context->vars);
    hemp_mem_free(context);
}


HEMP_INLINE hemp_frame
hemp_context_enter(
    hemp_context  context,
    hemp_fragment fragment
) {
    hemp_debug_call("hemp_context_enter(%p, %p)\n", context, fragment);

    /* Frame points at master context, parent frame and current element.
     * Also has local vars hash which are chained to current context vars
     * and then installed in the context as the new master set
     */
    hemp_frame frame    = hemp_frame_new();
    frame->fragment     = fragment;
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


HEMP_INLINE hemp_frame
hemp_context_frame(
    hemp_context context
) {
    if (! context->frame)
        hemp_fatal("No current frame in context");

    return context->frame;
}


HEMP_INLINE hemp_fragment
hemp_context_leave(
    hemp_context context
) {
    hemp_debug_call("hemp_context_leave(%p, %p)\n", context, context->frame);
    hemp_frame    frame    = hemp_context_frame(context);
    hemp_fragment fragment = frame->fragment;

    /* restore pointer to parent frame and parent frame's vars */
    context->frame = frame->parent;
    context->vars  = frame->vars->parent;

//  hemp_debug_msg(
//      "left frame for %s: %p  vars: %p\n", 
//      element ? element->type->name : "NULL element", 
//      frame, context->vars
//  );

    hemp_frame_free(frame);
    
    return fragment;
}


/*--------------------------------------------------------------------------
 * Shoot me now. This is an ugly hack to temporarily patch the named
 * parameter hash (params->nominals) in the current context frame into 
 * the context vars.  This is so that we can evaluate a parameter list as 
 * usual, but capture any side-effects from variables being set.  e.g. For 
 * a call like foo(a=10), we evaluate the (a=10) params which results in the 
 * variable 'a' being set to 10.  Except that it's set in the params->nominal 
 * hash, effectively making it a named parameter instead of a regular variable.
 * We attach the old context->vars hash as the parent of the params hash so 
 * that a) we can still lookup all other runtime variables and b) we can 
 * restore the original vars in hemp_context_blur_params().
 *--------------------------------------------------------------------------*/

HEMP_INLINE void
hemp_context_focus_params(
    hemp_context context
) {
    if (! context->frame)
        return;

    hemp_params params = context->frame->params;
//  hemp_debug_msg("focussing params at %p chained to %p\n", params->nominals, context->vars);
    params->nominals->parent = context->vars;
    context->vars  = params->nominals;
}


HEMP_INLINE void
hemp_context_blur_params(
    hemp_context context
) {
    if (! context->frame)
        return;

    hemp_params params = context->frame->params;
//  hemp_debug_msg("blurring params at %p chained to %p\n", context->vars, context->vars->parent);
    context->vars  = params->nominals->parent;
    params->nominals->parent = NULL;
}


/*--------------------------------------------------------------------------
 * Function to return temporary data items with memory managed by context.
 * TODO: needs cleaning up and generalising
 *--------------------------------------------------------------------------*/

HEMP_INLINE hemp_text
hemp_context_tmp_text(
    hemp_context context
) {
    hemp_text text = (hemp_text) hemp_pool_take(context->text_pool);
//    hemp_debug("*** got new text pointer at %p\n", text);
    hemp_text_init(text);
    return text;
}


HEMP_INLINE hemp_text
hemp_context_tmp_text_size(
    hemp_context context,
    hemp_size    size
) {
    hemp_text text = (hemp_text) hemp_pool_take(context->text_pool);
//  hemp_debug("new tmp text pointer at %p\n", list);
    return hemp_text_init_size(text, size);
}


HEMP_INLINE hemp_list
hemp_context_tmp_list(
    hemp_context context
) {
    hemp_list list = (hemp_list) hemp_pool_take(context->list_pool);
//  hemp_debug("new tmp list pointer at %p\n", list);
    return hemp_list_init(list);
}


HEMP_INLINE hemp_hash
hemp_context_tmp_hash(
    hemp_context context
) {
    hemp_hash hash = (hemp_hash) hemp_pool_take(context->hash_pool);
//  hemp_debug_msg("new tmp hash pointer at %p\n", hash);
    return hemp_hash_init(hash);
}


HEMP_INLINE hemp_code
hemp_context_tmp_code(
    hemp_context context
) {
    hemp_code code = (hemp_code) hemp_pool_take(context->code_pool);
//  hemp_debug_msg("new tmp code pointer at %p\n", code);
    return hemp_code_init(code);
}


HEMP_INLINE hemp_params
hemp_context_tmp_params(
    hemp_context context
) {
    return hemp_params_init(
        (hemp_params) hemp_pool_take(
            context->params_pool
        )
    );
}


hemp_bool
hemp_context_text_pool_cleaner(
    hemp_memory item
) {
//  hemp_debug_msg("hemp_context_text_pool_cleaner(%p)\n", item);
    hemp_text_release((hemp_text) item);
    return HEMP_TRUE;
}

hemp_bool
hemp_context_list_pool_cleaner(
    hemp_memory item
) {
//  hemp_debug_msg("hemp_context_list_pool_cleaner(%p)\n", item);
    hemp_list_release((hemp_list) item);
    return HEMP_TRUE;
}


hemp_bool
hemp_context_hash_pool_cleaner(
    hemp_memory item
) {
//  hemp_debug_msg("hemp_context_hash_pool_cleaner(%p)\n", item);
    hemp_hash_release((hemp_hash) item);
    return HEMP_TRUE;
}


hemp_bool
hemp_context_code_pool_cleaner(
    hemp_memory item
) {
//  hemp_debug_msg("hemp_context_code_pool_cleaner(%p)\n", item);
    hemp_code_release((hemp_code) item);
    return HEMP_TRUE;
}

hemp_bool
hemp_context_params_pool_cleaner(
    hemp_memory item
) {
//  hemp_debug_msg("hemp_context_params_pool_cleaner(%p)\n", item);
    hemp_params_release((hemp_params) item);
    return HEMP_TRUE;
}

