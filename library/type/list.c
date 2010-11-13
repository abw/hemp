#include <hemp/value.h>
#include <hemp/context.h>
#include <hemp/type/list.h>


HEMP_TYPE_FUNC(hemp_type_list) {
    hemp_type_p type = hemp_type_subtype(HempValue, id, name);
    type->fetch      = &hemp_type_list_fetch;
    type->store      = &hemp_type_list_store;
    type->text       = &hemp_type_list_text;       /* return/append text   */
    type->boolean    = &hemp_value_true;            /* list is always true   */  /* or list size? */
    type->defined    = &hemp_value_true;            /* and always defined   */

    hemp_type_extend(type, "length", &hemp_method_list_length);
    hemp_type_extend(type, "text",   &hemp_method_list_text);

    return type;
};


/*--------------------------------------------------------------------------
 * General purpose list functions
 *--------------------------------------------------------------------------*/

hemp_list_p
hemp_list_init(
    hemp_list_p list
) {
    if (! list) {
        list = (hemp_list_p) hemp_mem_alloc(
            sizeof(struct hemp_list_s)
        );
        if (! list)
            hemp_mem_fail("list");
    }

    list->items    = NULL;
    list->length   = 0;
    list->capacity = 0;
    list->cleaner  = NULL;
    
    return list;
}


HEMP_INLINE void
hemp_list_release(
    hemp_list_p list
) {
    if (list->items) {
        if (list->cleaner) {
            hemp_list_each(list, list->cleaner);
        }
        hemp_mem_free(list->items);
        list->items = NULL;
    }
}


void
hemp_list_free(
    hemp_list_p list
) {
    hemp_list_release(list);
    hemp_mem_free(list);
}


hemp_size_t
hemp_list_resize(
    hemp_list_p     list, 
    hemp_size_t     min_size
) {
    hemp_u16_t    old_size = list->capacity;
    hemp_u16_t    new_size = old_size;
    hemp_u16_t    size;

    if (! new_size)
        new_size = 1;

    while (min_size > new_size) {
        new_size = new_size << 1;
    }

    list->items = hemp_mem_resize(
        list->items, 
        new_size * sizeof(hemp_value_t)
    );
    if (! list->items)
        hemp_mem_fail("list item");

    list->capacity = new_size;
//  hemp_debug_mem("resized list to %d\n", list->capacity);

    return new_size;
}



hemp_list_p
hemp_list_push(
    hemp_list_p     list, 
    hemp_value_t    value
) {
    if (list->length == list->capacity) {
        hemp_list_resize(list, list->capacity + 1);
    }
    list->items[list->length++] = value;

    return list;
}


hemp_pos_t
hemp_list_each(
    hemp_list_p         list, 
    hemp_list_each_f    func
) {
    hemp_pos_t pos = 0;
    
    for (pos = 0; pos < list->length; pos++) {
        if (! func(list, pos, list->items[pos]))
            break;
    }
    
    // returns number of items visited
    return pos;
}


HEMP_INLINE hemp_bool_t 
hemp_list_index(
    hemp_context_p  context,
    hemp_value_t    key,
    hemp_int_t     *index
) {
    hemp_debug_call("hemp_list_index()\n");
    hemp_bool_t found = HEMP_FALSE;

    if (hemp_is_integer(key)) {
//      hemp_debug("got integer key\n");
        *index = hemp_val_int(key);
        found = HEMP_TRUE;
    }
    else {
        hemp_text_p ktext;
        hemp_bool_t kmine  = HEMP_FALSE;

        if (hemp_is_text(key)) {
//          hemp_debug("got text key\n");
            ktext = hemp_val_text(key);
        }
        else {
            /* otherwise we have to convert the key to text */
            /* TODO: must be a better way to check for numeric conversion without throwing an error? */
//          hemp_debug("creating text key\n");
            ktext = hemp_text_new_size(16);
            kmine = HEMP_TRUE;
            hemp_onto_text(key, context, hemp_text_val(ktext));
        }
//      hemp_debug("list text key: %s\n", ktext->string);

        if (hemp_string_numlike(ktext->string)) {
//          hemp_debug("got numlike string\n");
            *index = hemp_val_num( 
                hemp_value_string_number( hemp_str_val(ktext->string), context) 
            );
            found = HEMP_TRUE;
        }
        else {
//          hemp_debug("text index is not numlike: %s\n", ktext->string);
        }
        
        if (kmine)
            hemp_text_free(ktext);
    }

    return found;
}


