#include <hemp/value.h>
#include <hemp/context.h>
#include <hemp/type/text.h>
#include <hemp/type/hash.h>


const HempValue  HempMissing = HEMP_IDENT_MAKE(HEMP_IDENT_MISSING);
const HempValue  HempNothing = HEMP_IDENT_MAKE(HEMP_IDENT_NOTHING);
const HempValue  HempEmpty   = HEMP_IDENT_MAKE(HEMP_IDENT_EMPTY);
const HempValue  HempFalse   = HEMP_IDENT_MAKE(HEMP_IDENT_FALSE);
const HempValue  HempTrue    = HEMP_IDENT_MAKE(HEMP_IDENT_TRUE);
const HempValue  HempBefore  = HEMP_IDENT_MAKE(HEMP_IDENT_BEFORE);
const HempValue  HempAfter   = HEMP_IDENT_MAKE(HEMP_IDENT_AFTER);
const HempValue  HempEqual   = HEMP_IDENT_MAKE(HEMP_IDENT_EQUAL);


/*--------------------------------------------------------------------------
 * Destructor function
 *--------------------------------------------------------------------------*/

HEMP_INLINE void
hemp_value_free(
    HempValue  value
) {
    hemp_debug_call("hemp_value_free()\n");
    hemp_clean_f cleaner = hemp_tfunc(value, clean);

    if (cleaner) {
//      hemp_debug_msg("calling cleaner function for %s: %p\n", hemp_type_name(value), cleaner);
        cleaner(value);
    }
}


/*--------------------------------------------------------------------------
 * inline functions to encode native values as tagged values
 *--------------------------------------------------------------------------*/

HEMP_INLINE HempValue
hemp_num_val(HempNum n) {
    HempValue v;
    v.number = n;
    return v;
}


HEMP_INLINE HempValue
hemp_int_val(HempInt i) {
    HempValue v;
    v.bits = HEMP_INTEGER_TAG | ((HempU64) i & HEMP_INTEGER_MASK);
    return v;
}


HEMP_INLINE HempValue
hemp_ptr_val(HempMemory p) {
    HempValue v;
    v.bits = HEMP_POINTER_TAG | HEMP_POINTER_UP(p);
    return v;
}


HEMP_INLINE HempValue
hemp_str_val(HempString s) {
    HempValue v;
    v.bits = HEMP_STRING_TAG | HEMP_POINTER_UP(s);
    return v;
}


HEMP_INLINE HempValue
hemp_text_val(HempText t) {
    HempValue v;
    v.bits = HEMP_TEXT_TAG | HEMP_POINTER_UP(t);
    return v;
}


HEMP_INLINE HempValue
hemp_list_val(HempList t) {
    HempValue v;
    v.bits = HEMP_LIST_TAG | HEMP_POINTER_UP(t);
    return v;
}


HEMP_INLINE HempValue
hemp_hash_val(HempHash t) {
    HempValue v;
    v.bits = HEMP_HASH_TAG | HEMP_POINTER_UP(t);
    return v;
}


HEMP_INLINE HempValue
hemp_code_val(HempCode c) {
    HempValue v;
    v.bits = HEMP_CODE_TAG | HEMP_POINTER_UP(c);
    return v;
}


HEMP_INLINE HempValue
hemp_params_val(HempParams p) {
    HempValue v;
    v.bits = HEMP_PARAMS_TAG | HEMP_POINTER_UP(p);
    return v;
}


HEMP_INLINE HempValue
hemp_obj_val(HempObject t) {
    HempValue v;
    v.bits = HEMP_OBJECT_TAG | HEMP_POINTER_UP(t);
    return v;
}


HEMP_INLINE HempValue
hemp_ident_val(HempU8 i) {
    HempValue v;
    v.bits = HEMP_IDENTITY_TAG | ((HempU64) i & HEMP_IDENT_MASK);
    return v;
}


HEMP_INLINE HempValue
hemp_bool_val(HempBool b) {
    return b
        ? HempTrue
        : HempFalse;
}


HEMP_INLINE HempValue
hemp_frag_val(HempFragment f) {
    return hemp_obj_val((HempObject) f);
}


HEMP_INLINE HempValue
hemp_type_val(
    HempType type, 
    HempMemory  ptr
) {
    HempValue v;
    v.bits = HEMP_TAG_MAKE(type->id) | HEMP_POINTER_UP(ptr);
    return v;
}


/*--------------------------------------------------------------------------
 * inline functions to decode tagged values to native values
 *--------------------------------------------------------------------------*/

HEMP_INLINE HempNum
hemp_val_num(HempValue v) {
    return v.number;
}


