#ifndef HEMP_STACK_H
#define HEMP_STACK_H

#include <hemp/memory.h>


/*--------------------------------------------------------------------------
 * type definitions
 *--------------------------------------------------------------------------*/

struct hemp_stack {
    HempMemory    * items;          /* memory to store n pointers           */
    HempSize        capacity;       /* allocated storage capacity           */
    HempSize        depth;          /* number of records in use             */
};


/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

HEMP_INLINE HempStack
hemp_stack_init(
    HempStack       stack,
    HempSize        capacity
);

HEMP_INLINE void
hemp_stack_push(
    HempStack       stack,
    HempMemory      item
);

HEMP_INLINE HempMemory
hemp_stack_pop(
    HempStack       stack
);

void 
hemp_stack_free(
    HempStack       stack
);

void
hemp_stack_each(
    HempStack       stack,
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

