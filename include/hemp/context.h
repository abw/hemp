#ifndef HEMP_CONTEXT_H
#define HEMP_CONTEXT_H

#include <hemp/core.h>
#include <hemp/pool.h>
#include <hemp/fragment.h>
#include <hemp/frame.h>
#include <hemp/type/hash.h>
#include <hemp/type/code.h>
#include <hemp/type/params.h>


/*--------------------------------------------------------------------------
 * type definitions
 *--------------------------------------------------------------------------*/

struct hemp_context {
    Hemp       hemp;
    HempHash       vars;
    HempFrame      frame;
    HempPool       text_pool;          /* tmp hack */
    HempPool       list_pool;          /* tmp hack */
    HempPool       hash_pool;          /* tmp hack */
    HempPool       code_pool;          /* tmp hack */
    HempPool       params_pool;        /* tmp hack */
};


/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

HempContext
hemp_context_new(
    Hemp       hemp
);

void
hemp_context_free(
    HempContext    context
);

HEMP_INLINE HempText
hemp_context_tmp_text(
    HempContext    context
);

HEMP_INLINE HempText
hemp_context_tmp_text_size(
    HempContext    context,
    HempSize       size
);

HEMP_INLINE HempList
hemp_context_tmp_list(
    HempContext    context
);

HEMP_INLINE HempHash
hemp_context_tmp_hash(
    HempContext    context
);

HEMP_INLINE HempCode
hemp_context_tmp_code(
    HempContext    context
);

HEMP_INLINE HempParams
hemp_context_tmp_params(
    HempContext    context
);

HempBool
hemp_context_text_pool_cleaner(
    HempMemory     item
);

HempBool
hemp_context_list_pool_cleaner(
    HempMemory     item
);

HempBool
hemp_context_hash_pool_cleaner(
    HempMemory     item
);

HempBool
hemp_context_code_pool_cleaner(
    HempMemory     item
);

HempBool
hemp_context_params_pool_cleaner(
    HempMemory     item
);

HEMP_INLINE HempFrame
hemp_context_frame(
    HempContext    context
);

HEMP_INLINE HempFrame
hemp_context_enter(
    HempContext    context,
    HempFragment   fragment
);

HEMP_INLINE HempFragment
hemp_context_leave(
    HempContext    context
);

HEMP_INLINE void
hemp_context_focus_params(
    HempContext    context
);

HEMP_INLINE void
hemp_context_blur_params(
    HempContext    context
);

HempHash
hemp_context_with(
    HempContext    context,
    HempHash       vars
);

HempHash
hemp_context_just(
    HempContext    context,
    HempHash       vars
);

HempHash
hemp_context_without(
    HempContext    context,
    HempHash       vars
);


/*--------------------------------------------------------------------------
 * macros
 *--------------------------------------------------------------------------*/

#define hemp_context_set(c, k, v)           \
    hemp_hash_store(c->vars, k, v)

#define hemp_context_set_number(c, k, n)    \
    hemp_hash_store_number(c->vars, k, n)

#define hemp_context_set_integer(c, k, i)   \
    hemp_hash_store_integer(c->vars, k, i)

#define hemp_context_set_pointer(c, k, p)   \
    hemp_hash_store_pointer(c->vars, k, p)

#define hemp_context_set_string(c, k, s)    \
    hemp_hash_store_string(c->vars, k, s)

#define hemp_context_set_text(c, k, t)      \
    hemp_hash_store_text(c->vars, k, t)

#define hemp_context_set_list(c, k, l)      \
    hemp_hash_store_list(c->vars, k, l)

#define hemp_context_set_hash(c, k, h)      \
    hemp_hash_store_hash(c->vars, k, h)

#define hemp_context_get(c, k)              \
    hemp_hash_fetch(c->vars, k)

#define hemp_context_get_number(c, k)       \
    hemp_hash_fetch_number(c->vars, k)

#define hemp_context_get_integer(c, k)      \
    hemp_hash_fetch_integer(c->vars, k)

#define hemp_context_get_pointer(c, k)      \
    hemp_hash_fetch_pointer(c->vars, k)

#define hemp_context_get_string(c, k)       \
    hemp_hash_fetch_string(c->vars, k)


#endif /* HEMP_CONTEXT_H */
