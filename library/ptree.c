#include <hemp/ptree.h>

#define hemp_debug_cmp(fmt,...) hemp_debug_yellow(fmt,##__VA_ARGS__)
 

/*--------------------------------------------------------------------------
 *  hemp_ptree_init(capacity)
 *
 *  Create a new tree.
 *--------------------------------------------------------------------------*/

hemp_ptree_p
hemp_ptree_init(
    hemp_size_t capacity
) {
    hemp_ptree_p ptree = (hemp_ptree_p) hemp_mem_alloc(
        sizeof(struct hemp_ptree_s)
    );
    int i;

    if (! ptree)
        hemp_mem_fail("ptree");

    ptree->pool = hemp_pool_init(
        sizeof(struct hemp_pnode_s), capacity
    );

    for (i = 0; i < HEMP_PTREE_SIZE; i++) {
        ptree->head[i] = NULL;
    }

    return ptree;
}


/*--------------------------------------------------------------------------
 *  hemp_ptree_free(ptree)
 *
 *  Cleanup and release the memory used by a ptree.
 *--------------------------------------------------------------------------*/

void
hemp_ptree_free(
    hemp_ptree_p ptree
) {
    if (ptree->pool)
        hemp_pool_free(ptree->pool);

    hemp_mem_free(ptree);
}



/*--------------------------------------------------------------------------
 *  hemp_ptree_node(ptree, key, value)
 *
 *  Allocate a new node for inserting into a ptree.
 *--------------------------------------------------------------------------*/

hemp_pnode_p
hemp_ptree_node(
    hemp_ptree_p    ptree, 
    hemp_cstr_p     key, 
    hemp_mem_p      value
) {
    hemp_pnode_p pnode = (hemp_pnode_p) hemp_pool_take(ptree->pool);
    pnode->key     = key;
    pnode->value   = value;
    pnode->before  = 
    pnode->equal   = 
    pnode->after   = NULL;
    return pnode;
}



/*--------------------------------------------------------------------------
 *  hemp_ptree_store(ptree, key, value)
 *
 *  Store a value in a ptree indexed by a key.
 *--------------------------------------------------------------------------*/

hemp_pnode_p
hemp_ptree_store(
    hemp_ptree_p    ptree, 
    hemp_cstr_p     key, 
    hemp_mem_p      value
) {
    hemp_pos_t      position = *key % HEMP_PTREE_SIZE;
    hemp_pnode_p    pnode    = ptree->head[position];
    hemp_cstr_p     keyptr   = key;
    hemp_cstr_p     cmptr;
    hemp_pnode_p    new_node;

    if (! pnode)
        return (
            ptree->head[position] = hemp_ptree_node(
                ptree, key, value
            )
        );

    /* ptr into string in current pnode that we're comparing to insert key */
    cmptr = pnode->key;

    while (1) {
        while (*keyptr == *cmptr) {
            keyptr++;
            cmptr++;

            if (! *keyptr) {
                /* We've reached the end of the insert key */
                if (pnode->value) {
                    if (*cmptr && ! pnode->equal) {
                        pnode->equal = hemp_ptree_node(ptree, cmptr, pnode->value);
                        pnode->value = value;
                        return pnode;
                    }
                    else {
                        /* If current node has a payload then its a duplicate */
                        /* TODO: make overwrites optional */
                        return NULL;
                    }
                }
                else {
                    /* Success - added value to an existing pnode */
                    pnode->value = value;
                    return pnode;
                }
            }
            
            if (pnode->equal) {
                /* Traverse to existing follow-on equal pnode */
                pnode = pnode->equal;
                cmptr = pnode->key;
            }
            else if (*cmptr) {
                /* Branch node with > 1 characters in the key to create a new
                 * "equal" pnode
                 */
                new_node = pnode->equal = hemp_ptree_node(ptree, cmptr, pnode->value);
                pnode->value = NULL;
                pnode = new_node;
            }
            else {
                /* Insert key extends existing pnode via new equal pnode */
                return (pnode->equal = hemp_ptree_node(ptree, keyptr, value));
            }
        }

        /* we've reached a character that doesn't match */
        if (*keyptr < *cmptr) {
            /* traverse to existing before node or create a new one */
            if (pnode->before) {
                pnode = pnode->before;
                cmptr = pnode->key;
            }
            else {
                return (pnode->before = hemp_ptree_node(ptree, keyptr, value));
            }
        }
        else {
            /* traverse to existing after node or create a new one */
            if (pnode->after != NULL) {
                pnode = pnode->after;
                cmptr = pnode->key;
            }
            else {
                return (pnode->after = hemp_ptree_node(ptree, keyptr, value));
            }
        }
    }
    
    /* not reached */
    return pnode;
}



