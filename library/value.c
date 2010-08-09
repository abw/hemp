#include <hemp/value.h>


const struct hemp_vtype_s hemp_global_vtypes[] = {
    { 0x0, "number"  },
    { 0x1, "integer" },
    { 0x2, "string"  }
};

const hemp_value_t 
HempMissing = (hemp_value_t) 
    ((hemp_u64_t) HEMP_TYPE_IDENT_MASK | HEMP_IDENT_MISSING_ID);

const hemp_value_t 
HempEmpty = (hemp_value_t) 
    ((hemp_u64_t) HEMP_TYPE_IDENT_MASK | HEMP_IDENT_EMPTY_ID);

const hemp_value_t 
HempFalse = (hemp_value_t) 
    ((hemp_u64_t) HEMP_TYPE_IDENT_MASK | HEMP_IDENT_FALSE_ID);

const hemp_value_t 
HempTrue = (hemp_value_t) 
    ((hemp_u64_t) HEMP_TYPE_IDENT_MASK | HEMP_IDENT_TRUE_ID);


/*--------------------------------------------------------------------------
 * inline functions to encode native values as tagged values
 *--------------------------------------------------------------------------*/

HEMP_DO_INLINE hemp_value_t
HEMP_NUM_VAL(hemp_num_t n) {
    hemp_value_t v;
    v.number = n;
    return v;
}

HEMP_DO_INLINE hemp_value_t
HEMP_INT_VAL(hemp_int_t i) {
    hemp_value_t v;
    v.bits = HEMP_TYPE_INT_MASK | (hemp_u64_t) i;
    return v;
}

HEMP_DO_INLINE hemp_value_t
HEMP_STR_VAL(hemp_cstr_p s) {
    hemp_value_t v;
    v.bits = HEMP_TYPE_STR_MASK | (hemp_u64_t) s;
    return v;
}

HEMP_DO_INLINE hemp_value_t
HEMP_IDENT_VAL(hemp_u8_t i) {
    hemp_value_t v;
    v.bits = HEMP_TYPE_IDENT_MASK | (hemp_u64_t) i;
    return v;
}


/*--------------------------------------------------------------------------
 * inline functions to decode tagged values to native values
 *--------------------------------------------------------------------------*/

HEMP_DO_INLINE hemp_num_t
HEMP_VAL_NUM(hemp_value_t v) {
    return v.number;
}

HEMP_DO_INLINE hemp_int_t
HEMP_VAL_INT(hemp_value_t v) {
    return (hemp_int_t) v.bits;
}

HEMP_DO_INLINE hemp_cstr_p
HEMP_VAL_STR(hemp_value_t v) {
    return (hemp_cstr_p) HEMP_PAYLOAD(v);
}

//HEMP_DO_INLINE hemp_u8_t
//HEMP_VAL_IDENT(hemp_value_t v) {
//    return HEMP_IDENT_ID(v);
//}


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

