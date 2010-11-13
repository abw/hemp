#include <hemp/value.h>
#include <hemp/context.h>
#include <hemp/type/text.h>
#include <hemp/type/hash.h>


const hemp_value_t  HempMissing = HEMP_IDENT_MAKE(HEMP_IDENT_MISSING);
const hemp_value_t  HempNothing = HEMP_IDENT_MAKE(HEMP_IDENT_NOTHING);
const hemp_value_t  HempEmpty   = HEMP_IDENT_MAKE(HEMP_IDENT_EMPTY);
const hemp_value_t  HempFalse   = HEMP_IDENT_MAKE(HEMP_IDENT_FALSE);
const hemp_value_t  HempTrue    = HEMP_IDENT_MAKE(HEMP_IDENT_TRUE);
const hemp_value_t  HempBefore  = HEMP_IDENT_MAKE(HEMP_IDENT_BEFORE);
const hemp_value_t  HempAfter   = HEMP_IDENT_MAKE(HEMP_IDENT_AFTER);
const hemp_value_t  HempEqual   = HEMP_IDENT_MAKE(HEMP_IDENT_EQUAL);


/*--------------------------------------------------------------------------
 * inline functions to encode native values as tagged values
 *--------------------------------------------------------------------------*/

HEMP_INLINE hemp_value_t
hemp_num_val(hemp_num_t n) {
    hemp_value_t v;
    v.number = n;
    return v;
}


HEMP_INLINE hemp_value_t
hemp_int_val(hemp_int_t i) {
    hemp_value_t v;
    v.bits = HEMP_INTEGER_TAG | ((hemp_u64_t) i & HEMP_INTEGER_MASK);
    return v;
}


HEMP_INLINE hemp_value_t
hemp_ptr_val(hemp_mem_p p) {
    hemp_value_t v;
    v.bits = HEMP_POINTER_TAG | ((hemp_u64_t) p & HEMP_POINTER_MASK);
    return v;
}


HEMP_INLINE hemp_value_t
hemp_str_val(hemp_str_p s) {
    hemp_value_t v;
    v.bits = HEMP_STRING_TAG | ((hemp_u64_t) s & HEMP_POINTER_MASK);
    return v;
}


HEMP_INLINE hemp_value_t
hemp_text_val(hemp_text_p t) {
    hemp_value_t v;
    v.bits = HEMP_TEXT_TAG | ((hemp_u64_t) t & HEMP_POINTER_MASK);
    return v;
}


HEMP_INLINE hemp_value_t
hemp_list_val(hemp_list_p t) {
    hemp_value_t v;
    v.bits = HEMP_LIST_TAG | ((hemp_u64_t) t & HEMP_POINTER_MASK);
    return v;
}


HEMP_INLINE hemp_value_t
hemp_hash_val(hemp_hash_p t) {
    hemp_value_t v;
    v.bits = HEMP_HASH_TAG | ((hemp_u64_t) t & HEMP_POINTER_MASK);
    return v;
}


HEMP_INLINE hemp_value_t
hemp_obj_val(hemp_object_p t) {
    hemp_value_t v;
    v.bits = HEMP_OBJECT_TAG | ((hemp_u64_t) t & HEMP_POINTER_MASK);
    return v;
}


HEMP_INLINE hemp_value_t
hemp_ident_val(hemp_u8_t i) {
    hemp_value_t v;
    v.bits = HEMP_IDENTITY_TAG | ((hemp_u64_t) i & HEMP_IDENT_MASK);
    return v;
}


HEMP_INLINE hemp_value_t
hemp_bool_val(hemp_bool_t b) {
    return b
        ? HempTrue
        : HempFalse;
}


HEMP_INLINE hemp_value_t
hemp_type_val(
    hemp_type_p type, 
    hemp_mem_p  ptr
) {
    hemp_value_t v;
    v.bits = HEMP_TAG_MAKE(type->id) | ((hemp_u64_t) ptr & HEMP_POINTER_MASK);
    return v;
}


