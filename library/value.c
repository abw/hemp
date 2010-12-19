#include <hemp/value.h>
#include <hemp/context.h>
#include <hemp/type/text.h>
#include <hemp/type/hash.h>


const hemp_value  HempMissing = HEMP_IDENT_MAKE(HEMP_IDENT_MISSING);
const hemp_value  HempNothing = HEMP_IDENT_MAKE(HEMP_IDENT_NOTHING);
const hemp_value  HempEmpty   = HEMP_IDENT_MAKE(HEMP_IDENT_EMPTY);
const hemp_value  HempFalse   = HEMP_IDENT_MAKE(HEMP_IDENT_FALSE);
const hemp_value  HempTrue    = HEMP_IDENT_MAKE(HEMP_IDENT_TRUE);
const hemp_value  HempBefore  = HEMP_IDENT_MAKE(HEMP_IDENT_BEFORE);
const hemp_value  HempAfter   = HEMP_IDENT_MAKE(HEMP_IDENT_AFTER);
const hemp_value  HempEqual   = HEMP_IDENT_MAKE(HEMP_IDENT_EQUAL);


/*--------------------------------------------------------------------------
 * inline functions to encode native values as tagged values
 *--------------------------------------------------------------------------*/

HEMP_INLINE hemp_value
hemp_num_val(hemp_num n) {
    hemp_value v;
    v.number = n;
    return v;
}


HEMP_INLINE hemp_value
hemp_int_val(hemp_int i) {
    hemp_value v;
    v.bits = HEMP_INTEGER_TAG | ((hemp_u64) i & HEMP_INTEGER_MASK);
    return v;
}


HEMP_INLINE hemp_value
hemp_ptr_val(hemp_memory p) {
    hemp_value v;
    v.bits = HEMP_POINTER_TAG | HEMP_POINTER_UP(p);
    return v;
}


HEMP_INLINE hemp_value
hemp_str_val(hemp_string s) {
    hemp_value v;
    v.bits = HEMP_STRING_TAG | HEMP_POINTER_UP(s);
    return v;
}


HEMP_INLINE hemp_value
hemp_text_val(hemp_text t) {
    hemp_value v;
    v.bits = HEMP_TEXT_TAG | HEMP_POINTER_UP(t);
    return v;
}


HEMP_INLINE hemp_value
hemp_list_val(hemp_list t) {
    hemp_value v;
    v.bits = HEMP_LIST_TAG | HEMP_POINTER_UP(t);
    return v;
}


HEMP_INLINE hemp_value
hemp_hash_val(hemp_hash t) {
    hemp_value v;
    v.bits = HEMP_HASH_TAG | HEMP_POINTER_UP(t);
    return v;
}


HEMP_INLINE hemp_value
hemp_code_val(hemp_code c) {
    hemp_value v;
    v.bits = HEMP_CODE_TAG | HEMP_POINTER_UP(c);
    return v;
}


HEMP_INLINE hemp_value
hemp_params_val(hemp_params p) {
    hemp_value v;
    v.bits = HEMP_PARAMS_TAG | HEMP_POINTER_UP(p);
    return v;
}


HEMP_INLINE hemp_value
hemp_obj_val(hemp_object t) {
    hemp_value v;
    v.bits = HEMP_OBJECT_TAG | HEMP_POINTER_UP(t);
    return v;
}


HEMP_INLINE hemp_value
hemp_ident_val(hemp_u8 i) {
    hemp_value v;
    v.bits = HEMP_IDENTITY_TAG | ((hemp_u64) i & HEMP_IDENT_MASK);
    return v;
}


HEMP_INLINE hemp_value
hemp_bool_val(hemp_bool b) {
    return b
        ? HempTrue
        : HempFalse;
}


HEMP_INLINE hemp_value
hemp_frag_val(hemp_fragment f) {
    return hemp_obj_val((hemp_object) f);
}


HEMP_INLINE hemp_value
hemp_type_val(
    hemp_type type, 
    hemp_memory  ptr
) {
    hemp_value v;
    v.bits = HEMP_TAG_MAKE(type->id) | HEMP_POINTER_UP(ptr);
    return v;
}


/*--------------------------------------------------------------------------
 * inline functions to decode tagged values to native values
 *--------------------------------------------------------------------------*/

HEMP_INLINE hemp_num
hemp_val_num(hemp_value v) {
    return v.number;
}


HEMP_INLINE hemp_int
hemp_val_int(hemp_value v) {
    return (hemp_int)(v.bits & HEMP_INTEGER_MASK);
}


