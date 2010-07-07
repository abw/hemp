#ifndef HEMP_PTREE_H
#define HEMP_PTREE_H

#include "hemp/pool.h"

typedef struct hemp_pnode * hemp_pnode_t;
typedef struct hemp_ptree * hemp_ptree_t;

struct hemp_pnode {
   hemp_text_t      key;
   hemp_ptr_t       value;
   hemp_pnode_t     before;
   hemp_pnode_t     equal;
   hemp_pnode_t     after;
};

struct hemp_ptree {
    hemp_pnode_t    head[256];
    hemp_pool_t     pool;
};

hemp_ptree_t hemp_ptree_init(hemp_size_t capacity);
void         hemp_ptree_free(hemp_ptree_t ptree);
hemp_pnode_t hemp_ptree_node(hemp_ptree_t ptree, hemp_text_t key, hemp_ptr_t value);
hemp_pnode_t hemp_ptree_insert(hemp_ptree_t ptree, hemp_text_t key, hemp_ptr_t value);

void hemp_ptree_dump(hemp_ptree_t ptree);

#endif /* HEMP_PTREE_H */