HEMP_INLINE HempInt
hemp_val_int(HempValue v) {
    return (HempInt)(v.bits & HEMP_INTEGER_MASK);
}


HEMP_INLINE HempMemory
hemp_val_ptr(HempValue v) {
    return (HempMemory) HEMP_POINTER(v);
}


HEMP_INLINE HempString
hemp_val_str(HempValue v) {
    return (HempString) HEMP_POINTER(v);
}


HEMP_INLINE HempText
hemp_val_text(HempValue v) {
    return (HempText) HEMP_POINTER(v);
}


HEMP_INLINE HempList
hemp_val_list(HempValue v) {
    return (HempList) HEMP_POINTER(v);
}


HEMP_INLINE HempHash
hemp_val_hash(HempValue v) {
    return (HempHash) HEMP_POINTER(v);
}


HEMP_INLINE HempCode
hemp_val_code(HempValue v) {
    return (HempCode) HEMP_POINTER(v);
}


HEMP_INLINE HempParams
hemp_val_params(HempValue v) {
    return (HempParams) HEMP_POINTER(v);
}


HEMP_INLINE HempObject
hemp_val_obj(HempValue v) {
    return (HempObject) HEMP_POINTER(v);
}


HEMP_INLINE HempFragment
hemp_val_frag(HempValue v) {
    return (HempFragment) HEMP_POINTER(v);
}


HEMP_INLINE HempBool
hemp_val_bool(HempValue v) {
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
    hemp_debug_call("hemp_value_self(%s)\n", hemp_type_name(value));
    return value;
}


HEMP_VALUE(hemp_value_true) {
    return HempTrue;
}


HEMP_VALUE(hemp_value_false) {
    return HempFalse;
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
    HempList values;
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
    HempValue result = hemp_call(value, value, context);
    HempParams params;
    hemp_prepare_params(context, output, params);
//  hemp_debug_msg("hemp_value_params() pushing %s => %s\n", hemp_type_name(value), hemp_type_name(result));
    hemp_params_push(params, result);
    return output;
}

// This looks wrong.  In fact all these are wrong - they should be specific
// to elements, rather than for generic values.  Also there's the chance
// of an infinite loop if a value's value() method returns itself.  The 
// function below then calls params() on it again...
// 

HEMP_OUTPUT(hemp_value_pairs) {
    hemp_debug_call("hemp_value_pairs()\n");
    HempValue  result = hemp_call(value, value, context);
    hemp_call(result, pairs, context, output);
    return output;
}


HEMP_FETCH_FUNC(hemp_value_dot) {
    hemp_debug_call("hemp_value_dot(%s)\n", hemp_type_name(container));
    HempValue result = HempMissing;

    /* call the value's fetch method, if it has one */
    HempType type = hemp_vtype(container);

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
        HempText ktext = hemp_text_new_size(16);
        hemp_onto_text(key, context, hemp_text_val(ktext));
        result = hemp_send(container, ktext->string, context);
        hemp_text_free(ktext);
    }

    return result;
}


HEMP_INLINE HempString
hemp_value_to_string(
    HempValue      value,
    HempContext    context
) {
    HempString     string;

    if (hemp_is_string(value)) {
        string = hemp_val_str(value);
//      hemp_debug_msg("input is a string: %s\n", hemp_val_str(string));
    }
    else if (hemp_is_text(value)) {
        string = hemp_val_text(value)->string;
//      hemp_debug_msg("input is text: %s\n", hemp_val_str(string));
    }
    else {
//      hemp_debug_msg("input is %s\n", hemp_type_name(value));
        HempValue text = hemp_to_text(value, context);
        string = hemp_val_text(text)->string;
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
    HempValue value
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
    HempU64  value
) {
    HempU64 mask = (HempU64) 1 << 63;
    HempU64 bit;
    HempInt n = 1;
    HempString col;
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
//    hemp_dump_32((HempU32)(value >> 32));
//    hemp_dump_32((HempU32)(HempU64) value & 0xFFFFFFFFL);
}

void hemp_dump_64(
    HempU64 value
) {
    HempU32 top = value >> 32;
    HempU32 bot = value & 0xffffffffL;
    printf("0x%016llx\n", value);
//    printf("TOP: 0x%016llx\n", top);
    hemp_dump_32((HempU32) top);
    hemp_dump_32((HempU32) bot);
    printf("\n");
}

void hemp_dump_32(
    HempU32 value
) {
    HempU32  mask  = (HempU32) 1 << 31;
    HempU32 bit;
    printf("0x%08x : ", value);

    while (mask) {
        bit = value & mask;
        mask = mask >> 1;
        printf("%c", bit ? '1' : '0');
    }
    printf("\n");
}

