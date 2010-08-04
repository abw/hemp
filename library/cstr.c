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


hemp_list_p
hemp_cstr_splits(
    hemp_cstr_p source,
    hemp_cstr_p token
) {
    hemp_list_p list = hemp_list_init();
    hemp_size_t slen = strlen(source) + 1;
    hemp_size_t tlen = strlen(token);
    hemp_cstr_p from = source, to;
    hemp_pos_t  pos  = 0;
    hemp_cstr_split_p split;
    
    /* we're dynamically allocating memory for each directory so we must
     * ensure that the list has a cleaner function associated with it that
     * will free the memory for each item when the list is destroyed
     */
    list->cleaner = &hemp_list_each_free;

    while ((to = strstr(from, token))) {
        /* we're copying the string so must work on positions, not pointers */
        pos += to - from;

        /* Allocate a single block of memory to hold the split header and a
         * copy of the text string immediately after it.  It's a win because
         * we only need one call to malloc() and another to free() (called by 
         * the generic list cleaner installed above.
         */
        split = (hemp_cstr_split_p) hemp_mem_alloc(
            sizeof(struct hemp_cstr_split_s) + slen
        );
        split->left = (hemp_cstr_p) split + sizeof(struct hemp_cstr_split_s);
        strcpy(split->left, source);
        hemp_list_push(list, split);

        to   = split->left + pos;
        *to  = HEMP_NUL;
        pos += tlen;
        from = split->right = to += tlen;
    }

    if (list->length == 0) {
        hemp_list_free(list);
        list = NULL;
    }
    
    return list;
};


void
hemp_cstr_chomp(
    hemp_cstr_p cstr
) {
    hemp_cstr_p s = cstr;

    /* go to the end of the string */
    while (*s)
        s++;
    
    /* then walk back while the preceding character is whitespace */
    while (s > cstr && isspace(*--s))
        *s = HEMP_NUL;
}


