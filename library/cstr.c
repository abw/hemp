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
    *(str + size) = HEMP_NUL;

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
hemp_cstr_trim(
    hemp_cstr_p cstr
) {
    hemp_cstr_p s = cstr;
    hemp_cstr_p t;
    hemp_size_t len;

    while (isspace(*s))
        s++;

    len = strlen(s);
    if (s > cstr)
        hemp_mem_copy(s, cstr, len);

    t = cstr + len - 1;
    while (t > cstr && isspace(*t))
        t--;

    *++t = HEMP_NUL;
}


void
hemp_cstr_chomp(
    hemp_cstr_p cstr
) {
    hemp_cstr_p s = cstr;
    hemp_assert(s);

    /* go to the end of the string */
    while (*s)
        s++;
    
    /* then walk back while the preceding character is whitespace */
    while (s > cstr && isspace(*--s))
        *s = HEMP_NUL;
}

hemp_bool_t
hemp_cstr_wordlike(
    hemp_cstr_p cstr
) {
    while (isalnum(*cstr) || *cstr == HEMP_UNDERSCORE)
        cstr++;

    /* if we reached the end of the string then all characters are wordlike */
    return *cstr
        ? HEMP_FALSE
        : HEMP_TRUE;
}

HEMP_INLINE hemp_cstr_p
hemp_cstr_next_space(
    hemp_cstr_p cstr
) {
    while (*cstr && ! isspace(*cstr))
        cstr++;

    return *cstr ? cstr : NULL;
}


HEMP_INLINE hemp_bool_t
hemp_cstr_to_next_space(
    hemp_cstr_p *cstr
) {
    hemp_cstr_p space = hemp_cstr_next_space(*cstr);

    if (space) {
        *cstr = space;
        return HEMP_TRUE;
    }
    else {
        return HEMP_FALSE;
    }
}


HEMP_INLINE hemp_cstr_p
hemp_cstr_next_nonspace(
    hemp_cstr_p cstr
) {
    while (*cstr && isspace(*cstr))
        cstr++;

    return *cstr ? cstr : NULL;
}


HEMP_INLINE hemp_bool_t
hemp_cstr_to_next_nonspace(
    hemp_cstr_p *cstr
) {
    hemp_cstr_p nonspace = hemp_cstr_next_nonspace(*cstr);

    if (nonspace) {
        *cstr = nonspace;
        return HEMP_TRUE;
    }
    else {
        return HEMP_FALSE;
    }
}


HEMP_INLINE hemp_cstr_p
hemp_cstr_next_line(
    hemp_cstr_p cstr
) {
    while (*cstr) {
        if (*cstr == HEMP_LF) {
            cstr++;
            return cstr;
        }
        else if (*cstr == HEMP_CR) {
            cstr++;
            if (*cstr == HEMP_LF)
                cstr++;
            return cstr;
        }
        cstr++;
    }
    return NULL;
}


HEMP_INLINE hemp_bool_t
hemp_cstr_to_next_line(
    hemp_cstr_p *cstr
) {
    hemp_cstr_p line = hemp_cstr_next_line(*cstr);

    if (line) {
        *cstr = line;
        return HEMP_TRUE;
    }
    else {
        return HEMP_FALSE;
    }
}


HEMP_INLINE hemp_list_p
hemp_cstr_words(
    hemp_cstr_p cstr
) {
    return hemp_cstr_nwords(cstr, 0);
}


hemp_list_p
hemp_cstr_nwords(
    hemp_cstr_p cstr,
    hemp_size_t max
) {
    hemp_list_p list = hemp_list_init();
    hemp_cstr_p from = cstr, to, item;
    hemp_size_t size = 0;

    list->cleaner = &hemp_list_each_free;

    while (from && hemp_cstr_to_next_nonspace(&from)) {
        /* If the user has requested a maximum number of word splits and if
         * this will be the maxth word then consume everything to the end of
         * the string as the final "word".  Otherwise we only consume up to 
         * the next whitespace character.  But if there isn't another space
         * in the remaining text then 'to' is also NULL and we consume all
         * the remaining text
         */ 
        size++;
         
        if (max && size >= max)
            to = NULL;
        else
            to = hemp_cstr_next_space(from);
        
        if (to) {
            item = hemp_cstr_extract(from, to);
            from = to;
        }
        else {
            item = hemp_cstr_clone(from, "word");
            from = NULL;
        }
        
        hemp_list_push(list, item);
        
    }
    
    if (! size) {
        hemp_list_free(list);
        list = NULL;
    }

    return list;
};


//HEMP_DO_INLINE hemp_bool_t
//hemp_cstr_to_next_line(
//    hemp_cstr_p *cstr
//) {
//    hemp_cstr_p s = *cstr;
//
//    while (*s) {
//        if (*s == HEMP_LF) {
//            s++;
//            break;
//        }
//        else if (*s == HEMP_CR) {
//            s++;
//            if (*s == HEMP_LF)
//                s++;
//            break;
//        }
//        s++;
//    }
//    *cstr = s;
//}