HEMP_INLINE hemp_memory
hemp_val_ptr(hemp_value v) {
    return (hemp_memory) HEMP_POINTER(v);
}


HEMP_INLINE hemp_string
hemp_val_str(hemp_value v) {
    return (hemp_string) HEMP_POINTER(v);
}


HEMP_INLINE hemp_text
hemp_val_text(hemp_value v) {
    return (hemp_text) HEMP_POINTER(v);
}


HEMP_INLINE hemp_list
hemp_val_list(hemp_value v) {
    return (hemp_list) HEMP_POINTER(v);
}


HEMP_INLINE hemp_hash
hemp_val_hash(hemp_value v) {
    return (hemp_hash) HEMP_POINTER(v);
}


HEMP_INLINE hemp_code
hemp_val_code(hemp_value v) {
    return (hemp_code) HEMP_POINTER(v);
}


HEMP_INLINE hemp_params
hemp_val_params(hemp_value v) {
    return (hemp_params) HEMP_POINTER(v);
}


HEMP_INLINE hemp_object
hemp_val_obj(hemp_value v) {
    return (hemp_object) HEMP_POINTER(v);
}

HEMP_INLINE hemp_fragment
hemp_val_frag(hemp_value v) {
    return (hemp_fragment) HEMP_POINTER(v);
}


HEMP_INLINE hemp_bool
hemp_val_bool(hemp_value v) {
    return hemp_is_true(v)
        ? HEMP_TRUE
        : HEMP_FALSE;
}





/*--------------------------------------------------------------------------
 * Generic purpose conversion, literal and no-op functions, most of which are 
 * syntactic sugar so that our type vtables are more self-documenting.
 *--------------------------------------------------------------------------*/

HEMP_VALUE(hemp_value_no_op) {
    return value;
}


HEMP_VALUE(hemp_value_self) {
    hemp_debug_msg("hemp_value_self(%s)\n", hemp_type_name(value));
    return value;
}


HEMP_VALUE(hemp_value_true) {
    return HempTrue;
}


HEMP_VALUE(hemp_value_false) {
    return HempFalse;
}


HEMP_INPUT(hemp_value_input_self) {
    hemp_debug_call("hemp_value_input_self()\n");
    return value;
}


HEMP_INPUT(hemp_value_apply) {
    /* 
     * The default behaviour for an element evaluated in a function application
     * is to return iteself.
     */
    hemp_debug_call("hemp_value_apply(%s)\n", hemp_type_name(value));
    return value;
}


HEMP_OUTPUT(hemp_value_values) {
    /* 
     * The default behaviour for an element evaluated in a list of values is
     * to add itself to the value list
     */
    hemp_debug_call("hemp_value_values()\n");
    hemp_list values;
    hemp_prepare_values(context, output, values);
    hemp_list_push(values, value);
//    hemp_debug("hemp_value_values() added value to list, now has %d items\n", values->length);
    return output;
}


HEMP_OUTPUT(hemp_value_params) {
    /* 
     * The default behaviour for an element evalauated as a parameter is to 
     * add itself to the parameter's positional args, params->ordinals
     */
    hemp_debug_call("hemp_value_params()\n");
    hemp_value result = hemp_call(value, value, context);
    hemp_params params;
    hemp_prepare_params(context, output, params);
//  hemp_debug_msg("hemp_value_params() pushing %s => %s\n", hemp_type_name(value), hemp_type_name(result));
    hemp_params_push(params, result);
    return output;
}


HEMP_FETCH_FUNC(hemp_value_dot) {
    hemp_debug_call("hemp_value_dot(%s)\n", hemp_type_name(container));
    hemp_value result = HempMissing;

    /* call the value's fetch method, if it has one */
    hemp_type type = hemp_vtype(container);

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
        hemp_text ktext = hemp_text_new_size(16);
        hemp_onto_text(key, context, hemp_text_val(ktext));
        result = hemp_send(container, ktext->string, context);
        hemp_text_free(ktext);
    }

    return result;
}


HEMP_INLINE hemp_value
hemp_value_to_string(
    hemp_value      value,
    hemp_context    context
) {
    hemp_value      string;

    if (hemp_is_string(value)) {
        string = value;
//      hemp_debug_msg("input is a string: %s\n", hemp_val_str(string));
    }
    else if (hemp_is_text(value)) {
        string = hemp_str_val(hemp_val_text(value)->string);
//      hemp_debug_msg("input is text: %s\n", hemp_val_str(string));
    }
    else {
//      hemp_debug_msg("input is %s\n", hemp_type_name(value));
        hemp_value text = hemp_to_text(value, context);
        string = hemp_str_val(hemp_val_text(text)->string);
//      hemp_debug_msg("converted to text: %s\n", hemp_val_str(string));
    }
    return string;
}


