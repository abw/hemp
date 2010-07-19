#ifndef HEMP_PTREE_H
#define HEMP_PTREE_H

#include "hemp/pool.h"
#include "hemp/cstr.h"


/*--------------------------------------------------------------------------
 * data structures
 *--------------------------------------------------------------------------*/

struct hemp_pnode {
   hemp_cstr_t      key;
   hemp_ptr_t       value;
   hemp_pnode_t     before;
   hemp_pnode_t     equal;
   hemp_pnode_t     after;
};

struct hemp_ptree {
    hemp_pnode_t    head[256];
    hemp_pool_t     pool;
};


/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

hemp_ptree_t 
    hemp_ptree_init(
        hemp_size_t capacity
    );

void
    hemp_ptree_free(
        hemp_ptree_t ptree
    );

hemp_pnode_t
    hemp_ptree_node(
        hemp_ptree_t ptree, 
        hemp_cstr_t  key, 
        hemp_ptr_t  value
    );

hemp_pnode_t
    hemp_ptree_store(
        hemp_ptree_t ptree, 
        hemp_cstr_t  key, 
        hemp_ptr_t  value
    );


hemp_ptr_t
    hemp_ptree_fetch(
        hemp_ptree_t ptree,
        hemp_cstr_t  key
    );

void 
    hemp_ptree_dump(
        hemp_ptree_t ptree
    );


/*--------------------------------------------------------------------------
 * macros
 *--------------------------------------------------------------------------*/

#define hemp_ptree_null(p)  \
    hemp_ptree_free(p);     \
    p = NULL;                


#endif /* HEMP_PTREE_H */
