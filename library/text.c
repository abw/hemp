#include "hemp/text.h"
#include "hemp/debug.h"


hemp_text_t
hemp_text_init(
    hemp_size_t size
) {
    hemp_text_t text;

    if ((text = (hemp_text_t) hemp_mem_init(sizeof(struct hemp_text)))) {
        if (size) {
            if ((text->string = hemp_mem_init(size + 1))) {
                debug_mem("allocated string memory at %p\n", text->string);
                text->capacity = size;
                text->length   = 0;
            }
            else {
                hemp_text_null(text);
                text->capacity = text->length = 0;
            }
        }
        else {
            text->string   = NULL;
            text->capacity = text->length = 0;
        }
    }
    
    // TODO: handle null text

    return text;
}


void
hemp_text_free(
    hemp_text_t text
) {
    if (text->string)
        hemp_mem_free(text->string);
    hemp_mem_free(text);
}


hemp_text_t
hemp_text_capacity(
    hemp_text_t text, 
    hemp_size_t length
) {
    /* increment string length by 1 to account for terminating '\0' */
    if (text->capacity < ++length) {
        text->string = text->string
            ? hemp_mem_size(text->string, length)  
            : hemp_mem_init(length);
        if (text->string) {
            debug_mem("allocated %d bytes of string memory at %p\n", length, text->string);
            text->capacity = length - 1;
        }
        else {
            hemp_text_null(text);
            hemp_fatal("failed to increase string to length %d\n", length);
        }
    }
    
    return text;
}


hemp_text_t
hemp_text_from_text(
    hemp_text_t source
) {
    hemp_text_t text = hemp_text_init(source->length);
    hemp_text_append_text(text, source);
    return text;
}


hemp_text_t
hemp_text_from_cstr(
    hemp_cstr_t source
) {
    hemp_text_t text = hemp_text_init(strlen(source));
    hemp_text_append_cstr(text, source);
    return text;
}


hemp_text_t
hemp_text_append_text(
    hemp_text_t text, 
    hemp_text_t append
) {
    hemp_size_t length = text->length + append->length;
    
    if ((text = hemp_text_capacity(text, length))) {
        strncpy(text->string + text->length, append->string, append->length);
        text->string[length + 1] = '\0';
        text->length = length;
    }
    return text;
}


hemp_text_t 
hemp_text_append_cstrn(
    hemp_text_t text, 
    hemp_cstr_t append, 
    hemp_size_t length
) {
    hemp_size_t capacity = text->length + length;

    if ((text = hemp_text_capacity(text, capacity))) {
        strncpy(text->string + text->length, append, length);
        text->string[capacity] = '\0';
        text->length = capacity;
    }
    else {
        hemp_fatal("failed to increase text capacity to %d\n", length);
    }
    return text;
}


hemp_text_t 
hemp_text_insert_cstr(
    hemp_text_t     text, 
    hemp_offset_t   offset, 
    hemp_cstr_t     insert
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

    // debug("offset %d  length %d  extra %d  length %d\n", offset, text->length, extra, length);
    
    if ((text = hemp_text_capacity(text, length))) {
        /* shift down contents of existing string from offset onwards */
        hemp_mem_copy(text->string + offset, text->string + offset + extra, text->length - offset);
        /* insert new string at offset */
        strncpy(text->string + offset, insert, extra);
        text->string[length] = '\0';
        text->length = length;
    }
    return text;
}


