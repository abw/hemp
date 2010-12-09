#include "hemp/stack.h"


HEMP_INLINE hemp_stack
hemp_stack_init(
    hemp_stack      stack,
    hemp_size       capacity
) {
    HEMP_INSTANCE(stack);

    stack->depth    = 0;
    stack->capacity = capacity;
    stack->items    = hemp_mem_alloc(capacity * sizeof(hemp_memory));
    
    if (! stack->items)
        hemp_mem_fail("stack items");

    return stack;
}


HEMP_INLINE void
hemp_stack_grow(
    hemp_stack  stack
) {
    hemp_size   capacity = stack->capacity * 2;

    if (! capacity)
        capacity = HEMP_STACK_SIZE;

    stack->items = hemp_mem_resize(
        stack->items, 
        capacity * sizeof(hemp_memory)
    );
    if (! stack->items)
        hemp_mem_fail("stack items");

    stack->capacity = capacity;

//  hemp_debug_mem("resized stack to %d\n", stack->capacity);
}


HEMP_INLINE void
hemp_stack_push(
    hemp_stack  stack,
    hemp_memory item
) {
    hemp_debug_call("hemp_stack_push(%p, %p)\n", stack, item);

    if (stack->depth == stack->capacity)
        hemp_stack_grow(stack);

    stack->items[stack->depth++] = item;
}


HEMP_INLINE hemp_memory
hemp_stack_pop(
    hemp_stack  stack
) {
    hemp_debug_call("hemp_stack_pop(%p)\n", stack);

    if (stack->depth > 0)
        return stack->items[--stack->depth];
    else
        hemp_fatal("Stack underflow! (TODO: dynamically expand stack)\n");
    
    return NULL;
}


HEMP_INLINE void
hemp_stack_free(
    hemp_stack stack
) {
    hemp_mem_free(stack->items);
    hemp_mem_free(stack);
}

