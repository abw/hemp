#include "hemp/cstr.h"
#include "hemp/debug.h"


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