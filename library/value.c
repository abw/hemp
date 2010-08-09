#include <hemp/value.h>


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
    printf("0x%016llx\n", value);

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

