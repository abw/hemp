#ifndef HEMP_LIST_H
#define HEMP_LIST_H

#include <string.h>
#include "hemp/memory.h"
#include "hemp/text.h"


/*--------------------------------------------------------------------------
 * data structures
 *--------------------------------------------------------------------------*/

struct hemp_list {
    hemp_ptr_t        *items;
    hemp_size_t       capacity;
    hemp_size_t       length;
    hemp_list_each_fn cleaner;
}; 


/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

hemp_list_t hemp_list_init(void);
void        hemp_list_free(hemp_list_t list);
hemp_list_t hemp_list_push(hemp_list_t list, hemp_ptr_t item);
hemp_pos_t  hemp_list_each(hemp_list_t list, hemp_list_each_fn func);
hemp_bool_t hemp_list_each_free(hemp_list_t list, hemp_pos_t pos, hemp_ptr_t item);

// tmp hack for debugging
hemp_cstr_t hemp_list_dump(hemp_list_t list);


/*--------------------------------------------------------------------------
 * macros
 *--------------------------------------------------------------------------*/

#define hemp_list_null(l)       \
    hemp_list_free(l);          \
    l = NULL;

#define hemp_list_item(list,n)  \
    list->items[n]


#endif /* HEMP_LIST_H */
