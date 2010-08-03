#ifndef HEMP_LIST_H
#define HEMP_LIST_H

#include <hemp/type.h>


/*--------------------------------------------------------------------------
 * data structures
 *--------------------------------------------------------------------------*/

struct hemp_list_s {
    hemp_mem_p        *items;
    hemp_size_t       capacity;
    hemp_size_t       length;
    hemp_list_each_f  cleaner;
}; 


/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

hemp_list_p hemp_list_init(void);
void        hemp_list_free(hemp_list_p);
hemp_list_p hemp_list_push(hemp_list_p, hemp_mem_p);
hemp_pos_t  hemp_list_each(hemp_list_p, hemp_list_each_f);
hemp_bool_t hemp_list_each_free(hemp_list_p, hemp_pos_t, hemp_mem_p);

// tmp hack for debugging
hemp_cstr_p hemp_list_dump(hemp_list_p);


/*--------------------------------------------------------------------------
 * macros
 *--------------------------------------------------------------------------*/

#define hemp_list_item(list,n)  \
    list->items[n]


#endif /* HEMP_LIST_H */
