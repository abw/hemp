#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hemp/ptree.h"
#include "hemp/debug.h"


hemp_ptree_t
hemp_ptree_init(hemp_size_t capacity) {
    hemp_ptree_t ptree;

    debug_blue("hemp_ptree_init()\n");

    if ((ptree = (hemp_ptree_t) hemp_mem_init(sizeof(struct hemp_ptree)))) {
        if ((ptree->pool = hemp_pool_init(sizeof(struct hemp_pnode), capacity))) {
            debug_cyan(
                "Allocated ptree of %d nodes at %p\n", 
                capacity, ptree->pool
            );
        }
        else {
            hemp_ptree_free(ptree);
        }
    }
    
    // TODO: handle ptree == NULL
    
    return ptree;
}


void
hemp_ptree_free(hemp_ptree_t ptree) {
    if (ptree->pool) {
        debug_cyan(
            "Releasing ptree of %d nodes at %p\n", 
            ptree->pool->capacity, ptree->pool->capacity
        );
        hemp_pool_free(ptree->pool);
    }
    hemp_mem_free(ptree);
}


hemp_pnode_t
hemp_ptree_node(hemp_ptree_t ptree, hemp_text_t key, hemp_ptr_t value) {
    hemp_pnode_t pnode = (hemp_pnode_t) hemp_pool_take(ptree->pool);
    pnode->key     = key;
    pnode->value   = value;
    pnode->before  = 
    pnode->equal   = 
    pnode->after   = NULL;
    return pnode;
}


hemp_pnode_t
hemp_ptree_insert(hemp_ptree_t ptree, hemp_text_t key, hemp_ptr_t value) {
    hemp_pnode_t pnode    = ptree->head[(unsigned char) *key];
    hemp_text_t  keyptr   = key;
    hemp_text_t  cmptr;
    hemp_pnode_t new_node;

    debug_blue("hemp_ptree_insert(%s, %p)\n", key, value);

    if (! pnode) {
        // new top level node
        return (ptree->head[(unsigned char) *key] = hemp_ptree_node(ptree, key, value));
    }

    // cmptr is a pointer into the string in the current pnode that we're 
    // comparing the insert key against
    cmptr = pnode->key;

    while (1) {
        while (*keyptr == *cmptr) {
            keyptr++;
            cmptr++;

            if (! *keyptr) {
                // We've reached the end of the insert key.  If the current 
                // node has a payload then it a duplicate error (TODO: make
                // overwrites optional)
                if (pnode->value) {
                    return NULL;
                }
                else {
                    // Success - added value to an existing pnode
                    pnode->value = value;
                    return pnode;
                }
            }
            
            if (pnode->equal) {
                // Traverse to existing follow-on equal pnode
                pnode = pnode->equal;
                cmptr = pnode->key;
            }
            else if (*cmptr) {
                // Branch node with > 1 characters in the key to create new equal pnode
                new_node = pnode->equal = hemp_ptree_node(ptree, cmptr, pnode->value);
                pnode->value  = NULL;
                pnode = new_node;
            }
            else {
                // Insert key extends existing pnode as new follow-on equal pnode
                return (pnode->equal = hemp_ptree_node(ptree, keyptr, value));
            }
        }

        // we've reached a character that doesn't match
        if (*keyptr < *cmptr) {
            // traverse to existing before node or create a new one
            if (pnode->before) {
                pnode = pnode->before;
                cmptr = pnode->key;
            }
            else {
                return (pnode->before = hemp_ptree_node(ptree, keyptr, value));
            }
        }
        else {
            // traverse to existing after node or create a new one
            if (pnode->after != NULL) {
                pnode = pnode->after;
                cmptr = pnode->key;
            }
            else {
                return (pnode->after = hemp_ptree_node(ptree, keyptr, value));
            }
        }
    }
    
    // not reached
    return pnode;
}



/* NOT USED */
hemp_pnode_t
hemp_pnode_match_more(hemp_pnode_t head, hemp_text_t key) {
    hemp_pnode_t pnode    = head;
    hemp_text_t  keyptr   = key;
    hemp_text_t  cmptr    = pnode->key;

    debug("pnode_match_more(%s, %s)\n", head->key, key);

    cmptr = pnode->key;

    while (1) {
        debug("pnode[%s]: %p \n", pnode->key, pnode);

        while (*keyptr == *cmptr) {
            keyptr++;
            cmptr++;

            if (! *keyptr) {
                // We've reached the end of the search key.
   //             debug("reached end of key: %s\n", key);
                return pnode->value;
            }
            
            if (pnode->equal) {
                // Traverse to existing follow-on equal pnode
 //               debug("traversing to equal pnode[%s] for %s\n", pnode->key, key);
                pnode = pnode->equal;
                cmptr = pnode->key;
            }
            else if (*cmptr) {
                // match the rest of the key...
            }
            else {
                // no match, or perhaps a previous match
            }
        }

        // we've reached a character that doesn't match
        if (*keyptr < *cmptr) {
            // traverse to existing before node or create a new one
            if (pnode->before) {
                pnode = pnode->before;
                cmptr = pnode->key;
            }
            else {
                return NULL;
            }
        }
        else {
            // traverse to existing after node or create a new one
            if (pnode->after != NULL) {
//                debug("traversing to existing node after %s for %s\n", pnode->key, key);
                pnode = pnode->after;
                cmptr = pnode->key;
            }
            else {
                return NULL;
            }
        }
    }
    
    // not reached
    return pnode;
}


void
hemp_pnode_dump(hemp_pnode_t pnode, int indent) {
    char pad[100];
    int len = indent * 4;

    memset(pad, ' ', len);
    pad[len] = '\0';
    
    debug("%s => %p [%p]\n", pnode->key, pnode->value, pnode);

    if (pnode->before) {
        debug("%s  < ", pad);
        hemp_pnode_dump(pnode->before, indent + 1);
    }
    if (pnode->equal) {
        debug("%s  = ", pad);
        hemp_pnode_dump(pnode->equal, indent + 1);
    }
    if (pnode->after) {
        debug("%s  > ", pad);
        hemp_pnode_dump(pnode->after, indent + 1);
    }
}


void
hemp_ptree_dump(hemp_ptree_t ptree) {
    int i;
    
    for (i = 0; i < 256; i++) {
        if (ptree->head[i]) {
            hemp_pnode_dump(ptree->head[i], 0);
        }
    }
}

