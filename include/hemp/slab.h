#ifndef HEMP_SLAB_H
#define HEMP_SLAB_H

#include <hemp/core.h>


/*--------------------------------------------------------------------------
 * data structures
 *--------------------------------------------------------------------------*/

struct hemp_slab_s {
    hemp_size_t size;       /* size of allocated memory     */
    hemp_mem_p  data;       /* pointer to memory            */
    hemp_slab_p next;       /* next slab in linked list     */
};


/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

hemp_slab_p hemp_slab_init(hemp_size_t);
void        hemp_slab_free(hemp_slab_p);


#endif /* HEMP_SLAB_H */

