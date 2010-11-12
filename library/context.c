#include <hemp/context.h>


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
    context->vars      = hemp_hash_init();
    context->list_pool = hemp_pool_init(
        sizeof(struct hemp_list_s), 8                 // TODO: soft-code this value
    );
    context->text_pool = hemp_pool_init(
        sizeof(struct hemp_text_s), 8                 // TODO: soft-code this value
    );

    return context;
}


void
hemp_context_free(
    hemp_context_p context
) {
    // TODO: worry about cleanup
//    hemp_debug("cleaning context at %p\n", context);

    hemp_pool_each(context->text_pool, &hemp_context_text_pool_cleaner);
    hemp_pool_free(context->text_pool);

    hemp_pool_each(context->list_pool, &hemp_context_list_pool_cleaner);
    hemp_pool_free(context->list_pool);

    hemp_hash_free(context->vars);
    hemp_mem_free(context);
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
    hemp_text_init_size(text, size);
    return text;
}

hemp_list_p
hemp_context_tmp_list(
    hemp_context_p context
) {
    hemp_list_p list = (hemp_list_p) hemp_pool_take(context->list_pool);
//  hemp_debug("*** got new list pointer at %p\n", list);
    hemp_list_init(list);
    return list;
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

