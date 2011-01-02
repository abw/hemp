#include <hemp/value.h>
#include <hemp/context.h>
#include <hemp/type/list.h>


HEMP_TYPE_FUNC(hemp_type_string) {
    HempType type   = hemp_type_subtype(HempTypeValue, id, name);
    type->text      = &hemp_type_string_text;     /* string -> text       */
    type->number    = &hemp_type_string_number;   /* string -> number     */
    type->integer   = &hemp_type_string_integer;  /* string -> integer    */
    type->boolean   = &hemp_type_string_boolean;  /* string -> boolean    */
    type->compare   = &hemp_value_not_compare;     /* cannot compare       */
    type->defined   = &hemp_value_true;            /* always defined       */

    hemp_type_extend(type, "length", &hemp_method_string_length);

    return type;
};


/*--------------------------------------------------------------------------
 * String manipulation functions
 *--------------------------------------------------------------------------*/

HempString
hemp_string_vprintf(
    const HempString format,
    va_list args
) {
    HempString  string;
    vasprintf(&string, format, args);

    if (! string)
        hemp_mem_fail("string sprintf");

#if HEMP_DEBUG & HEMP_DEBUG_MEM
    /* tell the memory trace debugging code that we know about this memory */
    hemp_mem_trace_external(string, strlen(string) + 1, __FILE__, __LINE__);
#endif

    return string;
}


HempString
hemp_string_sprintf(
    const HempString format,
    ...
) {
    HempString  string;

    va_list args;
    va_start(args, format);
    string = hemp_string_vprintf(format, args);
    va_end(args);
    return string;
}


HempString
hemp_string_extract(
    HempString from,
    HempString to
) {
    HempSize   size = to - from;
    HempString str  = hemp_mem_alloc(size + 1);
    
    if (! str)
        hemp_mem_fail("string extract");

    strncpy(str, from, size);
    *(str + size) = HEMP_NUL;

    return str;
}


HempList
hemp_string_split(
    HempString source,
    HempString split
) {
    HempList   list = hemp_list_new();
    HempString from = source, to;
    HempSize   slen = strlen(split);
    HempString item;

    // TODO: if the split character has a finite length then we can dup the
    // string once and replace the start of each delimiter with NUL.  Then
    // each list element can point to the start of each item in the dup buffer.
    // The slightly ugly part would be the list cleaner which would have to
    // free the memory pointed to by the first item.  That would require the
    // list to be immutable.  Otherwise the user could push an item onto the
    // front and the wrong piece of memory could be freed.  Another approach
    // might be to make the list cleaner an action that can encapsulate the
    // memory pointer as an argument.  Although that means we then have an
    // action to clean up as well...

    /* we're dynamically allocating memory for each item so we must
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


HempList
hemp_string_splits(
    HempString source,
    HempString token
) {
    HempList list = hemp_list_new();
    HempSize slen = strlen(source) + 1;
    HempSize tlen = strlen(token);
    HempString  from  = source, to;
    HempPos  pos  = 0;
    hemp_str_split split;
    
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
        split = (hemp_str_split) hemp_mem_alloc(
            sizeof(struct hemp_str_split) + slen
        );
        split->left = (HempString) split + sizeof(struct hemp_str_split);
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
    HempString string
) {
    HempString  s = string;
    HempString  t;
    HempSize len;

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


HEMP_INLINE void
hemp_string_chomp(
    HempString string
) {
    HempString s = string;
    hemp_assert(s);

    /* go to the end of the string */
    while (*s)
        s++;
    
    /* then walk back while the preceding character is whitespace */
    while (s > string && isspace(*--s))
        *s = HEMP_NUL;
}

HEMP_INLINE HempBool
hemp_string_wordlike(
    HempString string
) {
    while (isalnum(*string) || *string == HEMP_UNDERSCORE)
        string++;

    /* if we reached the end of the string then all characters are wordlike */
    return *string
        ? HEMP_FALSE
        : HEMP_TRUE;
}


