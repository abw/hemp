#ifndef HEMP_CONTEXT_H
#define HEMP_CONTEXT_H

#include <hemp/core.h>
#include <hemp/pool.h>
#include <hemp/type/hash.h>


/*--------------------------------------------------------------------------
 * data structures
 *--------------------------------------------------------------------------*/

struct hemp_context_s {
    hemp_p          hemp;
    hemp_hash_p     vars;
/*  hemp_value_t    variables; */
    hemp_pool_p     list_pool;          /* tmp hack */
    hemp_pool_p     text_pool;          /* tmp hack */
};


/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

hemp_context_p  hemp_context_init(hemp_p);
void            hemp_context_free(hemp_context_p);
hemp_list_p     hemp_context_tmp_list(hemp_context_p);
hemp_text_p     hemp_context_tmp_text(hemp_context_p);
hemp_text_p     hemp_context_tmp_text_size(hemp_context_p, hemp_size_t);
hemp_bool_t     hemp_context_list_pool_cleaner(hemp_mem_p item);
hemp_bool_t     hemp_context_text_pool_cleaner(hemp_mem_p item);


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
