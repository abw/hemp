#include <hemp/value.h>


const struct hemp_vtype_s hemp_global_vtypes[16] = {
    { 0x0, "Number"          },
    { 0x1, "Integer"         },
    { 0x2, "String"          },
    { 0x3, "-- RESERVED 0x3" },
    { 0x4, "Text"            },
    { 0x5, "-- RESERVED 0x5" },
    { 0x6, "-- RESERVED 0x6" },
    { 0x7, "-- RESERVED 0x7" },
    { 0x8, "-- RESERVED 0x8" },
    { 0x9, "-- RESERVED 0x9" },
    { 0xA, "-- RESERVED 0xA" },
    { 0xB, "-- RESERVED 0xB" },
    { 0xC, "-- RESERVED 0xC" },
    { 0xD, "-- RESERVED 0xD" },
    { 0xE, "-- RESERVED 0xD" },
    { 0xF, "Identity"        }
};

const hemp_value_t 
HempMissing = (hemp_value_t) 
    ((hemp_u64_t) HEMP_TYPE_IDENT_MASK | HEMP_IDENT_MISSING_ID);

const hemp_value_t 
HempNothing = (hemp_value_t) 
    ((hemp_u64_t) HEMP_TYPE_IDENT_MASK | HEMP_IDENT_NOTHING_ID);

const hemp_value_t 
HempFalse = (hemp_value_t) 
    ((hemp_u64_t) HEMP_TYPE_IDENT_MASK | HEMP_IDENT_FALSE_ID);

const hemp_value_t 
HempTrue = (hemp_value_t) 
    ((hemp_u64_t) HEMP_TYPE_IDENT_MASK | HEMP_IDENT_TRUE_ID);

const hemp_value_t 
HempBefore = (hemp_value_t) 
    ((hemp_u64_t) HEMP_TYPE_IDENT_MASK | HEMP_IDENT_BEFORE_ID);

const hemp_value_t 
HempEqual = (hemp_value_t) 
    ((hemp_u64_t) HEMP_TYPE_IDENT_MASK | HEMP_IDENT_EQUAL_ID);

const hemp_value_t 
HempAfter = (hemp_value_t) 
    ((hemp_u64_t) HEMP_TYPE_IDENT_MASK | HEMP_IDENT_AFTER_ID);


/*--------------------------------------------------------------------------
 * return the name of special identity values
 *--------------------------------------------------------------------------*/

HEMP_DO_INLINE hemp_cstr_p
hemp_identity_name(
    hemp_int_t id
) {
    switch (id) {
        case HEMP_IDENT_MISSING_ID: return HEMP_STR_MISSING;
        case HEMP_IDENT_NOTHING_ID: return HEMP_STR_NOTHING;
        case HEMP_IDENT_FALSE_ID:   return HEMP_STR_FALSE;
        case HEMP_IDENT_TRUE_ID:    return HEMP_STR_TRUE;
        case HEMP_IDENT_EQUAL_ID:   return HEMP_STR_EQUAL;
        case HEMP_IDENT_BEFORE_ID:  return HEMP_STR_BEFORE;
        case HEMP_IDENT_AFTER_ID:   return HEMP_STR_AFTER;
        default:                    return HEMP_STR_UNKNOWN;
    }
}


/*--------------------------------------------------------------------------
 * inline functions to encode native values as tagged values
 *--------------------------------------------------------------------------*/

HEMP_DO_INLINE hemp_value_t
hemp_num_val(hemp_num_t n) {
    hemp_value_t v;
    v.number = n;
    return v;
}

HEMP_DO_INLINE hemp_value_t
hemp_int_val(hemp_int_t i) {
    hemp_value_t v;
    v.bits = HEMP_TYPE_INT_MASK | (hemp_u64_t) i;
    return v;
}

HEMP_DO_INLINE hemp_value_t
hemp_str_val(hemp_cstr_p s) {
    hemp_value_t v;
    v.bits = HEMP_TYPE_STR_MASK | (hemp_u64_t) s;
    return v;
}

HEMP_DO_INLINE hemp_value_t
hemp_text_val(hemp_text_p t) {
    hemp_value_t v;
    v.bits = HEMP_TYPE_TEXT_MASK | (hemp_u64_t) t;
    return v;
}

HEMP_DO_INLINE hemp_value_t
hemp_ident_val(hemp_u8_t i) {
    hemp_value_t v;
    v.bits = HEMP_TYPE_IDENT_MASK | (hemp_u64_t) i;
    return v;
}

HEMP_DO_INLINE hemp_value_t
hemp_bool_val(hemp_bool_t b) {
    return b
        ? HempTrue
        : HempFalse;
}


/*--------------------------------------------------------------------------
 * inline functions to decode tagged values to native values
 *--------------------------------------------------------------------------*/

HEMP_DO_INLINE hemp_num_t
hemp_val_num(hemp_value_t v) {
    return v.number;
}

HEMP_DO_INLINE hemp_int_t
hemp_val_int(hemp_value_t v) {
    return (hemp_int_t) v.bits;
}

HEMP_DO_INLINE hemp_cstr_p
hemp_val_str(hemp_value_t v) {
    return (hemp_cstr_p) HEMP_PAYLOAD(v);
}

HEMP_DO_INLINE hemp_text_p
hemp_val_text(hemp_value_t v) {
    return (hemp_text_p) HEMP_PAYLOAD(v);
}

//HEMP_DO_INLINE hemp_u8_t
//hemp_val_ident(hemp_value_t v) {
//    return HEMP_IDENT_ID(v);
//}

HEMP_DO_INLINE hemp_bool_t
hemp_val_bool(hemp_value_t v) {
    if (HEMP_IS_BOOLEAN(v)) {
        return HEMP_IS_TRUE(v);
    }
    else {
        hemp_fatal("value is not boolean"); 
        return HEMP_FALSE;
    }
    
//    return HEMP_IS_BOOLEAN(v)
//         ? HEMP_IS_TRUE(v)
//         : ({ hemp_fatal("value is not boolean"); 0 }); // shitfucks. no hemp 
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
            col = ANSI_MAGENTA;
        }
        else if (n < 13) {
            col = ANSI_YELLOW;
        }
        else if (n < 14) {
            col = ANSI_MAGENTA;
        }
        else if (n < 18) {
            col = ANSI_BLUE;
        }
        else {
            col = ANSI_CYAN;
        }
        printf("%s%c", col, bit ? '1' : '0');
        if (n % 4 == 0)
            printf(" ");
        n++;
    }
    printf(ANSI_RESET "\n");
//    hemp_dump_32((hemp_u32_t)(value >> 32));
//    hemp_dump_32((hemp_u32_t)(hemp_u64_t) value & 0xFFFFFFFFL);
}

void hemp_dump_64(
    hemp_u64_t value
) {
    hemp_u64_t top = value & 0xffffffff00000000LL;
    top = top >> 32;
    hemp_u64_t bot = value & 0xffffffffL;
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

