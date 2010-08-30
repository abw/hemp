#include <hemp/value.h>
#include <hemp/list.h>
#include <hemp/context.h>


HEMP_TYPE_FUNC(hemp_type_string) {
    hemp_type_p type = hemp_type_subtype(HempValue, id, name);
    type->text       = &hemp_value_string_text;     /* string -> text       */
    type->number     = &hemp_value_string_number;   /* string -> number     */
    type->integer    = &hemp_value_string_integer;  /* string -> integer    */
    type->boolean    = &hemp_value_string_boolean;  /* string -> boolean    */
    type->compare    = &hemp_value_not_compare;     /* cannot compare       */
    type->defined    = &hemp_value_true;            /* always defined       */
    return type;
};


/*--------------------------------------------------------------------------
 * String manipulation functions
 *--------------------------------------------------------------------------*/

hemp_str_p
hemp_string_extract(
    hemp_str_p from,
    hemp_str_p to
) {
    hemp_size_t size = to - from;
    hemp_str_p  str  = hemp_mem_alloc(size + 1);
    
    if (! str)
        hemp_mem_fail("string extract");

    strncpy(str, from, size);
    *(str + size) = HEMP_NUL;

    return str;
}


hemp_list_p
hemp_string_split(
    hemp_str_p source,
    hemp_str_p split
) {
    hemp_list_p list = hemp_list_init();
    hemp_str_p  from = source, to;
    hemp_size_t slen = strlen(split);
    hemp_str_p  item;
    hemp_size_t size;
    
    /* we're dynamically allocating memory for each directory so we must
     * ensure that the list has a cleaner function associated with it that
     * will free the memory for each item when the list is destroyed
     */
    list->cleaner = &hemp_list_each_free;

    while ((to = strstr(from, split))) {
        item = hemp_string_extract(from, to);
        hemp_list_push(list, hemp_str_val(item));
        from = (to += slen);
    }

    item = hemp_string_copy(from);
    if (! item)
        hemp_mem_fail("string extract");

    hemp_list_push(list, hemp_str_val(item));

    return list;
};


