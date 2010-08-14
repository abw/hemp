#include <hemp/text.h>


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
    const hemp_cstr_p format,
    ...
) {
    hemp_text_p text = hemp_text_new();
    hemp_cstr_p cstr;

    va_list args;
    va_start(args, format);
    vasprintf(&cstr, format, args);
    va_end(args);

    if (! cstr)
        hemp_mem_fail("text");

#if HEMP_DEBUG & HEMP_DEBUG_MEM
    /* tell the memory trace debugging code that we know about this memory */
    hemp_mem_trace_external(cstr, strlen(cstr) + 1, __FILE__, __LINE__);
#endif

    text->length   = 
    text->capacity = strlen(cstr);
    text->string   = cstr;

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


hemp_text_p
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
hemp_text_from_cstr(
    hemp_cstr_p source
) {
    hemp_text_p text = hemp_text_init(strlen(source));
    hemp_text_append_cstr(text, source);
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
    text->string[length + 1] = '\0';
    text->length = length;

    return text;
}


hemp_text_p 
hemp_text_append_cstrn(
    hemp_text_p text, 
    hemp_cstr_p append, 
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
hemp_text_insert_cstr(
    hemp_text_p     text, 
    hemp_offset_t   offset, 
    hemp_cstr_p     insert
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
        return hemp_text_append_cstr(text, insert);

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


