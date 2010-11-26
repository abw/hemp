#include <hemp/value.h>
#include <hemp/context.h>
#include <hemp/type/list.h>


HEMP_TYPE_FUNC(hemp_type_string) {
    hemp_type type = hemp_type_subtype(HempValue, id, name);
    type->text       = &hemp_type_string_text;     /* string -> text       */
    type->number     = &hemp_type_string_number;   /* string -> number     */
    type->integer    = &hemp_type_string_integer;  /* string -> integer    */
    type->boolean    = &hemp_type_string_boolean;  /* string -> boolean    */
    type->compare    = &hemp_value_not_compare;     /* cannot compare       */
    type->defined    = &hemp_value_true;            /* always defined       */

    hemp_type_extend(type, "length", &hemp_method_string_length);

    return type;
};


/*--------------------------------------------------------------------------
 * String manipulation functions
 *--------------------------------------------------------------------------*/

hemp_string
hemp_string_extract(
    hemp_string from,
    hemp_string to
) {
    hemp_size size = to - from;
    hemp_string  str  = hemp_mem_alloc(size + 1);
    
    if (! str)
        hemp_mem_fail("string extract");

    strncpy(str, from, size);
    *(str + size) = HEMP_NUL;

    return str;
}


hemp_list
hemp_string_split(
    hemp_string source,
    hemp_string split
) {
    hemp_list list = hemp_list_new();
    hemp_string  from = source, to;
    hemp_size slen = strlen(split);
    hemp_string  item;
    hemp_size size;
    
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


hemp_list
hemp_string_splits(
    hemp_string source,
    hemp_string token
) {
    hemp_list list = hemp_list_new();
    hemp_size slen = strlen(source) + 1;
    hemp_size tlen = strlen(token);
    hemp_string  from  = source, to;
    hemp_pos  pos  = 0;
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
        split->left = (hemp_string) split + sizeof(struct hemp_string_split_s);
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
    hemp_string string
) {
    hemp_string  s = string;
    hemp_string  t;
    hemp_size len;

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
    hemp_string string
) {
    hemp_string s = string;
    hemp_assert(s);

    /* go to the end of the string */
    while (*s)
        s++;
    
    /* then walk back while the preceding character is whitespace */
    while (s > string && isspace(*--s))
        *s = HEMP_NUL;
}

hemp_bool
hemp_string_wordlike(
    hemp_string string
) {
    while (isalnum(*string) || *string == HEMP_UNDERSCORE)
        string++;

    /* if we reached the end of the string then all characters are wordlike */
    return *string
        ? HEMP_FALSE
        : HEMP_TRUE;
}


hemp_bool
hemp_string_numlike(
    hemp_string string
) {
    /* not strictly correct, but good enough for integer list indexes */
    while (isdigit(*string))
        string++;

    /* if we reached the end of the string then all characters are wordlike */
    return *string
        ? HEMP_FALSE
        : HEMP_TRUE;
}


HEMP_INLINE hemp_string
hemp_string_next_space(
    hemp_string string
) {
    while (*string && ! isspace(*string))
        string++;

    return *string ? string : NULL;
}


HEMP_INLINE hemp_bool
hemp_string_to_next_space(
    hemp_string *string
) {
    hemp_string space = hemp_string_next_space(*string);

    if (space) {
        *string = space;
        return HEMP_TRUE;
    }
    else {
        return HEMP_FALSE;
    }
}


HEMP_INLINE hemp_string
hemp_string_next_nonspace(
    hemp_string string
) {
    while (*string && isspace(*string))
        string++;

    return *string ? string : NULL;
}


HEMP_INLINE hemp_bool
hemp_string_to_next_nonspace(
    hemp_string *string
) {
    hemp_string nonspace = hemp_string_next_nonspace(*string);

    if (nonspace) {
        *string = nonspace;
        return HEMP_TRUE;
    }
    else {
        return HEMP_FALSE;
    }
}


HEMP_INLINE hemp_string
hemp_string_next_line(
    hemp_string string
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


HEMP_INLINE hemp_bool
hemp_string_to_next_line(
    hemp_string *string
) {
    hemp_string line = hemp_string_next_line(*string);

    if (line) {
        *string = line;
        return HEMP_TRUE;
    }
    else {
        return HEMP_FALSE;
    }
}


HEMP_INLINE hemp_list
hemp_string_words(
    hemp_string string
) {
    return hemp_string_nwords(string, 0);
}


hemp_list
hemp_string_nwords(
    hemp_string  string,
    hemp_size max
) {
    hemp_list list = hemp_list_new();
    hemp_string  from = string, to, item;
    hemp_size size = 0;

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


//HEMP_DO_INLINE hemp_bool
//hemp_string_to_next_line(
//    hemp_string *string
//) {
//    hemp_string s = *string;
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


HEMP_OUTPUT_FUNC(hemp_type_string_text) {
    hemp_string  str = hemp_val_str(value);
    hemp_text text;

    hemp_prepare_text_size(context, output, text, strlen(str));

    hemp_text_append_string(text, str);
    return output;
}


HEMP_VALUE_FUNC(hemp_type_string_number) {
    hemp_string str = hemp_val_str(value);
    hemp_string end;
    hemp_num nval;
    
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


HEMP_VALUE_FUNC(hemp_type_string_integer) {
    hemp_value nval = hemp_type_string_number(value, context);
    return hemp_int_val((hemp_int) hemp_val_num(nval));
}


HEMP_VALUE_FUNC(hemp_type_string_boolean) {
    hemp_string str = hemp_val_str(value);
    /* TODO: check this is right: any non-zero length string is true.
     * What about "0" and "0.000" ?
     */
    return str && *str
        ? HempTrue
        : HempFalse;
}


/*--------------------------------------------------------------------------
 * String pseudo-object methods
 *--------------------------------------------------------------------------*/

HEMP_VALUE_FUNC(hemp_method_string_length) {
    return hemp_int_val( strlen( hemp_val_str(value) ) );
}

