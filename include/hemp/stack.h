#ifndef HEMP_STACK_H
#define HEMP_STACK_H

#include <hemp/memory.h>


/*--------------------------------------------------------------------------
 * type definitions
 *--------------------------------------------------------------------------*/

struct hemp_stack {
    hemp_memory   * items;          /* memory to store n pointers           */
    hemp_size       capacity;       /* allocated storage capacity           */
    hemp_size       depth;          /* number of records in use             */
};


/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

HEMP_INLINE hemp_stack
hemp_stack_init(
    hemp_stack      stack,
    hemp_size       capacity
);

HEMP_INLINE void
hemp_stack_push(
    hemp_stack      stack,
    hemp_memory     item
);

HEMP_INLINE hemp_memory
hemp_stack_pop(
    hemp_stack      stack
);

void 
hemp_stack_free(
    hemp_stack      stack
);

void
hemp_stack_each(
    hemp_stack      stack,
    hemp_list_iter  func
);


/*--------------------------------------------------------------------------
 * macros
 *--------------------------------------------------------------------------*/

#define hemp_stack_new(capacity)                \
    hemp_stack_init(NULL, capacity)

#define hemp_stack_top(stack) (                 \
    (stack->depth > 0)                          \
        ? stack->items[stack->depth - 1]        \
        : NULL                                  \
)


#endif /* HEMP_STACK_H */

