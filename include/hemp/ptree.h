#ifndef HEMP_TREE_H
#define HEMP_TREE_H

#include <hemp/core.h>
#include <hemp/pool.h>


/*--------------------------------------------------------------------------
 * data structures
 *--------------------------------------------------------------------------*/

struct hemp_ptree_s {
    hemp_pnode_p    head[HEMP_PTREE_SIZE];
    hemp_pool     pool;
};

struct hemp_pnode_s {
   hemp_string       key;
   hemp_memory       value;
   hemp_pnode_p     before;
   hemp_pnode_p     equal;
   hemp_pnode_p     after;
};



/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

hemp_ptree_p 
    hemp_ptree_init(
        hemp_size capacity
    );

void
    hemp_ptree_free(
        hemp_ptree_p ptree
    );

hemp_pnode_p
    hemp_ptree_node(
        hemp_ptree_p ptree, 
        hemp_string   key, 
        hemp_memory   value
    );

hemp_pnode_p
    hemp_ptree_store(
        hemp_ptree_p ptree, 
        hemp_string   key, 
        hemp_memory   value
    );

hemp_memory
    hemp_ptree_fetch(
        hemp_ptree_p ptree,
        hemp_string   key
    );

hemp_memory
    hemp_pnode_match_more(
        hemp_pnode_p    pnode, 
        hemp_string     *srcptr
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


/*--------------------------------------------------------------------------
 * Macros
 *--------------------------------------------------------------------------*/

#define HEMP_IN_PTREE(head,src) \
    head[(hemp_char) *src % HEMP_PTREE_SIZE]


#endif /* HEMP_PTREE_H */
