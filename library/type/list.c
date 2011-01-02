#include <hemp/value.h>
#include <hemp/context.h>
#include <hemp/type/list.h>


HEMP_TYPE_FUNC(hemp_type_list) {
    HempType type   = hemp_type_subtype(HempTypeValue, id, name);
    type->fetch     = &hemp_type_list_fetch;
    type->store     = &hemp_type_list_store;
    type->text      = &hemp_type_list_text;        /* return/append text   */
    type->boolean   = &hemp_value_true;            /* list is always true   */  /* or list size? */
    type->defined   = &hemp_value_true;            /* and always defined   */

    hemp_type_extend(type, "length", &hemp_method_list_length);
    hemp_type_extend(type, "text",   &hemp_method_list_text);

    return type;
};


/*--------------------------------------------------------------------------
 * General purpose list functions
 *--------------------------------------------------------------------------*/

HempList
hemp_list_init(
    HempList list
) {
    HEMP_INSTANCE(list);
    hemp_debug_call("hemp_list_init(%p)\n", list);
    list->items    = NULL;
    list->length   = 0;
    list->capacity = 0;
    list->cleaner  = NULL;
    return list;
}


HEMP_INLINE void
hemp_list_release(
    HempList list
) {
    hemp_debug_call("hemp_list_release(%p)\n", list);
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
    HempList list
) {
    hemp_list_release(list);
    hemp_mem_free(list);
}


HempSize
hemp_list_grow(
    HempList   list, 
    HempSize   min_size
) {
    HempU16    old_size = list->capacity;
    HempU16    new_size = old_size;

    if (! new_size)
        new_size = 1;

    while (min_size > new_size) {
        new_size = new_size << 1;
    }

    if (new_size <= list->capacity)
        return new_size;

    list->items = hemp_mem_resize(
        list->items, 
        new_size * sizeof(HempValue)
    );
    if (! list->items)
        hemp_mem_fail("list item");

    list->capacity = new_size;
//  hemp_debug_mem("resized list to %d\n", list->capacity);

    return new_size;
}


HempSize
hemp_list_resize(
    HempList   list, 
    HempSize   new_size
) {
    HempU16    old_size = list->length;

    if (new_size > old_size) {
        hemp_list_grow(list, new_size);

        while (old_size < new_size) {
            list->items[old_size++] = HempMissing;
        }
    }

    /* In all other cases (new_size < old_size) and (new_size == old_size)
     * it suffices to simply reset the list length to the new value.  
     * TODO: if the list has a cleaner then run it against truncated items.
     */

    list->length = new_size;

    return new_size;
}


HempList
hemp_list_copy(
    HempList src
) {
    HempList list = hemp_list_new();
    hemp_list_grow(list, src->length);
//    hemp_debug_msg(
//        "copying %d bytes from %p to $p\n", 
//        src->length * sizeof(HempValue),
//        src->items, list->items
//    );
    hemp_mem_copy(src->items, list->items, src->length * sizeof(HempValue));
    list->length = src->length;
    return list;
}


HempList
hemp_list_push(
    HempList   list, 
    HempValue  value
) {
    if (list->length == list->capacity) {
        hemp_list_grow(list, list->capacity + 1);
    }
    list->items[list->length++] = value;

    return list;
}


HempList
hemp_list_push_list(
    HempList   list, 
    HempList   values
) {
    HempSize   length = list->length + values->length;
    HempSize   from   = 0;

    if (length > list->capacity)
        hemp_list_grow(list, length);

    while (list->length < length) {
        list->items[list->length++] = values->items[from++];
    }

    return list;
}


HempValue
hemp_list_shift(
    HempList   list
) {
    HempValue  value;

    if (list->length < 1)
        return HempMissing;

    value = list->items[0];
    list->length--;

    /* Quick hack to copy values down in list.  It can be better implemented
     * by having start/end pointers for the list within an allocated range.
     * But this is another case of making it work today and making it better
     * tomorrow.
     */
    if (list->length)
        hemp_mem_copy(
            (HempMemory) list->items + sizeof(HempValue),
            (HempMemory) list->items,
            list->length * sizeof(HempValue)
        );

    return value;
}


HempValue
hemp_list_pop(
    HempList   list
) {
    if (list->length < 1)
        return HempMissing;

    return list->items[--list->length];
}


HempPos
hemp_list_each(
    HempList         list, 
    hemp_list_iter    func
) {
    HempPos pos = 0;
    
    for (pos = 0; pos < list->length; pos++) {
        if (! func(list, pos, list->items[pos]))
            break;
    }
    
    // returns number of items visited
    return pos;
}