hemp_bool_t 
hemp_list_each_free(
    hemp_list_p     list, 
    hemp_pos_t      pos, 
    hemp_value_t    item
) {
/*  hemp_debug_mem("freeing list item at %p\n", item); */
    /* this assume that each element is a pointer */
    hemp_mem_free( hemp_val_ptr(item) );
    return HEMP_TRUE;
}




/*--------------------------------------------------------------------------
 * Runtime list evaluation methods
 *--------------------------------------------------------------------------*/


HEMP_VTEXT_FUNC(hemp_type_list_text) {
    hemp_list_p  list = hemp_val_list(value);
    hemp_value_t item;
    hemp_text_p  text;
    hemp_size_t  pos;

    hemp_prepare_text(context, output, text);

    for (pos = 0; pos < list->length; pos++) {
        item = hemp_list_item(list, pos);
        hemp_text(item, context, output);
    }

    return output;
}


HEMP_FETCH_FUNC(hemp_type_list_fetch) {
    hemp_debug_call("hemp_type_list_fetch()\n");

    hemp_int_t  index;
    hemp_list_p list  = hemp_val_list(container);
    hemp_bool_t found = hemp_list_index(context, key, &index);

    if (! found) {
//      hemp_debug("no index key\n");
        return HempMissing;
    }

    /* TODO: proper bounds checking error, also negative numbers count back */
    if (index < 0 || index >= list->length  ) {
        hemp_debug("TODO: list bounds error\n");
        return HempMissing;
    }

    return found 
        ? hemp_list_item( hemp_val_list(container), index )
        : HempMissing;
}



HEMP_STORE_FUNC(hemp_type_list_store) {
    hemp_debug_call("hemp_type_list_store()\n");

    hemp_int_t  index;
    hemp_bool_t found = hemp_list_index(context, key, &index);
    hemp_list_p list  = hemp_val_list(container);
    
    if (! found) {
//      hemp_debug("no index key\n");
        return HempMissing;
    }

    if (index < 0) {
        hemp_debug("TODO: list bounds error (< 0)\n");
        return HempMissing;
    }

    if (index >= list->capacity) {
        hemp_debug("list index (%d) is larger than current capacity (%d)\n", index, list->capacity);
        hemp_list_resize(list, index + 1);
    }

    while (index > list->length) {
        hemp_debug("list index (%d) is larger than current size (%d)\n", index, list->length);
        list->items[list->length++] = HempNothing;
    }
    list->items[index] = value;
    
    if (index >= list->length)
        list->length = index + 1;
    
//  hemp_debug("new length is %d, capacity is %d\n", list->length, list->capacity);
    return value;
}



/*--------------------------------------------------------------------------
 * List pseudo-object methods
 *--------------------------------------------------------------------------*/

HEMP_VALUE_FUNC(hemp_method_list_length) {
    return hemp_int_val( hemp_val_list(value)->length );
}


HEMP_VALUE_FUNC(hemp_method_list_text) {
    return hemp_type_list_text(value, context, HempNothing);
}



/*--------------------------------------------------------------------------
 * temporary hack for debugging purposes
 *--------------------------------------------------------------------------*/

char buffer[1024];                  // FIXME!

hemp_bool_t 
hemp_list_dump_item(
    hemp_list_p     list, 
    hemp_pos_t      pos, 
    hemp_value_t    item
) {
    hemp_str_p str = hemp_val_str(item);
    if (buffer[0])
        strcat(buffer, ", ");
    strcat(buffer, str);
    return HEMP_TRUE;
}


hemp_str_p
hemp_list_dump(
    hemp_list_p list
) {
    buffer[0] = '\0';
    hemp_list_each(list, &hemp_list_dump_item);
    return buffer;
}