hemp_list_p
hemp_string_splits(
    hemp_str_p source,
    hemp_str_p token
) {
    hemp_list_p list = hemp_list_init();
    hemp_size_t slen = strlen(source) + 1;
    hemp_size_t tlen = strlen(token);
    hemp_str_p  from  = source, to;
    hemp_pos_t  pos  = 0;
    hemp_string_split_p split;
    
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
        split = (hemp_string_split_p) hemp_mem_alloc(
            sizeof(struct hemp_string_split_s) + slen
        );
        split->left = (hemp_str_p) split + sizeof(struct hemp_string_split_s);
        strcpy(split->left, source);
        hemp_list_push(list, hemp_ptr_val(split));

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
hemp_string_trim(
    hemp_str_p string
) {
    hemp_str_p  s = string;
    hemp_str_p  t;
    hemp_size_t len;

    while (isspace(*s))
        s++;

    len = strlen(s);
    if (s > string)
        hemp_mem_copy(s, string, len);

    t = string + len - 1;
    while (t > string && isspace(*t))
        t--;

    *++t = HEMP_NUL;
}


void
hemp_string_chomp(
    hemp_str_p string
) {
    hemp_str_p s = string;
    hemp_assert(s);

    /* go to the end of the string */
    while (*s)
        s++;
    
    /* then walk back while the preceding character is whitespace */
    while (s > string && isspace(*--s))
        *s = HEMP_NUL;
}

hemp_bool_t
hemp_string_wordlike(
    hemp_str_p string
) {
    while (isalnum(*string) || *string == HEMP_UNDERSCORE)
        string++;

    /* if we reached the end of the string then all characters are wordlike */
    return *string
        ? HEMP_FALSE
        : HEMP_TRUE;
}

HEMP_INLINE hemp_str_p
hemp_string_next_space(
    hemp_str_p string
) {
    while (*string && ! isspace(*string))
        string++;

    return *string ? string : NULL;
}


HEMP_INLINE hemp_bool_t
hemp_string_to_next_space(
    hemp_str_p *string
) {
    hemp_str_p space = hemp_string_next_space(*string);

    if (space) {
        *string = space;
        return HEMP_TRUE;
    }
    else {
        return HEMP_FALSE;
    }
}


HEMP_INLINE hemp_str_p
hemp_string_next_nonspace(
    hemp_str_p string
) {
    while (*string && isspace(*string))
        string++;

    return *string ? string : NULL;
}


HEMP_INLINE hemp_bool_t
hemp_string_to_next_nonspace(
    hemp_str_p *string
) {
    hemp_str_p nonspace = hemp_string_next_nonspace(*string);

    if (nonspace) {
        *string = nonspace;
        return HEMP_TRUE;
    }
    else {
        return HEMP_FALSE;
    }
}


HEMP_INLINE hemp_str_p
hemp_string_next_line(
    hemp_str_p string
) {
    while (*string) {
        if (*string == HEMP_LF) {
            string++;
            return string;
        }
        else if (*string == HEMP_CR) {
            string++;
            if (*string == HEMP_LF)
                string++;
            return string;
        }
        string++;
    }
    return NULL;
}


HEMP_INLINE hemp_bool_t
hemp_string_to_next_line(
    hemp_str_p *string
) {
    hemp_str_p line = hemp_string_next_line(*string);

    if (line) {
        *string = line;
        return HEMP_TRUE;
    }
    else {
        return HEMP_FALSE;
    }
}


HEMP_INLINE hemp_list_p
hemp_string_words(
    hemp_str_p string
) {
    return hemp_string_nwords(string, 0);
}


hemp_list_p
hemp_string_nwords(
    hemp_str_p  string,
    hemp_size_t max
) {
    hemp_list_p list = hemp_list_init();
    hemp_str_p  from = string, to, item;
    hemp_size_t size = 0;

    list->cleaner = &hemp_list_each_free;

    while (from && hemp_string_to_next_nonspace(&from)) {
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
            to = hemp_string_next_space(from);
        
        if (to) {
            item = hemp_string_extract(from, to);
            from = to;
        }
        else {
            item = hemp_string_clone(from, "word");
            from = NULL;
        }
        
        hemp_list_push_string(list, item);
        
    }
    
    if (! size) {
        hemp_list_free(list);
        list = NULL;
    }

    return list;
};


//HEMP_DO_INLINE hemp_bool_t
//hemp_string_to_next_line(
//    hemp_str_p *string
//) {
//    hemp_str_p s = *string;
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
//    *string = s;
//}


/*--------------------------------------------------------------------------
 * Runtime evaluation functions
 *--------------------------------------------------------------------------*/


HEMP_VTEXT_FUNC(hemp_value_string_text) {
    hemp_str_p  str = hemp_val_str(value);
    hemp_text_p text;

    hemp_prepare_output(output, text, strlen(str));

    hemp_text_append_string(text, str);
    return output;
}


HEMP_VALUE_FUNC(hemp_value_string_number) {
    hemp_str_p str = hemp_val_str(value);
    hemp_str_p end;
    hemp_num_t nval;
    
    if (! str || ! *str) {
        HEMP_CONVERT_ERROR(
            context, 
            HEMP_STR_NO_TEXT,
            HEMP_STR_NUMBER,
            HEMP_STR_BLANK
        );
    }

    errno = 0;
    nval  = strtod(str, &end);
    
    if (*end || (errno == EINVAL)) {
        HEMP_CONVERT_ERROR(
            context, 
            HEMP_STR_STRING, 
            HEMP_STR_NUMBER,
            str
        );
    }
    else if (errno == ERANGE) {
        HEMP_OVERFLOW_ERROR(
            context, 
            str
        );
    }
    else {
        return hemp_num_val(nval);
    }

    return hemp_num_val(0);
}


HEMP_VALUE_FUNC(hemp_value_string_integer) {
    hemp_value_t nval = hemp_value_string_number(value, context);
    return hemp_int_val((hemp_int_t) hemp_val_num(nval));
}


HEMP_VALUE_FUNC(hemp_value_string_boolean) {
    hemp_str_p str = hemp_val_str(value);
    /* TODO: check this is right: any non-zero length string is true.
     * What about "0" and "0.000" ?
     */
    return str && *str
        ? HempTrue
        : HempFalse;
}

