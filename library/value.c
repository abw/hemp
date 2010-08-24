#include <hemp/value.h>
#include <hemp/text.h>
#include <hemp/hash.h>
#include <hemp/context.h>


const struct hemp_vtype_s hemp_global_vtypes[32] = {
    {   0x00, "Number",
        &hemp_value_number_text,
        &hemp_value_no_op,
        &hemp_value_number_integer,
        &hemp_value_number_boolean,
        &hemp_value_number_compare
    },
    {   0x01, "Integer",
        &hemp_value_integer_text,
        &hemp_value_integer_number,
        &hemp_value_no_op,
        &hemp_value_integer_boolean,
        &hemp_value_integer_compare
    },
    {   0x02, "String"            },
    {   0x03, "-- RESERVED 0x03"  },
    {   0x04, "Text"              },
    {   0x05, "-- RESERVED 0x05"  },
    {   0x06, "-- RESERVED 0x06"  },
    {   0x07, "-- RESERVED 0x07"  },
    {   0x08, "-- RESERVED 0x08"  },
    {   0x09, "-- RESERVED 0x09"  },
    {   0x0A, "-- RESERVED 0x0A"  },
    {   0x0B, "-- RESERVED 0x0B"  },
    {   0x0C, "-- RESERVED 0x0C"  },
    {   0x0D, "-- RESERVED 0x0D"  },
    {   0x0E, "-- RESERVED 0x0D"  },
    {   0x0F, "-- RESERVED 0x0F"  },
    {   0x10, "Identity",
        &hemp_value_identity_text,
        &hemp_value_identity_number,
        &hemp_value_identity_integer,
        &hemp_value_identity_defined,
        &hemp_value_identity_boolean,
        &hemp_value_identity_compare
    },
    {   0x11, "-- RESERVED 0x11"  },
    {   0x12, "-- RESERVED 0x12"  },
    {   0x13, "-- RESERVED 0x13"  },
    {   0x14, "-- RESERVED 0x14"  },
    {   0x15, "-- RESERVED 0x15"  },
    {   0x16, "-- RESERVED 0x16"  },
    {   0x17, "-- RESERVED 0x17"  },
    {   0x18, "-- RESERVED 0x18"  },
    {   0x19, "-- RESERVED 0x19"  },
    {   0x1A, "-- RESERVED 0x1A"  },
    {   0x1B, "-- RESERVED 0x1B"  },
    {   0x1C, "-- RESERVED 0x1C"  },
    {   0x1D, "-- RESERVED 0x1D"  },
    {   0x1E, "-- RESERVED 0x1E"  },
    {   0x1F, "-- RESERVED 0x1F"  }
//    {   0x20, "Overload"          }
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
    v.bits = HEMP_POINTER_TAG | ((hemp_u64_t) s & HEMP_POINTER_MASK);
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
 * generic conversion functions
 *--------------------------------------------------------------------------*/

HEMP_VALUE_FUNC(hemp_value_no_op) {
    return value;
}


HEMP_VALUE_FUNC(hemp_value_defined) {
    return HempTrue;
}


HEMP_VALUE_FUNC(hemp_value_undefined) {
    return HempFalse;
}


/*--------------------------------------------------------------------------
 * number -> xxx conversion
 *--------------------------------------------------------------------------*/

HEMP_VTEXT_FUNC(hemp_value_number_text) {
    static hemp_char_t buffer[HEMP_BUFFER_SIZE];
    hemp_text_p text;

    snprintf(buffer, HEMP_BUFFER_SIZE, HEMP_FMT_NUM, hemp_val_num(value));
    hemp_prepare_output(output, text, strlen(buffer));
    hemp_text_append_cstr(text, buffer);

    return output;
}


HEMP_VALUE_FUNC(hemp_value_number_integer) {
    return hemp_int_val((hemp_int_t) hemp_val_num(value));
}


HEMP_VALUE_FUNC(hemp_value_number_boolean) {
    /* TODO: decide if this is the right thing to do */
    return hemp_val_num(value) == 0.0
        ? HempFalse
        : HempTrue;
}


HEMP_VALUE_FUNC(hemp_value_number_compare) {
    hemp_num_t cmp = hemp_val_num(value);
    return  cmp < 0 ? HempBefore
        :   cmp > 0 ? HempAfter
        :             HempEqual; 
}


/*--------------------------------------------------------------------------
 * integer -> xxx conversion
 *--------------------------------------------------------------------------*/

HEMP_VTEXT_FUNC(hemp_value_integer_text) {
    static hemp_char_t buffer[HEMP_BUFFER_SIZE];
    hemp_text_p text;

    snprintf(buffer, HEMP_BUFFER_SIZE, HEMP_FMT_INT, hemp_val_int(value));
    hemp_prepare_output(output, text, strlen(buffer));
    hemp_text_append_cstr(text, buffer);

    return output;
}


HEMP_VALUE_FUNC(hemp_value_integer_number) {
    return hemp_num_val((hemp_num_t) hemp_val_int(value));
}


HEMP_VALUE_FUNC(hemp_value_integer_boolean) {
    return hemp_val_int(value) == 0
        ? HempFalse
        : HempTrue;
}


HEMP_VALUE_FUNC(hemp_value_integer_compare) {
    hemp_num_t cmp = hemp_val_int(value);
    return  cmp < 0 ? HempBefore
        :   cmp > 0 ? HempAfter
        :             HempEqual; 
}


/*--------------------------------------------------------------------------
 * text -> xxx conversion
 *--------------------------------------------------------------------------*/

HEMP_VALUE_FUNC(hemp_value_text_number) {
    hemp_text_p text = hemp_val_text(value);
    hemp_cstr_p end;
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


HEMP_VALUE_FUNC(hemp_value_text_compare) {
    hemp_todo("THROW ERROR: text cannot convert to comparison");
    return HempNothing;
}



/*--------------------------------------------------------------------------
 * identity -> xxx conversions
 *--------------------------------------------------------------------------*/

HEMP_DO_INLINE hemp_cstr_p
hemp_identity_name(
    hemp_value_t value
) {
    switch (HEMP_IDENT_ID(value)) {
        case 0:                     return HEMP_STR_INFINITY;
        case HEMP_IDENT_MISSING:    return HEMP_STR_MISSING;
        case HEMP_IDENT_NOTHING:    return HEMP_STR_NOTHING;
        case HEMP_IDENT_FALSE:      return HEMP_STR_FALSE;
        case HEMP_IDENT_TRUE:       return HEMP_STR_TRUE;
        case HEMP_IDENT_BEFORE:     return HEMP_STR_BEFORE;
        case HEMP_IDENT_AFTER:      return HEMP_STR_AFTER;
        case HEMP_IDENT_EQUAL:      return HEMP_STR_EQUAL;
        default:                    return HEMP_STR_UNKNOWN;
    }
}



HEMP_VTEXT_FUNC(hemp_value_identity_text) {
    hemp_cstr_p name = hemp_identity_name(value);
    hemp_text_p text;
    hemp_prepare_output(output, text, strlen(name));
    hemp_text_append_cstr(text, name);
    return output;
}


HEMP_VALUE_FUNC(hemp_value_identity_number) {
    /* might want to auto-convert true(1), false(0), before(-1), equal(0)
     * and after(1)
     */
    HEMP_CONVERT_ERROR(
        context, 
        HEMP_STR_IDENTITY,
        HEMP_STR_NUMBER, 
        hemp_identity_name(value)
    );
}


HEMP_VALUE_FUNC(hemp_value_identity_integer) {
    /* might want to auto-convert true(1), false(0), before(-1), equal(0)
     * and after(1)
     */
    HEMP_CONVERT_ERROR(
        context, 
        HEMP_STR_IDENTITY,
        HEMP_STR_INTEGER,
        hemp_identity_name(value)
    );
}


HEMP_VALUE_FUNC(hemp_value_identity_defined) {
    return hemp_is_defined(value) 
        ? HempTrue 
        : HempFalse;
}


HEMP_VALUE_FUNC(hemp_value_identity_boolean) {
    if (hemp_is_boolean(value)) 
        return hemp_is_true(value) 
            ? HempTrue 
            : HempFalse;
    else
        HEMP_CONVERT_ERROR(
            context, 
            HEMP_STR_IDENTITY,
            HEMP_STR_BOOLEAN, 
            hemp_identity_name(value)
        );
}


HEMP_VALUE_FUNC(hemp_value_identity_compare) {
    if (hemp_is_compare(value)) 
        return value;
    else 
        HEMP_CONVERT_ERROR(
            context, 
            HEMP_STR_IDENTITY,
            HEMP_STR_COMPARE, 
            hemp_identity_name(value)
        );
}


/*--------------------------------------------------------------------------
 * boolean -> xxx conversion
 *--------------------------------------------------------------------------*/

/* Not sure about this: is undef boolean false? */
/* I'm not sure that comparisons should automatically convert to numbers... */

//HEMP_VALUE_FUNC(hemp_value_compare_number) {
//    return hemp_num_val(
//        hemp_is_before(value) ? -1 :
//        hemp_is_after(value)  ?  1 :
//                                 0
//    );
//}
//
//
//HEMP_VALUE_FUNC(hemp_value_compare_integer) {
//    return hemp_int_val(
//        hemp_is_before(value) ? -1 :
//        hemp_is_after(value)  ?  1 :
//                                 0
//    );
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

