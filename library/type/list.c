#include <hemp/value.h>
#include <hemp/context.h>
#include <hemp/type/list.h>


HEMP_TYPE_FUNC(hemp_type_list) {
    hemp_type_p type = hemp_type_subtype(HempValue, id, name);
    type->fetch      = &hemp_type_list_fetch;
    type->text       = &hemp_value_list_text;       /* return/append text   */
    type->boolean    = &hemp_value_true;            /* list is always true   */  /* or list size? */
    type->defined    = &hemp_value_true;            /* and always defined   */

    hemp_type_extend(type, "length", &hemp_method_list_length);

    return type;
};


/*--------------------------------------------------------------------------
 * General purpose list functions
 *--------------------------------------------------------------------------*/

hemp_list_p
hemp_list_init() {
    hemp_list_p list = (hemp_list_p) hemp_mem_alloc(
        sizeof(struct hemp_list_s)
    );

    if (! list)
        hemp_mem_fail("list");

    list->items    = NULL;
    list->length   = 0;
    list->capacity = 0;
    list->cleaner  = NULL;
    
    return list;
}


void
hemp_list_free(
    hemp_list_p list
) {
    if (list->items) {
        if (list->cleaner) {
            hemp_list_each(list, list->cleaner);
        }
        hemp_mem_free(list->items);
        list->items = NULL;
    }
    hemp_mem_free(list);
}


hemp_list_p
hemp_list_push(
    hemp_list_p     list, 
    hemp_value_t    value
) {
    if (list->length == list->capacity) {
        // TODO: slab allocate with overflow to avoid repeated realloc
        // (or just plug in an off-the-shelf list library...)
        list->items = hemp_mem_resize(
            list->items, 
            (list->capacity + 1) * sizeof(hemp_value_t)
        );
        if (! list->items)
            hemp_mem_fail("list item");

        list->capacity++;
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


HEMP_VTEXT_FUNC(hemp_value_list_text) {
    hemp_text_p text;
    hemp_prepare_output(output, text, 32);
    hemp_text_append_string(text, "TODO: list.text");
    return output;
}


HEMP_FETCH_FUNC(hemp_type_list_fetch) {
    hemp_debug(
        "hemp_type_list_fetch()\n"
    );

    hemp_int_t  index;
    hemp_bool_t found = HEMP_FALSE;

    if (hemp_is_integer(key)) {
        hemp_debug("got integer key\n");
        index = hemp_val_int(key);
        found = HEMP_TRUE;
    }
    else {
        hemp_text_p ktext;
        hemp_bool_t kmine  = HEMP_FALSE;

        if (hemp_is_text(key)) {
            hemp_debug("got text key\n");
            ktext = hemp_val_text(key);
        }
        else {
            /* otherwise we have to convert the key to text */
            /* TODO: must be a better way to check for numeric conversion without throwing an error? */
            hemp_debug("creating text key\n");
            ktext = hemp_text_init(16);
            kmine = HEMP_TRUE;
            hemp_onto_text(key, context, hemp_text_val(ktext));
        }
        hemp_debug("list text key: %s\n", ktext->string);

        if (hemp_string_numlike(ktext->string)) {
            hemp_debug("got numlike string\n");
            index = hemp_val_num( 
                hemp_value_string_number( hemp_str_val(ktext->string), context) 
            );
            found = HEMP_TRUE;
        }
        
        if (kmine)
            hemp_text_free(ktext);
    }
    
    if (found)
        hemp_debug("got index key: %d\n", index);
    else
        hemp_debug("no index key\n");
        
    return found 
        ? hemp_list_item( hemp_val_list(container), index )
        : HempMissing;
}



/*--------------------------------------------------------------------------
 * List pseudo-object methods
 *--------------------------------------------------------------------------*/

HEMP_VALUE_FUNC(hemp_method_list_length) {
    return hemp_int_val( hemp_val_list(value)->length );
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

