#include <hemp/text.h>
#include <hemp/value.h>
#include <hemp/context.h>


HEMP_TYPE_FUNC(hemp_type_text) {
    hemp_type_p type = hemp_type_subtype(HempValue, id, name);
    type->text       = &hemp_value_text_text;       /* return/append text   */
    type->number     = &hemp_value_text_number;     /* text -> number       */
    type->integer    = &hemp_value_text_integer;    /* text -> integer      */
    type->boolean    = &hemp_value_text_boolean;    /* text -> boolean      */
    type->compare    = &hemp_value_not_compare;     /* can't compare        */
    type->defined    = &hemp_value_true;            /* always defined       */
    return type;
};



/*--------------------------------------------------------------------------
 * General purpose text functions.
 *--------------------------------------------------------------------------*/

HEMP_INLINE hemp_text_p
hemp_text_new( ) {
    hemp_text_p text = (hemp_text_p) hemp_mem_alloc(
        sizeof(struct hemp_text_s)
    );
    
    /* TODO: allocate both header and body in one block */
    if (! text)
        hemp_mem_fail("text");

    text->capacity = 
    text->length   = 0;
    text->string   = NULL;

    return text;
}


hemp_text_p
hemp_text_init(
    hemp_size_t size
) {
    hemp_text_p text = hemp_text_new();

    if (size) {
        text->string = hemp_mem_alloc(size + 1);
        
        if (! text->string)
            hemp_mem_fail("text string");

        text->capacity = size;
        text->length   = 0;
    }

    return text;
}


hemp_text_p
hemp_text_init_format(
    const hemp_str_p format,
    ...
) {
    hemp_text_p text = hemp_text_new();
    hemp_str_p  string;

    va_list args;
    va_start(args, format);
    vasprintf(&string, format, args);
    va_end(args);

    if (! string)
        hemp_mem_fail("text");

#if HEMP_DEBUG & HEMP_DEBUG_MEM
    /* tell the memory trace debugging code that we know about this memory */
    hemp_mem_trace_external(string, strlen(string) + 1, __FILE__, __LINE__);
#endif

    text->length   = 
    text->capacity = strlen(string);
    text->string   = string;

    return text;
}


void
hemp_text_free(
    hemp_text_p text
) {
    if (text->string)
        hemp_mem_free(text->string);

    hemp_mem_free(text);
}


HEMP_INLINE hemp_text_p
hemp_text_capacity(
    hemp_text_p text, 
    hemp_size_t length
) {
    /* increment string length by 1 to account for terminating '\0' */
    if (text->capacity < ++length) {
        text->string = text->string
            ? hemp_mem_resize(text->string, length)  
            : hemp_mem_alloc(length);

        if (! text->string)
            hemp_mem_fail("text string");

        text->capacity = length - 1;
    }
    
    return text;
}


hemp_text_p
hemp_text_from_text(
    hemp_text_p source
) {
    hemp_text_p text = hemp_text_init(source->length);
    hemp_text_append_text(text, source);
    return text;
}


hemp_text_p
hemp_text_from_string(
    hemp_str_p source
) {
    hemp_text_p text = hemp_text_init(strlen(source));
    hemp_text_append_string(text, source);
    return text;
}



hemp_text_p
hemp_text_append_text(
    hemp_text_p text, 
    hemp_text_p append
) {
    hemp_size_t length = text->length + append->length;
    
    text = hemp_text_capacity(text, length);
    strncpy(text->string + text->length, append->string, append->length);
    text->length = length;
    *(text->string + length) = HEMP_NUL;

    return text;
}


hemp_text_p 
hemp_text_append_stringn(
    hemp_text_p text, 
    hemp_str_p  append, 
    hemp_size_t length
) {
    hemp_size_t capacity = text->length + length;
    text = hemp_text_capacity(text, capacity);
    strncpy(text->string + text->length, append, length);
    text->string[capacity] = '\0';
    text->length = capacity;
    return text;
}


hemp_text_p
hemp_text_insert_string(
    hemp_text_p     text, 
    hemp_offset_t   offset, 
    hemp_str_p      insert
) {
    hemp_size_t length = text->length;
    hemp_size_t extra;

    /* negative offset counts back from the end of the text */
    if (offset < 0) {
        offset += length;
        if (offset < 0)
            offset = 0;
    }
    else if (offset > length || length == 0)
        return hemp_text_append_string(text, insert);

    extra = strlen(insert);
    length += extra;

    // hemp_debug("offset %d  length %d  extra %d  length %d\n", offset, text->length, extra, length);
    
    text = hemp_text_capacity(text, length);
    /* shift down contents of existing string from offset onwards */
    hemp_mem_copy(text->string + offset, text->string + offset + extra, text->length - offset);
    /* insert new string at offset */
    strncpy(text->string + offset, insert, extra);
    text->string[length] = '\0';
    text->length = length;

    return text;
}


hemp_text_p
hemp_text_replace_string(
    hemp_text_p     text, 
    hemp_str_p      replace
) {
    hemp_size_t length = strlen(replace);
    hemp_text_capacity(text, length);
    strcpy(text->string, replace);
    text->string[length + 1] = HEMP_NUL;
    text->length = length;
    return text;
}



/*--------------------------------------------------------------------------
 * Runtime text evaluation methods
 *--------------------------------------------------------------------------*/


HEMP_VTEXT_FUNC(hemp_value_text_text) {
     hemp_debug_call("hemp_value_text_text\n");

    /* FIXME: can we safely return value?  Should we return a copy?  What 
     * happens when the caller is done and tries to free it?  Ref counts, 
     * here we come.... :-(
     */

    /* if we haven't been passed an output buffer then we return self */
    if (hemp_is_undef(output))
        return value;

    /* if we have been passed an output buffer then we append the value
     * text onto the end of it 
     */
//    hemp_text_p text = hemp_val_text(output);
    hemp_text_append_text(hemp_val_text(output), hemp_val_text(value));
    return output;
}


HEMP_VALUE_FUNC(hemp_value_text_number) {
    hemp_text_p text = hemp_val_text(value);
    hemp_str_p  end;
    hemp_num_t  nval;
    
    if (! text->length) {
        HEMP_CONVERT_ERROR(
            context, 
            HEMP_STR_NO_TEXT,
            HEMP_STR_NUMBER,
            HEMP_STR_BLANK
        );
    }

    errno = 0;
    nval  = strtod(text->string, &end);
    
    if (*end || (errno == EINVAL)) {
        HEMP_CONVERT_ERROR(
            context, 
            HEMP_STR_TEXT, 
            HEMP_STR_NUMBER,
            text->string
        );
    }
    else if (errno == ERANGE) {
        HEMP_OVERFLOW_ERROR(
            context, 
            text->string
        );
    }
    else {
        return hemp_num_val(nval);
    }

    return hemp_num_val(0);
}


HEMP_VALUE_FUNC(hemp_value_text_integer) {
    hemp_value_t nval = hemp_value_text_number(value, context);
    return hemp_int_val((hemp_int_t) hemp_val_num(nval));
}


HEMP_VALUE_FUNC(hemp_value_text_boolean) {
    /* hmmm... must be careful here... I think the best approach is to say 
     * that any non-zero length string is true, but unlike Perl, we won't 
     * perform any implicit text->number conversion.  The end result is that 
     * things like "0" and "false" (both strings) are true values, while 0 
     * (integer zero), 0.0 (float zero), "" (empty string) and HempFalse 
     * (explicit false) are all false.
     */
    return hemp_val_text(value)->length
        ? HempTrue
        : HempFalse;
}