HEMP_INLINE HempBool
hemp_string_intlike(
    HempString string
) {
    /* not strictly correct, but good enough for integer list indexes */
    while (isdigit(*string))
        string++;

    /* if we reached the end of the string then all characters are wordlike */
    return *string
        ? HEMP_FALSE
        : HEMP_TRUE;
}


HEMP_INLINE HempBool
hemp_string_numlike(
    HempString string
) {
    // should use strtod() instead
    hemp_debug_msg("WARNING: hemp_string_numlike() doesn't accept floating point numbers\n");
    
    /* not strictly correct, but good enough for integer list indexes */
    while (isdigit(*string))
        string++;

    /* if we reached the end of the string then all characters are wordlike */
    return *string
        ? HEMP_FALSE
        : HEMP_TRUE;
}


HEMP_INLINE HempString
hemp_string_next_space(
    HempString string
) {
    while (*string && ! isspace(*string))
        string++;

    return *string ? string : NULL;
}


HEMP_INLINE HempBool
hemp_string_to_next_space(
    HempString *string
) {
    HempString space = hemp_string_next_space(*string);

    if (space) {
        *string = space;
        return HEMP_TRUE;
    }
    else {
        return HEMP_FALSE;
    }
}


HEMP_INLINE HempString
hemp_string_next_nonspace(
    HempString string
) {
    while (*string && isspace(*string))
        string++;

    return *string ? string : NULL;
}


HEMP_INLINE HempBool
hemp_string_to_next_nonspace(
    HempString *string
) {
    HempString nonspace = hemp_string_next_nonspace(*string);

    if (nonspace) {
        *string = nonspace;
        return HEMP_TRUE;
    }
    else {
        return HEMP_FALSE;
    }
}


HEMP_INLINE HempString
hemp_string_next_line(
    HempString string
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


HEMP_INLINE HempBool
hemp_string_to_next_line(
    HempString *string
) {
    HempString line = hemp_string_next_line(*string);

    if (line) {
        *string = line;
        return HEMP_TRUE;
    }
    else {
        return HEMP_FALSE;
    }
}


HEMP_INLINE HempList
hemp_string_words(
    HempString string
) {
    return hemp_string_nwords(string, 0);
}


HempList
hemp_string_nwords(
    HempString  string,
    HempSize max
) {
    HempList list = hemp_list_new();
    HempString  from = string, to, item;
    HempSize size = 0;

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


HempLocation
hemp_string_location(
    HempString     string,
    HempString     marker,
    HempLocation   location
) {
    HempString     scan = string;

    if (!location)
        location = hemp_location_new();

    location->position = marker - string;
    location->extract  = string;
    location->line     = 1;

//  hemp_debug_msg("computing of position from %p to %p\n", string, marker);

    while ( (scan = hemp_string_next_line(scan)) && scan < marker ) {
//      hemp_debug_msg("- line %d:\n", str_pos.line);
        location->extract = scan;
        location->line++;
    }
    location->column = marker - location->extract;

//  hemp_debug_msg(
//      "position for string from %p to %p is pos:%ld  line:%ld  col:%ld\n", 
//      string, marker, str_pos.position, str_pos.line, str_pos.column
//  );

    return location;
}



//HEMP_DO_INLINE HempBool
//hemp_string_to_next_line(
//    HempString *string
//) {
//    HempString s = *string;
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


HEMP_OUTPUT(hemp_type_string_text) {
    HempString  str = hemp_val_str(value);
    HempText text;

    hemp_prepare_text_size(context, output, text, strlen(str));

    hemp_text_append_string(text, str);
    return output;
}


HEMP_VALUE(hemp_type_string_number) {
    HempString str = hemp_val_str(value);
    HempString end;
    HempNum nval;
    
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


HEMP_VALUE(hemp_type_string_integer) {
    HempValue nval = hemp_type_string_number(value, context);
    return hemp_int_val((HempInt) hemp_val_num(nval));
}


HEMP_VALUE(hemp_type_string_boolean) {
    HempString str = hemp_val_str(value);
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

HEMP_VALUE(hemp_method_string_length) {
    return hemp_int_val( strlen( hemp_val_str(value) ) );
}

