#ifndef HEMP_TREE_H
#define HEMP_TREE_H

#include <hemp/core.h>
#include <hemp/pool.h>


/*--------------------------------------------------------------------------
 * type definitions
 *--------------------------------------------------------------------------*/

struct hemp_ptree {
    HempPnode     * roots;
    HempSize        width;
    HempPool        pool;
};

struct hemp_pnode {
   HempString       key;
   HempMemory       value;
   HempPnode        before;
   HempPnode        equal;
   HempPnode        after;
};



/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

HempPtree 
hemp_ptree_new(
    HempSize        capacity
);

void
hemp_ptree_free(
    HempPtree       ptree
);

HEMP_INLINE HempPnode
hemp_ptree_node(
    HempPtree       ptree, 
    HempString      key, 
    HempMemory      value
);

HempPnode
hemp_ptree_store(
    HempPtree       ptree, 
    HempString      key, 
    HempMemory      value
);

HempMemory
hemp_ptree_fetch(
    HempPtree       ptree,
    HempString      key
);

HempMemory
hemp_pnode_match_more(
    HempPnode       pnode, 
    HempString    * srcptr
);

void 
hemp_ptree_dump(
    HempPtree       ptree
);

int 
hemp_ptree_max_depth(
    HempPtree       ptree
);

int
hemp_ptree_count_nodes(
    HempPtree       ptree
);


/*--------------------------------------------------------------------------
 * Macros
 *--------------------------------------------------------------------------*/

#define hemp_ptree_root(ptree,src) \
    ptree->roots[(HempChar) *src % HEMP_PTREE_SIZE]



#endif /* HEMP_PTREE_H */
