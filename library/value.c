#include <hemp/value.h>
#include <hemp/text.h>
#include <hemp/hash.h>
#include <hemp/context.h>


const struct hemp_type_s hemp_type_reserved = {
    HEMP_RESERVED_ID, "Reserved",
};

const struct hemp_type_s hemp_type_unused = {
    HEMP_UNUSED_ID, "Unused",
};


/*--------------------------------------------------------------------------
 * WARNING: the order of entries in this vtable is critical and must 
 * correspond to the HEMP_XXXX_ID values defined in value.h.
 *
 * Types 0x04 to 0x0F are reserved for future hemp use.
 * Types 0x11 to 0x1F are available for user-defined types (e.g. alien values)
 *--------------------------------------------------------------------------*/

const struct hemp_type_s *hemp_global_types[32] = {
    &hemp_type_number,      /* 0x00 == 00 */
    &hemp_type_integer,     /* 0x01 == 01 */
    &hemp_type_reserved,    /* 0x02 == 02 */        /* pointer: TODO */
    &hemp_type_string,      /* 0x03 == 03 */
    &hemp_type_text,        /* 0x04 == 04 */
    &hemp_type_reserved,    /* 0x05 == 05 */
    &hemp_type_reserved,    /* 0x06 == 06 */
    &hemp_type_reserved,    /* 0x07 == 07 */
    &hemp_type_reserved,    /* 0x08 == 08 */
    &hemp_type_reserved,    /* 0x09 == 09 */
    &hemp_type_reserved,    /* 0x0A == 10 */
    &hemp_type_reserved,    /* 0x0B == 11 */
    &hemp_type_reserved,    /* 0x0C == 12 */
    &hemp_type_reserved,    /* 0x0D == 13 */
    &hemp_type_reserved,    /* 0x0E == 14 */
    &hemp_type_reserved,    /* 0x0F == 15 */
    
    &hemp_type_identity,    /* 0x10 == 16 */
    &hemp_type_unused,      /* 0x11 == 17 */    /* available for user defined types */
    &hemp_type_unused,      /* 0x12 == 18 */
    &hemp_type_unused,      /* 0x13 == 19 */
    &hemp_type_unused,      /* 0x14 == 20 */
    &hemp_type_unused,      /* 0x15 == 21 */
    &hemp_type_unused,      /* 0x16 == 22 */
    &hemp_type_unused,      /* 0x17 == 23 */
    &hemp_type_unused,      /* 0x18 == 24 */
    &hemp_type_unused,      /* 0x19 == 25 */
    &hemp_type_unused,      /* 0x1A == 26 */
    &hemp_type_unused,      /* 0x1B == 27 */
    &hemp_type_unused,      /* 0x1C == 28 */
    &hemp_type_unused,      /* 0x1D == 29 */
    &hemp_type_unused,      /* 0x1E == 30 */
    &hemp_type_unused,      /* 0x1F == 31 */
};

const hemp_value_t  HempMissing = HEMP_IDENT_MAKE(HEMP_IDENT_MISSING);
const hemp_value_t  HempNothing = HEMP_IDENT_MAKE(HEMP_IDENT_NOTHING);
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
hemp_str_val(hemp_cstr_p s) {
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


HEMP_INLINE hemp_cstr_p
hemp_val_str(hemp_value_t v) {
    return (hemp_cstr_p) HEMP_POINTER(v);
}


HEMP_INLINE hemp_text_p
hemp_val_text(hemp_value_t v) {
    return (hemp_text_p) HEMP_POINTER(v);
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


HEMP_VALUE_FUNC(hemp_value_cannot_compare) {
    HEMP_CONVERT_ERROR(
        context, 
        hemp_type_name(value),
        HEMP_STR_COMPARE,
        HEMP_STR_BLANK
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
    hemp_u64_t  mask  = (hemp_u64_t) 1 << 63;
    hemp_u64_t  bit;
    hemp_int_t  n = 1;
    hemp_cstr_p col;
    printf("0x%016llx : ", value);

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

