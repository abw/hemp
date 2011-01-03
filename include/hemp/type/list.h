#ifndef HEMP_LIST_H
#define HEMP_LIST_H

#include <hemp/core.h>
#include <hemp/value.h>


/*--------------------------------------------------------------------------
 * Type definitions
 *--------------------------------------------------------------------------*/

struct hemp_list {
    HempValue     * items;
    HempSize        capacity;
    HempSize        length;
    hemp_list_iter  cleaner;
}; 


/*--------------------------------------------------------------------------
 * Function prototypes
 *--------------------------------------------------------------------------*/

HempList 
hemp_list_init(
    HempList        list
);

HEMP_INLINE void
hemp_list_release(
    HempList        list
);

void
hemp_list_free(
    HempList        list
);

HempSize
hemp_list_resize(
    HempList        list, 
    HempSize        new_size
);

HempList
hemp_list_copy(
    HempList src
);

HempList 
hemp_list_push(
    HempList        list, 
    HempValue       item
);

HempList
hemp_list_push_list(
    HempList        list, 
    HempList        values
);


HempValue
hemp_list_shift(
    HempList        list
);

HempValue
hemp_list_pop(
    HempList        list
);
    

HempPos 
hemp_list_each(
    HempList        list,
    hemp_list_iter  func
);

HempBool
hemp_list_each_free(
    HempList        list, 
    HempPos         index,
    HempValue       value
);

HEMP_INLINE HempBool 
hemp_list_index(
    HempContext     context,
    HempValue       key,
    HempInt       * index
);
    

// tmp hack for debugging
HempText
hemp_list_dump(
    HempList        list
);


/*--------------------------------------------------------------------------
 * Type functions and methods
 *--------------------------------------------------------------------------*/

HEMP_TYPE(hemp_type_list);
HEMP_FETCH(hemp_type_list_fetch);
HEMP_STORE(hemp_type_list_store);
HEMP_OUTPUT(hemp_type_list_text);
HEMP_VALUE(hemp_method_list_length);
HEMP_VALUE(hemp_method_list_text);


/*--------------------------------------------------------------------------
 * Macros
 *--------------------------------------------------------------------------*/

#define hemp_list_new()                         \
    hemp_list_init(NULL)

#define hemp_list_item(list, n)                 \
    list->items[n]

#define hemp_list_item_string(list, n)          \
    hemp_val_str( hemp_list_item(list, n) )

#define hemp_list_push_number(list, n)          \
    hemp_list_push( list, hemp_num_val(n) )

#define hemp_list_push_integer(list, i)         \
    hemp_list_push( list, hemp_int_val(i) )

#define hemp_list_push_string(list, s)          \
    hemp_list_push( list, hemp_str_val(s) )


#endif /* HEMP_LIST_H */