HEMP_INLINE HempBool 
hemp_list_index(
    HempContext  context,
    HempValue    key,
    HempInt     *index
) {
    hemp_debug_call("hemp_list_index()\n");
    HempBool found = HEMP_FALSE;

    if (hemp_is_integer(key)) {
//      hemp_debug("got integer key\n");
        *index = hemp_val_int(key);
        found = HEMP_TRUE;
    }
    else {
        HempText ktext;
        HempBool kmine  = HEMP_FALSE;

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

        if (hemp_string_intlike(ktext->string)) {
//          hemp_debug("got numlike string\n");
            *index = hemp_val_int( 
                hemp_type_string_integer( hemp_str_val(ktext->string), context) 
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


HempBool 
hemp_list_each_free(
    HempList     list, 
    HempPos      pos, 
    HempValue    item
) {
//  hemp_debug_mem("freeing list item %d at %p\n", pos, hemp_val_ptr(item));
    /* this assume that each element is a pointer */
    hemp_mem_free( hemp_val_ptr(item) );
    return HEMP_TRUE;
}




/*--------------------------------------------------------------------------
 * Runtime list evaluation methods
 *--------------------------------------------------------------------------*/


HEMP_OUTPUT(hemp_type_list_text) {
    HempList  list = hemp_val_list(value);
    HempValue item;
    HempText  text;
    HempSize  pos;

    hemp_prepare_text(context, output, text);

    for (pos = 0; pos < list->length; pos++) {
        item = hemp_list_item(list, pos);
        hemp_vtext(item, context, output);
    }

    return output;
}


HEMP_FETCH_FUNC(hemp_type_list_fetch) {
    hemp_debug_call("hemp_type_list_fetch()\n");

    HempInt  index = -1;
    HempList list  = hemp_val_list(container);
    HempBool found = hemp_list_index(context, key, &index);

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

    HempInt  index = -1;
    HempBool found = hemp_list_index(context, key, &index);
    HempList list  = hemp_val_list(container);
    
    if (! found) {
//      hemp_debug("no index key\n");
        return HempMissing;
    }

    if (index < 0) {
        hemp_debug("TODO: list bounds error (< 0)\n");
        return HempMissing;
    }

    if (index >= list->capacity) {
//      hemp_debug_msg("list index (%d) is larger than current capacity (%d)\n", index, list->capacity);
        hemp_list_grow(list, index + 1);
    }

    while (index > list->length) {
//      hemp_debug_msg("list index (%d) is larger than current size (%d)\n", index, list->length);
        list->items[list->length++] = HempNothing;
    }
    list->items[index] = value;
    
    if (index >= list->length)
        list->length = index + 1;
    
//  hemp_debug_msg("new length is %d, capacity is %d\n", list->length, list->capacity);
    return value;
}



/*--------------------------------------------------------------------------
 * List pseudo-object methods
 *--------------------------------------------------------------------------*/

HEMP_VALUE(hemp_method_list_length) {
    return hemp_int_val( hemp_val_list(value)->length );
}


HEMP_VALUE(hemp_method_list_text) {
    return hemp_type_list_text(value, context, HempNothing);
}



/*--------------------------------------------------------------------------
 * temporary hack for debugging purposes
 *--------------------------------------------------------------------------*/

HempText hemp_list_dump_buffer = NULL;
 
HempBool 
hemp_list_dump_item(
    HempList     list,
    HempPos      pos, 
    HempValue    item
) {
    hemp_text_append_string(hemp_list_dump_buffer, "    ");
    if (hemp_is_text(item)) {
        hemp_text_append_text(hemp_list_dump_buffer, hemp_val_text(item));
    }
    else if (hemp_is_string(item)) {
        hemp_text_append_string(hemp_list_dump_buffer, hemp_val_str(item));
    }
    else {
        hemp_text_append_string(hemp_list_dump_buffer, hemp_type_name(item));
    }
    hemp_text_append_string(hemp_list_dump_buffer, ",\n");

    return HEMP_TRUE;
}


void 
hemp_list_dump_list(
    HempList list
) {
//  hemp_debug("dump list: %p (%d items)\n", list, list->length);
    hemp_text_append_string(hemp_list_dump_buffer, "[\n");
    hemp_list_each(list, &hemp_list_dump_item);
    hemp_text_append_string(hemp_list_dump_buffer, "]\n");
}


HempText
hemp_list_dump(
    HempList list
) {
    hemp_list_dump_buffer = hemp_text_new();
    hemp_list_dump_list(list);
    return hemp_list_dump_buffer;
}
