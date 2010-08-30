#ifndef HEMP_TREE_H
#define HEMP_TREE_H

#include <hemp/core.h>
#include <hemp/pool.h>


/*--------------------------------------------------------------------------
 * data structures
 *--------------------------------------------------------------------------*/

struct hemp_ptree_s {
    hemp_pnode_p    head[HEMP_PTREE_SIZE];
    hemp_pool_p     pool;
};

struct hemp_pnode_s {
   hemp_str_p       key;
   hemp_mem_p       value;
   hemp_pnode_p     before;
   hemp_pnode_p     equal;
   hemp_pnode_p     after;
};



/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

hemp_ptree_p 
    hemp_ptree_init(
        hemp_size_t capacity
    );

void
    hemp_ptree_free(
        hemp_ptree_p ptree
    );

hemp_pnode_p
    hemp_ptree_node(
        hemp_ptree_p ptree, 
        hemp_str_p   key, 
        hemp_mem_p   value
    );

hemp_pnode_p
    hemp_ptree_store(
        hemp_ptree_p ptree, 
        hemp_str_p   key, 
        hemp_mem_p   value
    );

hemp_mem_p
    hemp_ptree_fetch(
        hemp_ptree_p ptree,
        hemp_str_p   key
    );

hemp_mem_p
    hemp_pnode_match_more(
        hemp_pnode_p    pnode, 
        hemp_str_p     *srcptr
    );

void 
    hemp_ptree_dump(
        hemp_ptree_p ptree
    );

int 
    hemp_ptree_max_depth(
        hemp_ptree_p ptree
    );

int
    hemp_ptree_count_nodes(
        hemp_ptree_p ptree
    );


#endif /* HEMP_PTREE_H */