hemp_mem_p
hemp_ptree_fetch(
    hemp_ptree_p ptree,
    hemp_cstr_p  key
) {
    hemp_pos_t   position = *key % HEMP_PTREE_SIZE;
    hemp_pnode_p pnode    = ptree->head[position];
    hemp_cstr_p  src      = key;
    hemp_cstr_p  cmptr    = pnode ? pnode->key : NULL;

    while (src && cmptr) {
//      hemp_debug_yellow("[%p] ?= [%p]\n", src, cmptr);
        
        if (*src == *cmptr) {
//          hemp_debug_yellow("[%c] == [%c]\n", *src, *cmptr);

            if (* ++src) {
//              hemp_debug_yellow("more of source to come: %s\n", src);

                if (pnode->equal) {
//                  hemp_debug_yellow("[%c->equal]", *cmptr);
                    pnode = pnode->equal;
                    cmptr = pnode->key;
                    continue;
                }
                else if (* ++cmptr) {
//                  hemp_debug_yellow("[%c->more => %c]\n", *(cmptr - 1), *cmptr);
                    continue;
                }
            }
            else if  (* ++cmptr && ! pnode->equal) {
//              hemp_debug("incomplete key lookup: %s\n", cmptr);
                break;
            }
            else {
//              hemp_debug("returning final payload: %s\n", pnode->value);
                return pnode->value;
            }
        }
        else if (*src < *cmptr && pnode->before) {
//          hemp_debug_blue("[%c is before %c]", *src, *cmptr);
            pnode = pnode->before;
            cmptr = pnode->key;
        }
        else if (*src > *cmptr && pnode->after) {
//          hemp_debug_blue("[%c is after %c]", *src, *cmptr);
            pnode = pnode->after;
            cmptr = pnode->key;
        }
        else {
            break;
        }
    }

    return NULL;
}



hemp_mem_p
hemp_pnode_match_more(
    hemp_pnode_p    pnode, 
    hemp_cstr_p     *srcptr
) {
    hemp_cstr_p cmptr  = pnode->key;
    hemp_cstr_p src    = *srcptr;
    hemp_mem_p  value  = NULL;

    while (1) {
        if (*src == *cmptr) {
            src++;

            if (pnode->equal) {
//              hemp_debug_yellow("[%c->equal]", *cmptr);
                value = pnode->value;     // payload - but only if pnode is set - don't want to trash previous match
                pnode = pnode->equal;
                cmptr = pnode->key;
            }
            else if (* ++cmptr) {
//              hemp_debug_yellow("[%c->more => %c]", *(cmptr - 1), cmptr);
            }
            else {
                value = pnode->value;
                break;
            }
        }
        else if (*src < *cmptr && pnode->before) {
//          hemp_debug_blue("[%c is before %c]", *src, *cmptr);
            pnode = pnode->before;
            cmptr = pnode->key;
        }
        else if (*src > *cmptr && pnode->after) {
//          hemp_debug_blue("[%c is after %c]", *src, *cmptr);
            pnode = pnode->after;
            cmptr = pnode->key;
        }
        else {
            break;
        }
    }
  
    if (value)
        *srcptr = src;

    return value;
}


void
hemp_pnode_dump(
    hemp_pnode_p pnode, 
    int indent
) {
    char pad[100];
    int len = indent * 2;

    memset(pad, ' ', len);
    pad[len] = '\0';
    
//    hemp_debug("%s => %p [%p]\n", pnode->key, pnode->value, pnode);
    hemp_debug("%s%s%s => %s [%p]\n", HEMP_ANSI_CYAN, pnode->key, HEMP_ANSI_RESET, pnode->value, pnode);

    if (pnode->before) {
        hemp_debug("%s %s<%s ", pad, HEMP_ANSI_RED, HEMP_ANSI_RESET);
        hemp_pnode_dump(pnode->before, indent + 1);
    }
    if (pnode->equal) {
        hemp_debug("%s %s=%s ", pad, HEMP_ANSI_RED, HEMP_ANSI_RESET);
        hemp_pnode_dump(pnode->equal, indent + 1);
    }
    if (pnode->after) {
        hemp_debug("%s %s>%s ", pad, HEMP_ANSI_RED, HEMP_ANSI_RESET);
        hemp_pnode_dump(pnode->after, indent + 1);
    }
}


void
hemp_ptree_dump(
    hemp_ptree_p ptree
) {
    int i;
    
    for (i = 0; i < HEMP_PTREE_SIZE; i++) {
        if (ptree->head[i]) {
            hemp_pnode_dump(ptree->head[i], 0);
        }
    }
}


int
hemp_pnode_max_depth(
    hemp_pnode_p pnode,
    int depth
) {
    int before = pnode->before 
        ? hemp_pnode_max_depth(pnode->before, depth + 1)
        : depth;
    int equal = pnode->equal 
        ? hemp_pnode_max_depth(pnode->equal, depth + 1)
        : depth;
    int after = pnode->after 
        ? hemp_pnode_max_depth(pnode->after, depth + 1)
        : depth;

    int max = before > after 
        ? before
        : after;
        
    return equal > max
        ? equal
        : max;
}

int
hemp_ptree_max_depth(
    hemp_ptree_p ptree
) {
    int i;
    int depth;
    int max = 0;
    
    for (i = 0; i < HEMP_PTREE_SIZE; i++) {
        if (ptree->head[i]) {
            depth = hemp_pnode_max_depth(ptree->head[i], 1);
            printf("[%d] depth: %d\n", i, depth);
            max   = depth > max ? depth : max;
        }
    }
    
    return max;
}

int
hemp_pnode_count_nodes(
    hemp_pnode_p pnode
) {
    int n = 1;
    if (pnode->before)
        n += hemp_pnode_count_nodes(pnode->before);
    if (pnode->after)
        n += hemp_pnode_count_nodes(pnode->after);
    if (pnode->equal)
        n += hemp_pnode_count_nodes(pnode->equal);
    return n;
}


int
hemp_ptree_count_nodes(
    hemp_ptree_p ptree
) {
    int i;
    int n = 0;
    
    for (i = 0; i < HEMP_PTREE_SIZE; i++) {
        if (ptree->head[i]) {
            n += hemp_pnode_count_nodes(ptree->head[i]);
        }
        else {
            n++;
        }
    }
    
    return n;
}
