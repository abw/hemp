#ifndef HEMP_LIST_H
#define HEMP_LIST_H

#include <hemp/core.h>
#include <hemp/value.h>


/*--------------------------------------------------------------------------
 * type definitions
 *--------------------------------------------------------------------------*/

struct hemp_list {
    hemp_value    * items;
    hemp_size       capacity;
    hemp_size       length;
    hemp_list_iter  cleaner;
}; 


/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

hemp_list 
hemp_list_init(
    hemp_list       list
);

HEMP_INLINE void
hemp_list_release(
    hemp_list       list
);

void
hemp_list_free(
    hemp_list       list
);

hemp_size
hemp_list_resize(
    hemp_list       list, 
    hemp_size       new_size
);

hemp_list
hemp_list_copy(
    hemp_list src
);

hemp_list 
hemp_list_push(
    hemp_list       list, 
    hemp_value      item
);

hemp_list
hemp_list_push_list(
    hemp_list   list, 
    hemp_list   values
);


hemp_value
hemp_list_shift(
    hemp_list       list
);

hemp_value
hemp_list_pop(
    hemp_list   list
);
    

hemp_pos 
hemp_list_each(
    hemp_list       list,
    hemp_list_iter  func
);

hemp_bool
hemp_list_each_free(
    hemp_list       list, 
    hemp_pos        index,
    hemp_value      value
);

HEMP_INLINE hemp_bool 
hemp_list_index(
    hemp_context  context,
    hemp_value    key,
    hemp_int     *index
);
    

// tmp hack for debugging
hemp_text
hemp_list_dump(
    hemp_list       list
);


/*--------------------------------------------------------------------------
 * type functions and methods
 *--------------------------------------------------------------------------*/

HEMP_TYPE_FUNC(hemp_type_list);
HEMP_FETCH_FUNC(hemp_type_list_fetch);
HEMP_STORE_FUNC(hemp_type_list_store);
HEMP_OUTPUT(hemp_type_list_text);
HEMP_VALUE(hemp_method_list_length);
HEMP_VALUE(hemp_method_list_text);

/*--------------------------------------------------------------------------
 * macros
 *--------------------------------------------------------------------------*/

#define hemp_list_new()                 \
    hemp_list_init(NULL)

#define hemp_list_item(list, n)         \
    list->items[n]

#define hemp_list_item_string(list, n)  \
    hemp_val_str( hemp_list_item(list, n) )

#define hemp_list_push_number(list, n)  \
    hemp_list_push( list, hemp_num_val(n) )

#define hemp_list_push_integer(list, i)  \
    hemp_list_push( list, hemp_int_val(i) )

#define hemp_list_push_string(list, s)  \
    hemp_list_push( list, hemp_str_val(s) )


#endif /* HEMP_LIST_H */
