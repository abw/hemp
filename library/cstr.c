#include "hemp/cstr.h"
#include "hemp/type.h"
#include "hemp/debug.h"


/* cstr is a C string, i.e. a char * pointer */

/* hstr is a hemp string - a chunk of text copied from a cstr.  We provide 
 * basic object methods prepare/cleanup to handle the memory management
 */

hemp_type_t
hemp_hstr_type() {
    hemp_type_t hstr = hemp_type_init("HStr", sizeof(hemp_hstr_t));
    hstr->prepare = &hemp_hstr_prepare;
    hstr->cleanup = &hemp_hstr_cleanup;
    return hstr;
}

hemp_bool_t
hemp_hstr_prepare(
    hemp_item_t item,
    HEMP_ARGS
) {
    hemp_hstr_t hstr = (hemp_hstr_t) item;
    hemp_cstr_t cstr = HEMP_ARG(hemp_cstr_t);
    hemp_size_t size = strlen(cstr);
    *hstr = hemp_mem_init(strlen(cstr));
    debug_mem("Allocated %d bytes for hstr at %p\n", size, *hstr);
    strcpy(*hstr, cstr);
    return HEMP_TRUE;
}

hemp_bool_t
hemp_hstr_cleanup(
    hemp_item_t item
) {
    hemp_hstr_t hstr = (hemp_hstr_t) item;
    if (*hstr) {
        debug_mem("Releasing memory for hstr at %p\n", *hstr);
        hemp_mem_free(*hstr);
    }
    return HEMP_TRUE;
}


/*--------------------------------------------------------------------------
 * basic c string manipulation functions
 *--------------------------------------------------------------------------*/

hemp_cstr_t
hemp_cstr_extract(
    hemp_cstr_t from,
    hemp_cstr_t to
) {
    hemp_size_t size = to - from;
    hemp_cstr_t str  = hemp_mem_init(size + 1);

    if (str) {
        strncpy(str, from, size);
        str[size] = '\0';
    }
    return str;
}


hemp_list_t
hemp_cstr_split(
    hemp_cstr_t source,
    hemp_cstr_t split
) {
    hemp_list_t list = hemp_list_init();
    hemp_cstr_t from = source, to;
    hemp_size_t slen = strlen(split);
    hemp_cstr_t item;
    hemp_size_t size;
    
    /* we're dynamically allocating memory for each directory so we must
     * ensure that the list has a cleaner function associated with it that
     * will free the memory for each item when the list is destroyed
     */
    list->cleaner = &hemp_list_each_free;

//  debug("splitting %s on %s\n", from, split);

    while ((to = strstr(from, split))) {
        if ((item = hemp_cstr_extract(from, to))) {
            hemp_list_push(list, item);
            from = (to += slen);
        }
        else {
            hemp_list_null(list);           // TODO: cleanup allocated strings
            return NULL;
        }
    }

    if ((item = strdup(from))) {
        hemp_list_push(list, item);
    }
    else {
        hemp_list_null(list);           // TODO: cleanup allocated strings
    }

    return list;
};