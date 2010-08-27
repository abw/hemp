#ifndef HEMP_LIST_H
#define HEMP_LIST_H

#include <hemp/core.h>
#include <hemp/value.h>


/*--------------------------------------------------------------------------
 * data structures
 *--------------------------------------------------------------------------*/

struct hemp_list_s {
    hemp_value_t     *items;
    hemp_size_t       capacity;
    hemp_size_t       length;
    hemp_list_each_f  cleaner;
}; 


/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

hemp_list_p hemp_list_init(void);
void        hemp_list_free(hemp_list_p);
hemp_list_p hemp_list_push(hemp_list_p, hemp_value_t);
hemp_pos_t  hemp_list_each(hemp_list_p, hemp_list_each_f);
hemp_bool_t hemp_list_each_free(hemp_list_p, hemp_pos_t, hemp_value_t);

// tmp hack for debugging
hemp_cstr_p hemp_list_dump(hemp_list_p);


/*--------------------------------------------------------------------------
 * macros
 *--------------------------------------------------------------------------*/

#define hemp_list_item(list, n)         \
    list->items[n]

#define hemp_list_item_string(list, n)  \
    hemp_val_str( hemp_list_item(list, n) )

#define hemp_list_push_string(list, s)  \
    hemp_list_push( list, hemp_str_val(s) )


#endif /* HEMP_LIST_H */