/*--------------------------------------------------------------------------
 * Default "I'm sorry Dave, I'm afraid I can't do that" functions
 *--------------------------------------------------------------------------*/

HEMP_OUTPUT(hemp_value_not_text) {
    HEMP_CONVERT_ERROR(
        context, 
        hemp_type_name(value),
        HEMP_STR_TEXT,
        HEMP_STR_BLANK
    );
    return output;
}


HEMP_OUTPUT(hemp_value_not_pairs) {
    HEMP_CONVERT_ERROR(
        context,
        hemp_type_name(value),
        HEMP_STR_PAIRS,
        HEMP_STR_BLANK
    );
    return output;
}


HEMP_VALUE(hemp_value_not_number) {
    HEMP_CONVERT_ERROR(
        context, 
        hemp_type_name(value),
        HEMP_STR_NUMBER,
        HEMP_STR_BLANK
    );
    return HempMissing;
}


HEMP_VALUE(hemp_value_not_integer) {
    HEMP_CONVERT_ERROR(
        context, 
        hemp_type_name(value),
        HEMP_STR_INTEGER,
        HEMP_STR_BLANK
    );
    return HempMissing;
}


HEMP_VALUE(hemp_value_not_boolean) {
    HEMP_CONVERT_ERROR(
        context, 
        hemp_type_name(value),
        HEMP_STR_BOOLEAN,
        HEMP_STR_BLANK
    );
    return HempMissing;
}


HEMP_VALUE(hemp_value_not_compare) {
    HEMP_CONVERT_ERROR(
        context, 
        hemp_type_name(value),
        HEMP_STR_COMPARE,
        HEMP_STR_BLANK
    );
    return HempMissing;
}


HEMP_VALUE(hemp_value_not_defined) {
    HEMP_CONVERT_ERROR(
        context, 
        hemp_type_name(value),
        HEMP_STR_DEFINED,
        HEMP_STR_BLANK
    );
    return HempMissing;
}


HEMP_FETCH_FUNC(hemp_value_not_fetch) {
    HEMP_FETCH_ERROR(
        context, 
        hemp_type_name(container),
        "Cannot fetch"
    );
    return HempMissing;
}


HEMP_STORE_FUNC(hemp_value_not_store) {
    HEMP_STORE_ERROR(
        context, 
        hemp_type_name(container),
        "Cannot store"
    );
    return HempMissing;
}


HEMP_FETCH_FUNC(hemp_value_not_dot) {
    HEMP_FETCH_ERROR(
        context, 
        hemp_type_name(container),
        "Cannot dot"
    );
    return HempMissing;
}



/*--------------------------------------------------------------------------
 * debugging
 *--------------------------------------------------------------------------*/

void hemp_dump_value(
    hemp_value value
) {
    hemp_dump_u64(value.bits);
    /*
    printf("TYPE: %s\n", hemp_type_name(value));
    if (hemp_is_text(value)) {
        printf("TEXT: %s\n", hemp_val_text(value)->string);
    }
    else if (hemp_is_string(value)) {
        printf("TEXT: %s\n", hemp_val_str(value));
    }
    */
}


void hemp_dump_u64(
    hemp_u64  value
) {
    hemp_u64 mask = (hemp_u64) 1 << 63;
    hemp_u64 bit;
    hemp_int n = 1;
    hemp_string col;
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
//    hemp_dump_32((hemp_u32)(value >> 32));
//    hemp_dump_32((hemp_u32)(hemp_u64) value & 0xFFFFFFFFL);
}

void hemp_dump_64(
    hemp_u64 value
) {
    hemp_u32 top = value >> 32;
    hemp_u32 bot = value & 0xffffffffL;
    printf("0x%016llx\n", value);
//    printf("TOP: 0x%016llx\n", top);
    hemp_dump_32((hemp_u32) top);
    hemp_dump_32((hemp_u32) bot);
    printf("\n");
}

void hemp_dump_32(
    hemp_u32 value
) {
    hemp_u32  mask  = (hemp_u32) 1 << 31;
    hemp_u32 bit;
    printf("0x%08x : ", value);

    while (mask) {
        bit = value & mask;
        mask = mask >> 1;
        printf("%c", bit ? '1' : '0');
    }
    printf("\n");
}

