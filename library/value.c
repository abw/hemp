#include <hemp/value.h>


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
    {   0x0F, "Identity"          },
    {   0x10, "Undefined"         },
    {   0x11, "Truth",
        &hemp_value_boolean_text,
        &hemp_value_boolean_number,
        &hemp_value_boolean_integer,
        &hemp_value_no_op,
        &hemp_value_boolean_compare
    },
    {   0x12, "-- RESERVED 0x12"  },
    {   0x13, "Compare",
        &hemp_value_compare_text,
        &hemp_value_compare_number,
        &hemp_value_compare_integer,
        &hemp_value_compare_boolean,
        &hemp_value_no_op
    },
    {   0x14, "Overload"          }
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
//  v.bits = HEMP_TYPE_INTEGER_MASK | (hemp_u64_t) i;   /* FAIL on -ve ints */
    v.bits = HEMP_TYPE_INTEGER_MASK | (hemp_u64_t)(i & HEMP_INTEGER_MASK);
    return v;
}

HEMP_DO_INLINE hemp_value_t
hemp_str_val(hemp_cstr_p s) {
    hemp_value_t v;
    v.bits = HEMP_TYPE_STRING_MASK | (hemp_u64_t) s;
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
    if (hemp_is_boolean(v)) {
        return hemp_is_true(v);
    }
    else {
        hemp_fatal("value is not boolean"); 
        return HEMP_FALSE;
    }
    
//    return hemp_is_boolean(v)
//         ? hemp_is_true(v)
//         : ({ hemp_fatal("value is not boolean"); 0 }); // shitfucks. no hemp 
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
 * boolean -> xxx conversion
 *--------------------------------------------------------------------------*/

HEMP_VTEXT_FUNC(hemp_value_boolean_text) {
    hemp_text_p text;
    hemp_prepare_output(output, text, 5);

    hemp_text_append_cstr(
        text, 
        hemp_is_true(value)
            ? HEMP_STR_TRUE
            : HEMP_STR_FALSE
    );

    return output;
}


HEMP_VALUE_FUNC(hemp_value_boolean_number) {
    /* shitfucks!  We can't throw an error because we don't have a hemp */
    hemp_todo("THROW ERROR: boolean cannot convert to number");
    return HempNothing;
}


HEMP_VALUE_FUNC(hemp_value_boolean_integer) {
    /* shitfucks!  We can't throw an error because we don't have a hemp */
    hemp_todo("THROW ERROR: boolean cannot convert to integer");
    return HempNothing;
}


HEMP_VALUE_FUNC(hemp_value_boolean_compare) {
    /* shitfucks!  We can't throw an error because we don't have a hemp */
    hemp_todo("THROW ERROR: boolean cannot convert to comparison");
    return HempNothing;
}


/*--------------------------------------------------------------------------
 * text -> xxx conversion
 *--------------------------------------------------------------------------*/

HEMP_VALUE_FUNC(hemp_value_text_number) {
    hemp_text_p text = hemp_val_text(value);
    hemp_cstr_p end;
    hemp_num_t  nval;
    
    if (! text->length) {
        hemp_todo("THROW ERROR: empty string is not a number");
        return hemp_num_val(0);
    }

    errno = 0;
    nval  = strtod(text->string, &end);
    
    if (*end || (errno == EINVAL)) {
        hemp_todo("THROW ERROR: text is not a number: %s", text->string);
    }
    else if (errno == ERANGE) {
        hemp_todo("THROW ERROR: text number is too large: %s", text->string);
    }
    else {
        return hemp_num_val(nval);
    }

    return hemp_num_val(0);
}


HEMP_VALUE_FUNC(hemp_value_text_integer) {
    hemp_value_t nval = hemp_value_text_number(value);
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
 * identity -> xxx conversion
 *--------------------------------------------------------------------------*/

HEMP_VALUE_FUNC(hemp_value_identity_number) {
    hemp_todo("THROW ERROR: %s is not a number", hemp_type_name(value));
    return hemp_num_val(0);
}

HEMP_VALUE_FUNC(hemp_value_identity_integer) {
    hemp_todo("THROW ERROR: %s is not an integer", hemp_type_name(value));
    return hemp_num_val(0);
}

/* Not sure about this: is undef boolean false? */

HEMP_VALUE_FUNC(hemp_value_identity_boolean) {
    return hemp_is_truth(value)
        ? (hemp_is_true(value) ? HempTrue : HempFalse)
        : (hemp_todo("THROW ERROR: %s is not boolean (or is it?)", hemp_type_name(value)), HempFalse);
}


HEMP_VTEXT_FUNC(hemp_value_identity_text) {
    hemp_cstr_p name = hemp_type_name(value);
    hemp_text_p text;
    hemp_prepare_output(output, text, strlen(name));
    hemp_text_append_cstr(text, name);
    return output;
}


HEMP_VTEXT_FUNC(hemp_value_compare_text) {
    hemp_text_p text;
    hemp_prepare_output(output, text, 5);

    hemp_text_append_cstr(
        text, 
        hemp_is_before(value) ? HEMP_STR_BEFORE :
        hemp_is_after(value)  ? HEMP_STR_AFTER  : 
                                HEMP_STR_EQUAL
    );

    return output;
}


/* I'm not sure that comparisons should automatically convert to numbers... */

HEMP_VALUE_FUNC(hemp_value_compare_number) {
    return hemp_num_val(
        hemp_is_before(value) ? -1 :
        hemp_is_after(value)  ?  1 :
                                 0
    );
}


HEMP_VALUE_FUNC(hemp_value_compare_integer) {
    return hemp_int_val(
        hemp_is_before(value) ? -1 :
        hemp_is_after(value)  ?  1 :
                                 0
    );
}


HEMP_VALUE_FUNC(hemp_value_compare_boolean) {
    return hemp_is_true(value)
        ? HempTrue
        : HempFalse;
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
        else if (n < 14) {
            col = HEMP_ANSI_MAGENTA;
        }
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