/*--------------------------------------------------------------------------
 * inline functions to decode tagged values to native values
 *--------------------------------------------------------------------------*/

HEMP_INLINE hemp_num_t
hemp_val_num(hemp_value_t v) {
    return v.number;
}


HEMP_INLINE hemp_int_t
hemp_val_int(hemp_value_t v) {
    return (hemp_int_t)(v.bits & HEMP_INTEGER_MASK);
}


HEMP_INLINE hemp_mem_p
hemp_val_ptr(hemp_value_t v) {
    return (hemp_mem_p) HEMP_POINTER(v);
}


HEMP_INLINE hemp_str_p
hemp_val_str(hemp_value_t v) {
    return (hemp_str_p) HEMP_POINTER(v);
}


HEMP_INLINE hemp_text_p
hemp_val_text(hemp_value_t v) {
    return (hemp_text_p) HEMP_POINTER(v);
}


HEMP_INLINE hemp_list_p
hemp_val_list(hemp_value_t v) {
    return (hemp_list_p) HEMP_POINTER(v);
}


HEMP_INLINE hemp_hash_p
hemp_val_hash(hemp_value_t v) {
    return (hemp_hash_p) HEMP_POINTER(v);
}


HEMP_INLINE hemp_object_p
hemp_val_obj(hemp_value_t v) {
    return (hemp_object_p) HEMP_POINTER(v);
}


HEMP_INLINE hemp_bool_t
hemp_val_bool(hemp_value_t v) {
    return hemp_is_true(v)
        ? HEMP_TRUE
        : HEMP_FALSE;
}





/*--------------------------------------------------------------------------
 * Generic purpose conversion, literal and no-op functions, most of which are 
 * syntactic sugar so that our type vtables are more self-documenting.
 *--------------------------------------------------------------------------*/

HEMP_VALUE_FUNC(hemp_value_no_op) {
    return value;
}


HEMP_VALUE_FUNC(hemp_value_self) {
    return value;
}


HEMP_VALUE_FUNC(hemp_value_true) {
    return HempTrue;
}


HEMP_VALUE_FUNC(hemp_value_false) {
    return HempFalse;
}


HEMP_VALUES_FUNC(hemp_value_values) {
    hemp_debug_call("hemp_value_values()\n");
    hemp_list_p values;
    hemp_prepare_values(context, output, values);
    hemp_list_push(values, value);
//    hemp_debug("hemp_value_values() added value to list, now has %d items\n", values->length);
    return output;
}


HEMP_FETCH_FUNC(hemp_value_dot) {
    hemp_debug_call("hemp_value_dot(%s)\n", hemp_type_name(container));
    hemp_debug("hemp_value_dot(%s)\n", hemp_type_name(container));
    hemp_value_t result = HempMissing;

    /* call the value's fetch method, if it has one */
    hemp_type_p type = hemp_type(container);

    if (type->fetch && type->fetch != &hemp_value_not_fetch) {
        result = type->fetch(container, context, key);
        if (hemp_is_found(result))
            return result;
    }

    /* if we didn't find anything then look for a method */
    if (hemp_is_text(key)) {
        /* we got a text key */
        result = hemp_send(container, hemp_val_text(key)->string, context);
    }
    else {
        /* otherwise we have to convert the key to text */
        hemp_text_p ktext = hemp_text_new_size(16);
        hemp_onto_text(key, context, hemp_text_val(ktext));
        result = hemp_send(container, ktext->string, context);
        hemp_text_free(ktext);
    }

    return result;
}



/*--------------------------------------------------------------------------
 * Default "I'm sorry Dave, I'm afraid I can't do that" functions
 *--------------------------------------------------------------------------*/

HEMP_VTEXT_FUNC(hemp_value_not_text) {
    HEMP_CONVERT_ERROR(
        context, 
        hemp_type_name(value),
        HEMP_STR_TEXT,
        HEMP_STR_BLANK
    );
}


