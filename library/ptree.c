#include <hemp/ptree.h>

#if HEMP_DEBUG & HEMP_DEBUG_PTREE
#   define hemp_debug_find(fmt,...) hemp_debug_cyan(fmt,##__VA_ARGS__)
#   define hemp_debug_cmp(fmt,...)  hemp_debug_yellow(fmt,##__VA_ARGS__)
#   define hemp_debug_hit(fmt,...)  hemp_debug_green(fmt,##__VA_ARGS__)
#   define hemp_debug_miss(fmt,...) hemp_debug_red(fmt,##__VA_ARGS__)
#   define hemp_debug_walk(fmt,...) hemp_debug_blue(fmt,##__VA_ARGS__)
#else
#   define hemp_debug_find(fmt,...)
#   define hemp_debug_cmp(fmt,...)
#   define hemp_debug_hit(fmt,...)
#   define hemp_debug_miss(fmt,...)
#   define hemp_debug_walk(fmt,...)
#endif
 

hemp_ptree
hemp_ptree_new(
    hemp_size capacity
) {
    hemp_ptree ptree;
    HEMP_ALLOCATE(ptree);
    int i;

    hemp_size width = HEMP_PTREE_SIZE;

    /* allocate memory for pointers to tree heads (er... roots) */
    ptree->roots = (hemp_pnode *) hemp_mem_alloc(
        width * sizeof(struct hemp_pnode)
    );
    if (! ptree->roots)
        hemp_mem_fail("ptree roots");
        
    for (i = 0; i < width; i++) {
        ptree->roots[i] = NULL;
    }

    /* allocate a pool of nodes for the tree */
    ptree->pool = hemp_pool_new(
        sizeof(struct hemp_pnode), 
        capacity,
        NULL
    );

    return ptree;
}


void
hemp_ptree_free(
    hemp_ptree ptree
) {
    if (ptree->roots)
        hemp_mem_free(ptree->roots);

    if (ptree->pool)
        hemp_pool_free(ptree->pool);

    hemp_mem_free(ptree);
}


HEMP_INLINE hemp_pnode
hemp_ptree_node(
    hemp_ptree  ptree, 
    hemp_string key, 
    hemp_memory value
) {
    hemp_pnode pnode = (hemp_pnode) hemp_pool_take(ptree->pool);
    pnode->key       = key;
    pnode->value     = value;
    pnode->before    = 
    pnode->equal     = 
    pnode->after     = NULL;
    return pnode;
}


hemp_pnode
hemp_ptree_store(
    hemp_ptree  ptree, 
    hemp_string key, 
    hemp_memory value
) {
    hemp_pos    position = *key % HEMP_PTREE_SIZE;
    hemp_pnode  pnode    = ptree->roots[position];
    hemp_string keyptr   = key;
    hemp_string cmptr;
    hemp_pnode  new_node;

    if (! pnode)
        return (
            ptree->roots[position] = hemp_ptree_node(
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



hemp_memory
hemp_ptree_fetch(
    hemp_ptree  ptree,
    hemp_string key
) {
    hemp_pos    position = *key % HEMP_PTREE_SIZE;
    hemp_pnode  pnode    = ptree->roots[position];
    hemp_string src      = key;
    hemp_string cmptr    = pnode ? pnode->key : NULL;

    hemp_debug_find("hemp_ptree_fetch(%p, %s)\n", ptree, key);

    while (src && cmptr) {
        hemp_debug_cmp("[%s] ?= [%s]\n", src, cmptr);
        
        if (*src == *cmptr) {
            hemp_debug_cmp("[%c] == [%c]\n", *src, *cmptr);

            if (* ++src) {
                hemp_debug_cmp("more of source to come: %s\n", src);

                if (pnode->equal) {
                    hemp_debug_cmp("[%c->equal]\n", *cmptr);
                    pnode = pnode->equal;
                    cmptr = pnode->key;
                    continue;
                }
                else if (* ++cmptr) {
                    hemp_debug_cmp("[%c->more => %c]\n", *(cmptr - 1), *cmptr);
                    continue;
                }
            }
            else if  (* ++cmptr && ! pnode->equal) {
                hemp_debug_miss("incomplete key lookup: %s\n", cmptr);
                break;
            }
            else {
                hemp_debug_hit("returning final payload: %p\n", pnode->value);
                return pnode->value;
            }
        }
        else if (*src < *cmptr && pnode->before) {
            hemp_debug_walk("[%c is before %c]\n", *src, *cmptr);
            pnode = pnode->before;
            cmptr = pnode->key;
        }
        else if (*src > *cmptr && pnode->after) {
            hemp_debug_walk("[%c is after %c]\n", *src, *cmptr);
            pnode = pnode->after;
            cmptr = pnode->key;
        }
        else {
            break;
        }
    }

    return NULL;
}



hemp_memory
hemp_pnode_match_more(
    hemp_pnode  pnode, 
    hemp_string *srcptr
) {
    hemp_string cmptr  = pnode->key;
    hemp_string src    = *srcptr;
    hemp_memory value  = NULL;

    hemp_debug_find("hemp_ptree_match_more()\n");

    while (1) {
        if (*src == *cmptr) {
            src++;

            if (pnode->equal) {
                hemp_debug_cmp("[%c->equal]", *cmptr);
                value = pnode->value;     // payload - but only if pnode is set - don't want to trash previous match
                pnode = pnode->equal;
                cmptr = pnode->key;
            }
            else if (* ++cmptr) {
                hemp_debug_cmp("[%c->more => %c]", *(cmptr - 1), cmptr);
            }
            else {
                value = pnode->value;
                break;
            }
        }
        else if (*src < *cmptr && pnode->before) {
            hemp_debug_walk("[%c is before %c]", *src, *cmptr);
            pnode = pnode->before;
            cmptr = pnode->key;
        }
        else if (*src > *cmptr && pnode->after) {
            hemp_debug_walk("[%c is after %c]", *src, *cmptr);
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
    hemp_pnode  pnode, 
    int indent
) {
    char pad[100];
    int len = indent * 2;

    memset(pad, ' ', len);
    pad[len] = '\0';
    
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
    hemp_ptree  ptree
) {
    int i;
    
    for (i = 0; i < HEMP_PTREE_SIZE; i++) {
        if (ptree->roots[i]) {
            hemp_pnode_dump(ptree->roots[i], 0);
        }
    }
}


int
hemp_pnode_max_depth(
    hemp_pnode  pnode,
    int         depth
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
    hemp_ptree ptree
) {
    int i;
    int depth;
    int max = 0;
    
    for (i = 0; i < HEMP_PTREE_SIZE; i++) {
        if (ptree->roots[i]) {
            depth = hemp_pnode_max_depth(ptree->roots[i], 1);
            printf("[%d] depth: %d\n", i, depth);
            max   = depth > max ? depth : max;
        }
    }
    
    return max;
}

int
hemp_pnode_count_nodes(
    hemp_pnode pnode
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
    hemp_ptree ptree
) {
    int i;
    int n = 0;
    
    for (i = 0; i < HEMP_PTREE_SIZE; i++) {
        if (ptree->roots[i]) {
            n += hemp_pnode_count_nodes(ptree->roots[i]);
        }
        else {
            n++;
        }
    }
    
    return n;
}
