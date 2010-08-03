#include <hemp/core.h>
#include <hemp/list.h>


/*--------------------------------------------------------------------------
 * basic c string manipulation functions
 *--------------------------------------------------------------------------*/

hemp_cstr_p
hemp_cstr_extract(
    hemp_cstr_p from,
    hemp_cstr_p to
) {
    hemp_size_t size = to - from;
    hemp_cstr_p str  = hemp_mem_alloc(size + 1);
    
    if (! str)
        hemp_mem_fail("string extract");

    strncpy(str, from, size);
    str[size] = '\0';

    return str;
}


hemp_list_p
hemp_cstr_split(
    hemp_cstr_p source,
    hemp_cstr_p split
) {
    hemp_list_p list = hemp_list_init();
    hemp_cstr_p from = source, to;
    hemp_size_t slen = strlen(split);
    hemp_cstr_p item;
    hemp_size_t size;
    
    /* we're dynamically allocating memory for each directory so we must
     * ensure that the list has a cleaner function associated with it that
     * will free the memory for each item when the list is destroyed
     */
    list->cleaner = &hemp_list_each_free;

    while ((to = strstr(from, split))) {
        item = hemp_cstr_extract(from, to);
        hemp_list_push(list, item);
        from = (to += slen);
    }

    item = hemp_cstr_copy(from);
    if (! item)
        hemp_mem_fail("string extract");

    hemp_list_push(list, item);

    return list;
};


