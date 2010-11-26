#ifndef HEMP_TREE_H
#define HEMP_TREE_H

#include <hemp/core.h>
#include <hemp/pool.h>


/*--------------------------------------------------------------------------
 * data structures
 *--------------------------------------------------------------------------*/

struct hemp_ptree {
    hemp_pnode         *roots;
    hemp_size           width;
    hemp_pool           pool;
};

struct hemp_pnode {
   hemp_string          key;
   hemp_memory          value;
   hemp_pnode           before;
   hemp_pnode           equal;
   hemp_pnode           after;
};



/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

hemp_ptree 
    hemp_ptree_new(
        hemp_size       capacity
    );

void
    hemp_ptree_free(
        hemp_ptree      ptree
    );

HEMP_INLINE hemp_pnode
    hemp_ptree_node(
        hemp_ptree      ptree, 
        hemp_string     key, 
        hemp_memory     value
    );

hemp_pnode
    hemp_ptree_store(
        hemp_ptree      ptree, 
        hemp_string     key, 
        hemp_memory     value
    );

hemp_memory
    hemp_ptree_fetch(
        hemp_ptree      ptree,
        hemp_string     key
    );

hemp_memory
    hemp_pnode_match_more(
        hemp_pnode      pnode, 
        hemp_string   * srcptr
    );

void 
    hemp_ptree_dump(
        hemp_ptree      ptree
    );

int 
    hemp_ptree_max_depth(
        hemp_ptree      ptree
    );

int
    hemp_ptree_count_nodes(
        hemp_ptree      ptree
    );


/*--------------------------------------------------------------------------
 * Macros
 *--------------------------------------------------------------------------*/

#define hemp_ptree_root(ptree,src) \
    ptree->roots[(hemp_char) *src % HEMP_PTREE_SIZE]



#endif /* HEMP_PTREE_H */
