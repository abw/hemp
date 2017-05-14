#include <hemp/context.h>


/*--------------------------------------------------------------------------
 * constructor/destructor functions
 *--------------------------------------------------------------------------*/

HempContext
hemp_context_new(
    Hemp hemp
) {
    HempContext context;
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
    HempContext context
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


HempFrame
hemp_context_enter(
    HempContext  context,
    HempFragment fragment
) {
    hemp_debug_call("hemp_context_enter(%p, %p)\n", context, fragment);

    /* Frame points at master context, parent frame and current element.
     * Also has local vars hash which are chained to current context vars
     * and then installed in the context as the new master set
     */
    HempFrame frame    = hemp_frame_new();
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


HempFrame
hemp_context_frame(
    HempContext context
) {
    if (! context->frame)
        hemp_fatal("No current frame in context");

    return context->frame;
}


HempFragment
hemp_context_leave(
    HempContext context
) {
    hemp_debug_call("hemp_context_leave(%p, %p)\n", context, context->frame);
    HempFrame    frame    = hemp_context_frame(context);
    HempFragment fragment = frame->fragment;

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

HempHash
hemp_context_with(
    HempContext    context,
    HempHash       vars
) {
    HempHash old = context->vars;
    vars->parent  = old;
    context->vars = vars;
    return old;
}


HempHash
hemp_context_just(
    HempContext    context,
    HempHash       vars
) {
    HempHash old = context->vars;
    context->vars = vars;
    vars->parent  = NULL;
    return old;
}


HempHash
hemp_context_without(
    HempContext    context,
    HempHash       vars
) {
    HempHash old = context->vars;

    if (! vars)
        vars = old->parent;

    context->vars = vars;
    old->parent   = NULL;

    return old;
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

void
hemp_context_focus_params(
    HempContext context
) {
    if (! context->frame)
        return;

    HempParams params = context->frame->params;
//  hemp_debug_msg("focussing params at %p chained to %p\n", params->nominals, context->vars);
    params->nominals->parent = context->vars;
    context->vars  = params->nominals;
}


void
hemp_context_blur_params(
    HempContext context
) {
    if (! context->frame)
        return;

    HempParams params = context->frame->params;
//  hemp_debug_msg("blurring params at %p chained to %p\n", context->vars, context->vars->parent);
    context->vars  = params->nominals->parent;
    params->nominals->parent = NULL;
}


/*--------------------------------------------------------------------------
 * Function to return temporary data items with memory managed by context.
 * TODO: needs cleaning up and generalising
 *--------------------------------------------------------------------------*/

HempText
hemp_context_tmp_text(
    HempContext context
) {
    HempText text = (HempText) hemp_pool_take(context->text_pool);
//    hemp_debug("*** got new text pointer at %p\n", text);
    hemp_text_init(text);
    return text;
}

HempText
hemp_context_tmp_text_size(
    HempContext context,
    HempSize    size
) {
    HempText text = (HempText) hemp_pool_take(context->text_pool);
//  hemp_debug("new tmp text pointer at %p\n", list);
    return hemp_text_init_size(text, size);
}

HempList
hemp_context_tmp_list(
    HempContext context
) {
    HempList list = (HempList) hemp_pool_take(context->list_pool);
//  hemp_debug("new tmp list pointer at %p\n", list);
    return hemp_list_init(list);
}

HempHash
hemp_context_tmp_hash(
    HempContext context
) {
    HempHash hash = (HempHash) hemp_pool_take(context->hash_pool);
//  hemp_debug_msg("new tmp hash pointer at %p\n", hash);
    return hemp_hash_init(hash);
}

HempCode
hemp_context_tmp_code(
    HempContext context
) {
    HempCode code = (HempCode) hemp_pool_take(context->code_pool);
//  hemp_debug_msg("new tmp code pointer at %p\n", code);
    return hemp_code_init(code);
}

HempParams
hemp_context_tmp_params(
    HempContext context
) {
    return hemp_params_init(
        (HempParams) hemp_pool_take(
            context->params_pool
        )
    );
}


HempBool
hemp_context_text_pool_cleaner(
    HempMemory item
) {
//  hemp_debug_msg("hemp_context_text_pool_cleaner(%p)\n", item);
    hemp_text_release((HempText) item);
    return HEMP_TRUE;
}

HempBool
hemp_context_list_pool_cleaner(
    HempMemory item
) {
//  hemp_debug_msg("hemp_context_list_pool_cleaner(%p)\n", item);
    hemp_list_release((HempList) item);
    return HEMP_TRUE;
}


HempBool
hemp_context_hash_pool_cleaner(
    HempMemory item
) {
//  hemp_debug_msg("hemp_context_hash_pool_cleaner(%p)\n", item);
    hemp_hash_release((HempHash) item);
    return HEMP_TRUE;
}


HempBool
hemp_context_code_pool_cleaner(
    HempMemory item
) {
//  hemp_debug_msg("hemp_context_code_pool_cleaner(%p)\n", item);
    hemp_code_release((HempCode) item);
    return HEMP_TRUE;
}

HempBool
hemp_context_params_pool_cleaner(
    HempMemory item
) {
//  hemp_debug_msg("hemp_context_params_pool_cleaner(%p)\n", item);
    hemp_params_release((HempParams) item);
    return HEMP_TRUE;
}