HEMP_VALUE_FUNC(hemp_value_not_number) {
    HEMP_CONVERT_ERROR(
        context, 
        hemp_type_name(value),
        HEMP_STR_NUMBER,
        HEMP_STR_BLANK
    );
}


HEMP_VALUE_FUNC(hemp_value_not_integer) {
    HEMP_CONVERT_ERROR(
        context, 
        hemp_type_name(value),
        HEMP_STR_INTEGER,
        HEMP_STR_BLANK
    );
}


HEMP_VALUE_FUNC(hemp_value_not_boolean) {
    HEMP_CONVERT_ERROR(
        context, 
        hemp_type_name(value),
        HEMP_STR_BOOLEAN,
        HEMP_STR_BLANK
    );
}


HEMP_VALUE_FUNC(hemp_value_not_compare) {
    HEMP_CONVERT_ERROR(
        context, 
        hemp_type_name(value),
        HEMP_STR_COMPARE,
        HEMP_STR_BLANK
    );
}


HEMP_VALUE_FUNC(hemp_value_not_defined) {
    HEMP_CONVERT_ERROR(
        context, 
        hemp_type_name(value),
        HEMP_STR_DEFINED,
        HEMP_STR_BLANK
    );
}


HEMP_FETCH_FUNC(hemp_value_not_fetch) {
    HEMP_FETCH_ERROR(
        context, 
        hemp_type_name(container),
        "Cannot fetch"
    );
}


HEMP_STORE_FUNC(hemp_value_not_store) {
    HEMP_STORE_ERROR(
        context, 
        hemp_type_name(container),
        "Cannot store"
    );
}


HEMP_FETCH_FUNC(hemp_value_not_dot) {
    HEMP_FETCH_ERROR(
        context, 
        hemp_type_name(container),
        "Cannot dot"
    );
}



/*--------------------------------------------------------------------------
 * debugging
 *--------------------------------------------------------------------------*/

void hemp_dump_value(
    hemp_value_t value
) {
    hemp_dump_u64(value.bits);
}


void hemp_dump_u64(
    hemp_u64_t  value
) {
    hemp_u64_t mask = (hemp_u64_t) 1 << 63;
    hemp_u64_t bit;
    hemp_int_t n = 1;
    hemp_str_p col;
    printf("0x%016llx : ", value);
    printf("DUMPING....\n");

    while (mask) {
        bit = value & mask;
        mask = mask >> 1;
        if (n == 1) {
            col = HEMP_ANSI_MAGENTA;
        }
        else if (n < 13) {
            col = HEMP_ANSI_YELLOW;
        }
//        else if (n < 14) {
//            col = HEMP_ANSI_MAGENTA;
//        }
        else if (n < 18) {
            col = HEMP_ANSI_BLUE;
        }
        else {
            col = HEMP_ANSI_CYAN;
        }
        printf("%s%c", col, bit ? '1' : '0');
        if (n % 4 == 0)
            printf(" ");
        n++;
    }
    printf(HEMP_ANSI_RESET "\n");
//    hemp_dump_32((hemp_u32_t)(value >> 32));
//    hemp_dump_32((hemp_u32_t)(hemp_u64_t) value & 0xFFFFFFFFL);
}

void hemp_dump_64(
    hemp_u64_t value
) {
    hemp_u32_t top = value >> 32;
    hemp_u32_t bot = value & 0xffffffffL;
    printf("0x%016llx\n", value);
//    printf("TOP: 0x%016llx\n", top);
    hemp_dump_32((hemp_u32_t) top);
    hemp_dump_32((hemp_u32_t) bot);
    printf("\n");
}

void hemp_dump_32(
    hemp_u32_t value
) {
    hemp_u32_t  mask  = (hemp_u32_t) 1 << 31;
    hemp_u32_t bit;
    printf("0x%08x : ", value);

    while (mask) {
        bit = value & mask;
        mask = mask >> 1;
        printf("%c", bit ? '1' : '0');
    }
    printf("\n");
}

